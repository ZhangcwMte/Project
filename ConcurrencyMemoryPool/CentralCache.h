#pragma once
#include"Common.h"

//单例模式
class CentralCache
{
public:
	static CentralCache* getInstance()
	{
		return &m_instance;
	}

	//获取一个非空Span
	Span* getOneSpan(SpanList& list, size_t size);
	//从中心缓存获取一定数量的对象调度给ThreadCache
	size_t fetchRangeObj(void*& start, void*& end, size_t batch_num, size_t size);

	void releaseListToSpans(void* start, size_t size);

private:
	CentralCache()
	{}

	CentralCache(const CentralCache&) = delete;

	static CentralCache m_instance;

private:
	SpanList m_span_lists[NFREELIST];
};