#pragma once
#include"Common.h"

class ThreadCache
{
public:
	void* FetchFromCentralCache(size_t size, size_t index); //��central cache ��ȡ�ռ�
	void* Allocate(size_t size); //thread cache ����ռ�
	void Deallocate(void* ptr, size_t size); //thread cache ���տռ�
private:
	FreeList _freeListArray[NFREELIST]; //��ϣͰ
};

//TLS thread local storage(thread cache����Ҫ�����ĸ���ԭ��)
static _declspec(thread) ThreadCache* pTLSThreadCache = nullptr;