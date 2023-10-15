#pragma once
#include"ThreadCache.h"
#include"PageCache.h"

static void* concurrentAlloc(size_t size)
{
	if (size > MAX_BYTES)
	{
		size_t align_size = SizeClass::roundUp(size);
		size_t page_num = align_size >> PAGE_SHIFT;

		PageCache::getInstance()->getMutex().lock();
		Span* span = PageCache::getInstance()->newSpan(page_num);
		span->m_obj_size = size;
		PageCache::getInstance()->getMutex().unlock();

		void* ptr = (void*)(span->m_page_id << PAGE_SHIFT);

		return ptr;
	}
	// 通过TLS 每个线程无锁的获取自己的专属的ThreadCache对象
	if (p_tls_threadcache == nullptr)
	{
		static ObjectPool<ThreadCache> tc_pool;
		p_tls_threadcache = tc_pool.New();
	}



	return p_tls_threadcache->allocate(size);
}

static void concurrentFree(void* ptr)
{
	Span* span = PageCache::getInstance()->mapObjectToSpan(ptr);
	size_t size = span->m_obj_size;

	if (size > MAX_BYTES)
	{
		PageCache::getInstance()->getMutex().lock();
		PageCache::getInstance()->releaseSpanToPageCache(span);
		PageCache::getInstance()->getMutex().unlock();
	}
	else
	{
		assert(p_tls_threadcache);	
		p_tls_threadcache->deallocate(ptr, size);
	}
}