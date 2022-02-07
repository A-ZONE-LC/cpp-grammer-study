#include"ThreadCache.h"

void* ThreadCache::FetchFromCentralCache(size_t size, size_t index)
{
	//...
	return nullptr;
}

void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAX_BYTES);
	size_t alignSize = SizeClass::RoundUp(size);
	size_t index = SizeClass::Index(size);

	//Ͱ��Ϊ��
	if (!_freeListArray[index].empty()) 
	{
		return _freeListArray[index].pop();
	}
	//ͰΪ�գ�û�пռ�ɹ�ʹ�ã���Ҫ���²��central cache��ȡ
	else
	{
		return FetchFromCentralCache(alignSize, index);
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(ptr);
	assert(size <= MAX_BYTES);

	//�ҵ�ӳ�����������Ͱ�������黹�Ŀռ������������
	size_t index = SizeClass::Index(size);
	_freeListArray[index].push(ptr);
}

