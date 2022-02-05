/*
 * @Author: your name
 * @Date: 2022-02-05 23:05:48
 * @LastEditTime: 2022-02-05 23:05:49
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \EX-220205\thread_cache.cpp
 */

#include"thread_cache.hpp"

void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAX_BYTES);
	size_t alignSize = SizeClass::RoundUp(size);
	size_t index = SizeClass::Index(size);

	if (!_freeListArray[index].empty()) //桶不为空
	{
		return _freeListArray[index].pop();
	}
	else
	{

	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{

}