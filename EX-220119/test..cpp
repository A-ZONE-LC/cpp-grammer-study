/*
 * @Author: your name
 * @Date: 2022-01-19 07:07:30
 * @LastEditTime: 2022-01-20 19:01:45
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \EX-220119\test..cpp
 */
#include<iostream>
#include<vector>
#include<string>
#include<initializer_list>
#include<list>
#include<array>
#include<stdio.h>

using namespace std;

class Date
{
public:
    Date(int year = 2022, int month = 1, int day = 19)
        :_year(year)
        ,_month(month)
        ,_day(day)
    {
        cout << "date constructor" << endl;
    }
private:
    int _year;
    int _month;
    int _day;
};

void test1()
{
    int x{1};

    int arr[]{1, 2, 3, 4, 5};

    int* p = new int[5]{1, 2, 3, 4, 5};
    int* q = new int{1};

    Date d1{2022, 2, 22};
    Date d2 = {2021, 1, 21};
}

void test2()
{
    auto a = {1, 2, 3};
    cout << typeid(a).name() << endl; //*St16initializer_listIiE
    std::initializer_list<int> il{1, 2, 3, 4, 5};
    cout << typeid(il).name() << endl; //*St16initializer_listIiE

    vector<int> v = {1, 3, 4};
    list<int> l = {1, 3, 4};
    for(auto& e: v)
        cout << e << ' ';
    cout << endl;
    for(auto& e: l)
        cout << e << ' ';
    cout << endl;
}

void test3()
{
    const int x = 1;
    float y = 2.0;
    decltype(x*y) z;
    cout << typeid(z).name() << endl;

    decltype(z) a;
    cout << typeid(a).name() << endl;
}

void test4()
{
    array<int, 5> a{1, 2, 3, 4, 5};
    cout << a.at(4) << endl; //*terminate called after throwing an instance of 'std::out_of_range'
    //*what():  array::at: __n (which is 5) >= _Nm (which is 5)
}


struct A
{
 int a;
 short b;
 int c;
 char d;
};
struct B
{
 int a;
 short b;
 char c;
 int d;
};

int main()
{
    //test1();
    //test2();
    //test3();
    cout << sizeof(struct A) << endl;
    cout << sizeof(struct B) << endl;
    return 0;
}
