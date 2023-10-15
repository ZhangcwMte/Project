#include"PageCache.h"

PageCache PageCache::m_instance;

//获取一个k页的Span
Span* PageCache::newSpan(size_t k)
{
	assert(k > 0);

	if (k >= NPAGES)
	{
		void* ptr = systemAlloc(k);
		Span* span = m_span_pool.New();
		span->m_page_id = (PAGE_ID)ptr >> PAGE_SHIFT;
		span->m_page_num = k;

		m_span_id_map.set(span->m_page_id , span);

		return span;
	}

	// 先检查第k个桶里面有没有span，若有直接返回
	if (!m_span_lists[k].empty())
	{
		Span* span = m_span_lists[k].popFront();

		for (size_t i = 0; i < span->m_page_num; i++)
		{
			m_span_id_map.set(span->m_page_id + i, span);
		}

		return span;
	}
	//若没有，继续向后检查
	for (size_t i = k + 1; i < NPAGES; i++)
	{
		//若找到，进行切割，切割成目标页数的Span
		if (!m_span_lists[i].empty())
		{
			Span* n_span = m_span_lists[i].popFront();
			Span* k_span = m_span_pool.New();

			//在n_span的头部切下目标Span，并将剩余的n_span重新挂入对应映射的桶中
			k_span->m_page_id = n_span->m_page_id;
			k_span->m_page_num = k;

			n_span->m_page_id += k;
			n_span->m_page_num -= k;

			m_span_lists[n_span->m_page_num].pushFront(n_span);
			//建立首尾pageid与span的映射关系，方便合并
			m_span_id_map.set(n_span->m_page_id, n_span);
			m_span_id_map.set(n_span->m_page_id + n_span->m_page_num - 1, n_span);

			//切分出去的span每一页都与pageid映射，方便回收小块内存
			for (size_t i = 0; i < k_span->m_page_num; i++)
			{
				m_span_id_map.set(k_span->m_page_id + i, k_span);
			}
			return k_span;
		}
	}

	//若没有大块内存，向堆区申请
	Span* big_span = m_span_pool.New();
	void* ptr = systemAlloc(NPAGES - 1);
	big_span->m_page_id = (PAGE_ID)ptr >> PAGE_SHIFT;
	big_span->m_page_num = NPAGES - 1;

	m_span_lists[big_span->m_page_num].pushFront(big_span);

	return newSpan(k);
}

Span* PageCache::mapObjectToSpan(void* obj)
{
	PAGE_ID id = (PAGE_ID)obj >> PAGE_SHIFT;
	auto ret = m_span_id_map.get(id);
	assert(ret != nullptr);
	return (Span*)ret;

}

//页合并
void PageCache::releaseSpanToPageCache(Span* span)
{
	if (span->m_page_num >= NPAGES)
	{
		void* ptr = (void*)(span->m_page_id << PAGE_SHIFT);

		systemFree(ptr);

		m_span_pool.Delete(span);

		return;
	}

	//向前合并
	while (true)
	{
		PAGE_ID prev_id = span->m_page_id - 1;
		auto ret = m_span_id_map.get(prev_id);

		if (ret == nullptr)//若没找到
			break;

		Span* prev_span = (Span*)ret;
		if (prev_span->m_is_using == true)//若正在被使用
			break;

		if (span->m_page_num + prev_span->m_page_num > NPAGES - 1)//若合并后超出128页，无法进行管理
			break;

		span->m_page_id = prev_span->m_page_id;
		span->m_page_num += prev_span->m_page_num;

		m_span_lists[prev_span->m_page_num].erase(prev_span);
		m_span_pool.Delete(prev_span);
	}

	//向后合并
	while (true)
	{
		PAGE_ID next_id = span->m_page_id - 1;
		auto ret = m_span_id_map.get(next_id);

		if (ret == nullptr)
			break;

		Span* next_span = (Span*)ret;
		if (next_span->m_is_using == true)
			break;

		if (span->m_page_num + next_span->m_page_num > NPAGES - 1)
			break;

		span->m_page_num += next_span->m_page_num;

		m_span_lists[next_span->m_page_num].erase(next_span);
		m_span_pool.Delete(next_span);
	}

	m_span_lists[span->m_page_num].pushFront(span);
	span->m_is_using = false;
	m_span_id_map.set(span->m_page_id, span);
	m_span_id_map.set(span->m_page_id + span->m_page_num - 1, span);
}