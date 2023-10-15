#include"ThreadCache.h"
#include"CentralCache.h"

void* ThreadCache::allocate(size_t size)
{
	assert(size <= MAX_BYTES);
	size_t align_size = SizeClass::roundUp(size);
	size_t index = SizeClass::index(size);

	if (!m_free_lists[index].empty())
		return m_free_lists[index].pop();
	else
		return fetchFromCentralCache(index, align_size);
}
void ThreadCache::deallocate(void* ptr, size_t size)
{
	assert(ptr);
	assert(size < MAX_BYTES);
	
	size_t index = SizeClass::index(size);
	m_free_lists[index].push(ptr);

	// 当链表长度大于一次批量申请的内存时就返还一段list给centralcache
	if (m_free_lists[index].size() >= m_free_lists[index].getMaxSize())
	{
		listExceeded(m_free_lists[index], size);
	}

}

void* ThreadCache::fetchFromCentralCache(size_t index, size_t size)
{
	// 慢开始反馈调节算法
	// 1、最开始不会一次向central cache一次批量要太多，因为要太多了可能用不完
	// 2、如果你不要这个size大小内存需求，那么batchNum就会不断增长，直到上限
	// 3、size越大，一次向central cache要的batchNum就越小
	// 4、size越小，一次向central cache要的batchNum就越大
	size_t batch_num = min(m_free_lists[index].getMaxSize(), SizeClass::numMoveSize(size));
	if (m_free_lists[index].getMaxSize() == batch_num)
		m_free_lists[index].getMaxSize()++;

	void* start = nullptr;
	void* end = nullptr;
	size_t actual_num = CentralCache::getInstance()->fetchRangeObj(start, end, batch_num, size);

	assert(actual_num > 0);//至少获取到一个对象空间
	if (actual_num == 1)
	{
		assert(start == end);
		return start;
	}
	else
	{
		m_free_lists[index].pushRange(*(void**)start, end , actual_num - 1);//返回一个，剩下的挂到自由链表中
		return start;
	}
}

void ThreadCache::listExceeded(FreeList& list, size_t size)
{
	void* start = nullptr;
	void* end = nullptr;
	list.popRange(start, end, list.getMaxSize());

	CentralCache::getInstance()->releaseListToSpans(start, size);
}