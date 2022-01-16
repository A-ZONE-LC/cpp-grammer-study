/*
 * @Author: A-ZONE-LC
 * @Date: 2022-01-16 10:18:32
 * @LastEditTime: 2022-01-16 16:50:04
 * @LastEditors: Please set LastEditors
 * @Description: 特殊类与单例模式（饿汉与懒汉）
 * @FilePath: \EX-0116\test.cpp
 */
#include<iostream>
#include<mutex>
using namespace std;

//*只能在堆上创建对象
class HeapOnly1     //?C++11
{
public:
    static HeapOnly1* CreateHeapOnly()   //?如果借没有static，调用CreateHeapOnly就要使用对象，而对象还未被创建
    {
        return new HeapOnly1();  //?在类外不能调用构造函数，但是在类内可以调用，并按指定的方式调用
    }
    HeapOnly1(const HeapOnly1& HeapOnly) = delete; //?防止从栈上创建的对象拷贝构造给该对象
private:
    HeapOnly1() {};  //?将默认构造函数私有
}; 


class HeapOnly2     //?C++98
{
public:
    static HeapOnly2* CreateHeapOnly()   //?如果借没有static，调用CreateHeapOnly就要使用对象，而对象还未被创建
    {
        return new HeapOnly2();  //?在类外不能调用构造函数，但是在类内可以调用，并按指定的方式调用
    }
private:
    HeapOnly2() {};  //?将默认构造函数私有
    HeapOnly2(const HeapOnly2& HeapOnly); //?防止从栈上创建的对象拷贝构造给该对象
}; 


//*-----------------------------------------------------------------------------------------------------------
//*只能在栈上创建对象
class StackOnly1    //?C++98
{
public:
    static StackOnly1 CreateStackOnly()
    {
        return StackOnly1();
    }
private:
    StackOnly1() {}; //?这里不可以将拷贝构造私有，CreateStackOnly返回时需要拷贝构造
};


class StackOnly2    //?C++11
{
public:
    StackOnly2() {};
    void* operator new(size_t size) = delete;   //?禁用operator new
    void operator delete(void* p) = delete;     //?禁用operator delete
private:
    //...
    
};

//*-----------------------------------------------------------------------------------------------------------
//*对象不可以被拷贝
class BanCopy
{
public:
    BanCopy() {};
private:
    BanCopy(const BanCopy& bc);     //?拷贝构造私有
    BanCopy& operator=(const BanCopy& bc);  //?赋值重载私有
};


//*-----------------------------------------------------------------------------------------------------------
//*类不可以被继承
class BanDerive     //?C++98 (可以被继承，生成对象时报错)
{
public:
    static BanDerive GetInstence()
    {
        return BanDerive();
    }
private:
    BanDerive() {};     //?构造函数私有
};


class BanDerive1 final  //?C++11 (继承就会报错)
{
public:
    BanDerive1() {};
private:
    //...
};


//*-----------------------------------------------------------------------------------------------------------
//*单例模式（饿汉模式） 

//*#1.将构造函数，拷贝构造，赋值重载私有化
//*#2.提供一个GetInstance()获取单例对象
//*#3.类内含有一个自己类的static指针，并在main函数之前初始化
                      
class Singleton1
{
public:
    static Singleton1* GetInstance()  //?获取实例化对象
    {
        return _sgl;
    }

    void Print()
    {
        cout << "print:" << _a << endl;
    }
private:
    Singleton1() {};
    Singleton1(const Singleton1& s) = delete;
    Singleton1& operator=(const Singleton1& s) = delete;
    //...
    int _a;
    static Singleton1* _sgl;
};
Singleton1* Singleton1::_sgl = new Singleton1;     //?在main函数之前就已经创建

//*-----------------------------------------------------------------------------------------------------------
//*单例模式（懒汉模式）

//*#1.将构造函数，拷贝构造，赋值重载私有化
//*#2.提供一个GetInstance()获取单例对象
//*#3.类内含有一个自己类的static指针，并置空，不进行初始化
//*#4._sgl放到GetInstance里初始化，需要用的时候再初始化
//*#5.对初始化的过程进行加锁，保证线程安全
//*#6.根据情况添加内嵌垃圾回收类
class Singleton2
{
public:
    static Singleton2* GetInstance()    //?双检查加锁：特点：只要第一次枷锁，后面都不需要加锁，保护线程安全
    {
        if(_sgl == nullptr)
        {
            _mtx.lock();
            if(_sgl == nullptr)
            {
                _sgl = new Singleton2;
            }
            _mtx.unlock();
        }
        return _sgl;
    }

    static void DelInstance()   //?释放单例对象(可加可不加，只要进程是正常结束的 ，都不会存在内存泄露)
    {
        if(_sgl)    //!也可以单层锁
        {
            _mtx.lock();
            if(_sgl)
            {
                delete _sgl;
                _sgl = nullptr;
            }
            _mtx.unlock();
        }
    }

    class CGarbo    //!是一个内嵌的垃圾回收类，根据情况可加可不加
    {
    public:
        ~CGarbo()   //?main函数结束之后才调用析构函数
        {
            DelInstance();
        }
    };

private:
    Singleton2() {};
    Singleton2(const Singleton2& s) = delete;
    Singleton2& operator=(const Singleton2& s) = delete;

    int _a;
    //...
    static Singleton2* _sgl;    //?声明
    static std::mutex _mtx;
    static CGarbo _bin;     //?垃圾回收类的静态对象，释放是在main函数结束之后，调用其析构
};
Singleton2* Singleton2::_sgl = nullptr;     //?定义：刚开始不进行初始化，用的时候在进行初始化
std::mutex Singleton2::_mtx;
Singleton2::CGarbo Singleton2::_bin;




//! Notes:懒汉模式和饿汉模式的对比
//! 饿汉模式
//* 优点：简单
//* 缺点：如果单例对象的构造函数所做的工作比较多，会导致程序启动慢，迟迟进入不了main函数；如果有多个单例对象，他们之间有初始化的依赖关系，饿汉模式不能保证谁先初始化谁后初始化
//! 懒汉模式
//* 优点：解决了饿汉模式的所有缺点
//* 缺点：相对来说比较复杂

//! 对懒汉模式的极致优化
//* 缺点：单例对象再静态区，如果单例对象太大了，不合适； 无法释放
class Singleton3
{
public:
    static Singleton3* GetInstance()
    {
        static Singleton3 sgl;
        return &sgl;
    }
private:
    Singleton3():_a(0) {};
    Singleton3(const Singleton3& s) = delete;
    Singleton3& operator=(const Singleton3& s) = delete;

    int _a;
};



//*-----------------------------------------------------------------------------------------------------------
//*main
int main()
{
    HeapOnly1* a = HeapOnly1::CreateHeapOnly();
    HeapOnly2* e = HeapOnly2::CreateHeapOnly();
    StackOnly1 b = StackOnly1::CreateStackOnly();
    StackOnly2 c;
    //StackOnly2* d = new StackOnly2; //!函数 "StackOnly2::operator new"不可访问
    cout << Singleton1::GetInstance() << endl;
    cout << Singleton1::GetInstance() << endl;
    cout << Singleton1::GetInstance() << endl;
    Singleton1::GetInstance()->Print();

    cout << Singleton2::GetInstance() << endl;
    cout << Singleton2::GetInstance() << endl;
    cout << Singleton2::GetInstance() << endl;
    return 0;
}