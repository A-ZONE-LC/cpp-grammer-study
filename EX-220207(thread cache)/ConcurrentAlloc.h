#pragma once 
#include"Common.h"
#include"ThreadCache.h"


//ͨ��TLS,ÿ���߳������Ļ�ȡ�Լ���ר����ThreadCache����
static void* ConcurrentAlloc(size_t size) //staticʹֻ�ڵ�ǰ�ļ��οɼ�
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