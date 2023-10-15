#include"Common.h"

class ThreadCache
{
public:
	//申请内存对象
	void* allocate(size_t size);
	//释放内存对象
	void deallocate(void* ptr, size_t size);
	//从中心缓存获取对象
	void* fetchFromCentralCache(size_t index, size_t size);

	void listExceeded(FreeList& list, size_t size);

private:
	FreeList m_free_lists[NFREELIST];
};

//线程局部存储（TLS），是一种变量的存储方法，这个变量在它所在的线程内是全局可访问的，但是不能被其他线程访问到，这样就保持了数据的线程独立性。
static _declspec(thread) ThreadCache* p_tls_threadcache;