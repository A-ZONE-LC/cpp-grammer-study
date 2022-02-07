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

	//桶不为空
	if (!_freeListArray[index].empty()) 
	{
		return _freeListArray[index].pop();
	}
	//桶为空，没有空间可供使用，需要从下层的central cache获取
	else
	{
		return FetchFromCentralCache(alignSize, index);
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(ptr);
	assert(size <= MAX_BYTES);

	//找到映射的自由链表桶，并将归还的空间插入自由链表
	size_t index = SizeClass::Index(size);
	_freeListArray[index].push(ptr);
}

