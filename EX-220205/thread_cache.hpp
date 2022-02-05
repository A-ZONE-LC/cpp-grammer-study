#pragma once
#include"common.hpp"

class ThreadCache
{
public:
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);
private:
	FreeList _freeListArray[]; //哈希桶
};