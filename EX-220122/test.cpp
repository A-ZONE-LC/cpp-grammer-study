/*
 * @Author: your name
 * @Date: 2022-01-22 09:43:36
 * @LastEditTime: 2022-01-22 14:14:16
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: \EX-220122\test.cpp
 */
#pragma pack(8)
#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cstring>

void test1()
{
    int a = 1;
    int b = 10;
    auto swap = [](int& a, int& b)->void{ int tmp = a; a = b; b = tmp;};
    swap(a, b);
    std::cout << a << ' ' << b << std::endl;
}

void test2()
{
    int a = 2;
    int b = 20;
    auto swap = [&a, &b]{ int tmp = a; a = b; b = tmp;};
    swap();
    std::cout << a << ' ' << b << std::endl;
}

void test3()
{
    int a = 3;
    int b = 30;
    auto swap = [&]{int tmp = a; a = b; b = tmp;};
    swap();
    std::cout << a << ' ' << b << std::endl;
}

void test4()
{
    int a = 4;
    int b = 40;
    auto swap = [a, b]()mutable {int tmp = a; a = b; b = tmp;}; //!里面的更改不会影响外面
    swap();
    std::cout << a << ' ' << b << std::endl; //!未交换
}

char func(char x, char y)
{
    if(x < y)
        return x;
    return y;
}

struct One{
	double d;
	char c;
	int i;
};
struct Two{
	char c;
	double d;
	int i;
};


//? 32位机器与64位机器：
//? 指针大小不一样：32位指针4个字节，64位指着 8个字节
//? long 32位4个字节 64位8个字节
//? unsigned long 32位4个字节 64位8个字节


#define SQR(A) A*A

int main()
{
    //test4()
    // int a = '1';
    // int b = '1';
    // int c = '2';
    // std::cout << func(func(a, b), func(b, c));
    //std::cout << "#pragma pack8: " << sizeof(struct One) << ' ' << sizeof(struct Two) << std::endl;
    // char p1[15]= "abcd",*p2= "ABCD", str[50]= "xyz"; 
    // strcpy(str+2,strcat(p1+2,p2+1)); 
    // printf("%s",str);

    //std::cout << INT_MIN << std::endl;

    int x=6,y=3,z=2; 
    x/=SQR(y+z)/SQR(y+z); 
    std::cout << x << std::endl; 
    return 0;
}