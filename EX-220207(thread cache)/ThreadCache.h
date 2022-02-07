#pragma once
#include"Common.h"

class ThreadCache
{
public:
	void* FetchFromCentralCache(size_t size, size_t index); //从central cache 获取空间
	void* Allocate(size_t size); //thread cache 分配空间
	void Deallocate(void* ptr, size_t size); //thread cache 回收空间
private:
	FreeList _freeListArray[NFREELIST]; //哈希桶
};

//TLS thread local storage(thread cache不需要加锁的根本原因)
static _declspec(thread) ThreadCache* pTLSThreadCache = nullptr;