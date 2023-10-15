#pragma once
#include"Common.h"
#include"PageMap.h"

//单例模式
class PageCache
{
public:
	static PageCache* getInstance()
	{
		return &m_instance;
	}

	std::mutex& getMutex()
	{
		return m_page_mtx;
	}

	//获取一个K页的Span
	Span* newSpan(size_t k);
	//通过pageid映射查找Span
	Span* mapObjectToSpan(void* obj);
	//页合并
	void releaseSpanToPageCache(Span* span);

private:
	PageCache()
	{}

	PageCache(const PageCache&) = delete;

	static PageCache m_instance;

private:
	//Span对象池
	ObjectPool<Span> m_span_pool;

	SpanList m_span_lists[NPAGES];
	//pageid与Span的映射关系
	TCMalloc_PageMap1<32 - PAGE_SHIFT> m_span_id_map;
	//大锁
	std::mutex m_page_mtx;
};