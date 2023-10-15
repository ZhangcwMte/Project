#include"CentralCache.h"
#include"PageCache.h"

CentralCache CentralCache::m_instance;

//获取一个非空Span
Span* CentralCache::getOneSpan(SpanList& list, size_t size)
{
	//检查是否有现成的span
	Span* cur = list.begin();
	while (cur != list.end())
	{
		if (cur->m_span_free_list)
			return cur;

		cur = cur->m_next;
	}
	//若未找到，向pagecache申请一块
	//先解开桶锁，不阻塞其它线程
	list.getMutex().unlock();

	PageCache::getInstance()->getMutex().lock();
	Span* span = PageCache::getInstance()->newSpan(SizeClass::numMovePage(size));
	span->m_is_using = true;
	span->m_obj_size = size;
	PageCache::getInstance()->getMutex().unlock();

	// 计算span的大块内存的起始地址和大块内存的大小(字节数)
	char* start = (char*)(span->m_page_id << PAGE_SHIFT);
	size_t bytes = span->m_page_num << PAGE_SHIFT;
	char* end = start + bytes;

	// 把大块内存切成自由链表链接起来
	span->m_span_free_list = start;
	void* tail = start;
	start += size;
	while (start < end)
	{
		*(void**)tail = start;
		tail = start;
		start += size;
	}
	*(void**)tail = nullptr;

	// 切好span以后，需要把span挂到桶里面去的时候，再加锁
	list.getMutex().lock();
	list.pushFront(span);

	return span;
}

size_t CentralCache::fetchRangeObj(void*& start, void*& end, size_t batch_num, size_t size)
{
	size_t index = SizeClass::index(size);

	m_span_lists[index].getMutex().lock();

	Span* span = getOneSpan(m_span_lists[index], size);

	assert(span);
	assert(span->m_span_free_list);

	//从Span上摘取batch_num个对象
	start = span->m_span_free_list;
	end = start;
	size_t i = 0;
	size_t actual_num = 1;
	while (i < batch_num - 1 && *(void**)end != nullptr)//如果不够，有多少拿多少
	{
		end = *(void**)end;
		actual_num++;
		i++;
	}

	void* ptr = nullptr;
	span->m_span_free_list = *(void**)end;
	span->m_use_count += actual_num;
	*(void**)end = nullptr;

	m_span_lists[index].getMutex().unlock();

	return actual_num;
}

void CentralCache::releaseListToSpans(void* obj, size_t size)
{
	size_t index = SizeClass::index(size);
	m_span_lists[index].getMutex().lock();
	while (obj)
	{
		void* next = *(void**)obj;

		Span* span = PageCache::getInstance()->mapObjectToSpan(obj);
		*(void**)obj = span->m_span_free_list;
		span->m_span_free_list = obj;
		span->m_use_count--;

		if (span->m_use_count == 0)
		{
			m_span_lists[index].erase(span);
			span->m_next = nullptr;
			span->m_prev = nullptr;
			span->m_span_free_list = nullptr;

			m_span_lists[index].getMutex().unlock();

			PageCache::getInstance()->getMutex().lock();
			PageCache::getInstance()->releaseSpanToPageCache(span);
			PageCache::getInstance()->getMutex().unlock();

			m_span_lists[index].getMutex().lock();
		}

		obj = next;
	}
	m_span_lists[index].getMutex().unlock();
}