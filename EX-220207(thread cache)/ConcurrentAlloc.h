#pragma once 
#include"Common.h"
#include"ThreadCache.h"


//通过TLS,每个线程无锁的获取自己的专属的ThreadCache对象
static void* ConcurrentAlloc(size_t size) //static使只在当前文件课可见
{
	assert(size <= MAX_BYTES);
	if (pTLSThreadCache == nullptr)
	{
		pTLSThreadCache = new ThreadCache;
	}

	cout << std::this_thread::get_id() << ":" << pTLSThreadCache << endl;
	return pTLSThreadCache->Allocate(size);
}

static void ConcurrentDealloc(void* ptr, size_t size)
{
	assert(pTLSThreadCache);
	pTLSThreadCache->Deallocate(ptr, size);
}