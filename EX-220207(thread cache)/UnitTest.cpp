#include"ConcurrentAlloc.h"

void Alloc_1()
{
	for (int i = 0; i <= 5; i++)
	{
		void* ptr = ConcurrentAlloc(12);
	}
}

void Alloc_2()
{
	for (int i = 0; i <= 6; i++)
	{
		void* ptr = ConcurrentAlloc(8);
	}
}

void TLSTest()
{
	std::thread th1(Alloc_1);
	th1.join();

	std::thread th2(Alloc_2);
	th2.join();
}

int main()
{
	TLSTest();
	return 0;
}