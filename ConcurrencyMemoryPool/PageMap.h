#pragma once
#include"Common.h"

template <int BITS>
class TCMalloc_PageMap1 {
private:
	static const int LENGTH = 1 << BITS;
	void** array_;

public:
	typedef uintptr_t Number;


	explicit TCMalloc_PageMap1() {
		size_t size = sizeof(void*) << BITS;
		size_t alignSize = SizeClass::_roundUp(size, 1 << PAGE_SHIFT);
		array_ = (void**)systemAlloc(alignSize >> PAGE_SHIFT);
		memset(array_, 0, sizeof(void*) << BITS);
	}

	void* get(Number k) const {
		if ((k >> BITS) > 0) {
			return NULL;
		}
		return array_[k];
	}

	void set(Number k, void* v) {
		array_[k] = v;
	}
};
