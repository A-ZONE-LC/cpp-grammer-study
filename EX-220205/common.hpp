/*
 * @Author: A-ZONE
 * @Date: 2022-02-05 23:05:08
 * @LastEditTime: 2022-02-05 23:08:49
 * @LastEditors: Please set LastEditors
 * @Description: thread cache 的实现（草稿一）
 * @FilePath: \EX-220205\common.hpp
 */

#pragma once
#include<iostream>
#include<vector>
#include<assert.h>

using std::cout;
using std::endl;

static const size_t MAX_BYTES = 256 * 1024; //能申请的最大字节数

void*& NextObj(void* obj) //找到下一个小内存块
{
	return *(void**)obj;
}

class FreeList //自由链表
{
public:
	void push(void* obj) //头插
	{
		assert(obj);
		NextObj(obj) = _freeList;
		_freeList = obj;
	}

	void* pop() //头删
	{
		assert(_freeList);
		void* obj = _freeList;
		_freeList = NextObj(obj);
		return obj;
	}

	bool empty()
	{
		return _freeList == nullptr;
	}
private:
	void* _freeList;
};

class SizeClass //计算对象大小的对齐映射规则
{
public:
	// 整体控制在最多10%左右的内碎片浪费
	// [1,128]					8byte对齐	    freelist[0,16)
	// [128+1,1024]				16byte对齐	    freelist[16,72)
	// [1024+1,8*1024]			128byte对齐	    freelist[72,128)
	// [8*1024+1,64*1024]		1024byte对齐     freelist[128,184)
	// [64*1024+1,256*1024]		8*1024byte对齐   freelist[184,208)
	static inline size_t RoundUp(size_t size) //计算向上对齐到多大字节
	{
		assert(size <= MAX_BYTES);
		if (size <= 128)
		{
			return _RoundUp(size, 8);
		}
		else if (size <= 1024)
		{
			return _RoundUp(size, 16);
		}
		else if (size <= 8 * 1024)
		{
			return _RoundUp(size, 128);
		}
		else if (size <= 64 * 1024)
		{
			return _RoundUp(size, 1024);
		}
		else if (size <= 256 * 1024)
		{
			return _RoundUp(size, 8 * 1024);
		}
		else
		{
			assert(false);
		}
		return -1;
	}

	
	static inline size_t Index(size_t bytes) //计算映射的哪一个自由链表桶
	{
		assert(bytes <= MAX_BYTES);

		//每个区间有多少个链
		static int group_array[4] = { 16, 56, 56, 56 };
		if (bytes <= 128) {
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024) 
		{
			return _Index(bytes - 128, 4) + group_array[0];
		}
		else if (bytes <= 8 * 1024) 
		{
			return _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (bytes <= 64 * 1024) 
		{
			return _Index(bytes - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
		}
		else if (bytes <= 256 * 1024) 
		{
			return _Index(bytes - 64 * 1024, 13) + group_array[3] + group_array[2] + group_array[1] + group_array[0];
		}
		else 
		{
			assert(false);
		}
		return -1;
	}
private:
	/*static inline size_t _RoundUp(size_t size, size_t alignNum)
	{
		size_t alignSize;
		if (size % alignNum == 0)
		{
			alignSize = size;
		}
		else
		{
			alignSize = (size / alignNum + 1) * alignNum;
		}
		return alignSize;
	}*/

	static inline size_t _RoundUp(size_t bytes, size_t alignNum)
	{
		return ((bytes + alignNum - 1) & ~(alignNum - 1));
	}

	/*size_t _Index(size_t bytes, size_t alignNum)
	{
		if (bytes % alignNum == 0)
		{
			return bytes / alignNum - 1;
		}
		else
		{
			return bytes / alignNum;
		}
	}*/
	static inline size_t _Index(size_t bytes, size_t align_shift)
	{
		return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
	}
};
