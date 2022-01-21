/*
 * @Author: your name
 * @Date: 2022-01-21 19:56:30
 * @LastEditTime: 2022-01-21 23:18:36
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \EX-220121\test.cpp
 */
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <cstring>
#include <algorithm>
#include <list>
using namespace std;

//*-------------------------------------------------------------------------------------------------------------
//todo 左值引用与右值引用

namespace cga
{
	class string //* 自主实现的string类
	{
	public:
		typedef char *iterator;
		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		//* 构造函数
		string(const char *str = "")
			: _size(strlen(str)), _capacity(_size)
		{
			cout << "string constructor" << endl;
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}

		// s1.swap(s2)
		//* 交换资源
		void swap(string &s)
		{
			::swap(_str, s._str);
			::swap(_size, s._size);
			::swap(_capacity, s._capacity);
		}

		//* 拷贝构造
		string(const string &s)
			: _str(nullptr)
		{
			cout << "string(const string& s) -> 拷贝构造深拷贝" << endl;
			string tmp(s._str);
			swap(tmp);
		}

		//* 移动构造
		string(string &&s)
			: _str(nullptr), _size(0), _capacity(0)
		{
			cout << "string(string&& s) -> 移动构造交换资源" << endl;
			this->swap(s);
		}

		//* 移动赋值
		string &operator=(string &&s)
		{
			cout << "string& operator=(string&& s) -> 移动赋值交换资源" << endl;
			this->swap(s);
			return *this;
		}

		//* 赋值重载
		string &operator=(const string &s)
		{
			cout << "string& operator=(string s) -> 赋值重载深拷贝" << endl;
			string tmp(s);
			swap(tmp);
			return *this;
		}

		//* 析构函数
		~string()
		{
			delete[] _str;
			_str = nullptr;
		}

		//* 下标寻址重载
		char &operator[](size_t pos)
		{
			assert(pos < _size);
			return _str[pos];
		}

		//* 调整容量
		void reserve(size_t n)
		{
			if (n > _capacity)
			{
				char *tmp = new char[n + 1];
				strcpy(tmp, _str);
				delete[] _str;
				_str = tmp;
				_capacity = n;
			}
		}

		void push_back(char ch)
		{
			if (_size >= _capacity)
			{
				size_t newcapacity = _capacity == 0 ? 4 : _capacity * 2;
				reserve(newcapacity);
			}

			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
		}

		//string operator+=(char ch)
		//* 追加
		string &operator+=(char ch)
		{
			push_back(ch);
			return *this;
		}

		//* 返回字符串
		const char *c_str() const
		{
			return _str;
		}

	private:
		char *_str;
		size_t _size;
		size_t _capacity; // 不包含最后做标识的\0
	};

	//* int to string
	cga::string to_string(int value)
	{
		bool flag = true;
		if (value < 0)
		{
			flag = false;
			value = 0 - value;
		}

		cga::string str;
		while (value > 0)
		{
			int x = value % 10;
			value /= 10;

			str += ('0' + x);
		}

		if (flag == false)
		{
			str += '-';
		}

		std::reverse(str.begin(), str.end());
		return str;
		//! str 是一个将亡值,编译器处理成了右值
		//! str不能使用引用返回，没有移动构造且编译器没有优化时，str会拷贝构造一个临时对象，然后再用临时对象拷贝构造返回值
		//! 没有移动构造且编译器有优化时，str直接拷贝构造返回值
		//! 有移动构造且编译器没有优化时，str会移动构造转移资源给一个临时对象，然后临时对象再移动构造返回值
		//! 有移动构造且编译器有优化时， str会直接移动构造返回值
	}
}

//* 左值引用的使用场景： 1.做函数参数 2.做函数返回值 目的是为了减少拷贝
//? 之所以可以使用左值引用作返回值，那是因为函数调用结束之后对应的变量或者对象还存在
//? 若对象是在函数内创建的，函数调用结束之后对象也就销毁了，这个时候是不能使用左值引用返回的

//todo 右值引用返回是为了解决上述的问题

void test1()
{
	//cga::string ret = cga::to_string(12315); //* string(string&& s) -> 移动构造交换资源

	cga::string ret2;
	ret2 = cga::to_string(12345); //* string(string&& s) -> 移动构造交换资源
								  //* string& operator=(string&& s) -> 移动赋值交换资源

	//cga::string s1("hello");
	//cga::string s2 = s1; //* string(const string& s) -> 拷贝构造深拷贝
	//cga::string s3 = std::move(s1); //* string(string&& s) -> 移动构造交换资源
	//! move之后s1就被悬空了
}

void test2()
{
	list<std::string> lt;
	std::string s("1111");
	lt.push_back(s); //* 拷贝构造

	//* 都是移动构造
	lt.push_back("1111");
	lt.push_back(std::string("1111"));
	lt.push_back(std::move(s));
}

//*-------------------------------------------------------------------------------------------------------------

//todo 完美转发
//? 模板中的&&不代表右值引用，而是万能引用，其既能接收左值又能接收右值。
//? 模板的万能引用只是提供了能够接收同时接收左值引用和右值引用的能力，
//? 但是引用类型的唯一作用就是限制了接收的类型，后续使用中都退化成了左值，
//? 我们希望能够在传递过程中保持它的左值或者右值的属性, 就需要用完美转发

void Fun(int &x) { cout << "左值引用" << endl; }
void Fun(const int &x) { cout << "const 左值引用" << endl; }
void Fun(int &&x) { cout << "右值引用" << endl; }
void Fun(const int &&x) { cout << "const 右值引用" << endl; }
template <typename T>
void NoPerfectForward(T &&t)
{
	Fun(t);
}
template <typename T>
void PerfectForward(T &&t)
{
	Fun(std::forward<T>(t));
}
void test3()
{
	NoPerfectForward(10); //?右值
	int a = 10;
	NoPerfectForward(a); //?左值
	const int b = 10;
	NoPerfectForward(b);			//?const 左值
	NoPerfectForward(std::move(b)); //?const 右值
	//* 左值引用
	//* 左值引用
	//* const 左值引用
	//* const 左值引用
	cout << endl;

	PerfectForward(10); //?右值
	PerfectForward(a);	//?左值
	const int c = 10;
	PerfectForward(c);			  //? const 左值
	PerfectForward(std::move(c)); //? const 右值
								  //* 右值引用
								  //* 左值引用
								  //* const 左值引用
								  //* const 右值引用
}

namespace cga
{
	template <class T>
	struct ListNode
	{
		ListNode *_next = nullptr;
		ListNode *_prev = nullptr;
		T _data;
	};

	template <class T>
	class List
	{
		typedef ListNode<T> Node;

	public:
		List()
		{
			//_head = new Node;
			_head = (Node *)malloc(sizeof(Node));
			_head->_next = _head;
			_head->_prev = _head;
		}

		void PushBack(const T &x)
		{
			//Insert(_head, x);
			Insert(_head, x);
		}

		void PushBack(T &&x)
		{
			//! cout << &x << endl; 右值引用可以取地址，退化为左值
			//! 这里x属性退化为左值，其他对象再来引用x，x会识别为左值

			//Insert(_head, x);
			//! 这里就要用完美转发，让x保持他的右值引属性
			Insert(_head, std::forward<T>(x));
		}

		void PushFront(T &&x)
		{
			//Insert(_head->_next, x);
			Insert(_head->_next, std::forward<T>(x));
		}

		void Insert(Node *pos, T &&x)
		{
			Node *prev = pos->_prev;
			//Node* newnode = new Node;
			//newnode->_data = std::forward<T>(x); // 关键位置
			Node *newnode = (Node *)malloc(sizeof(Node));
			new (&newnode->_data) T(std::forward<T>(x)); //? 定位new

			// prev newnode pos
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = pos;
			pos->_prev = newnode;
		}

		void Insert(Node *pos, const T &x)
		{
			Node *prev = pos->_prev;
			//Node* newnode = new Node;
			//newnode->_data = x; // 关键位置
			Node *newnode = (Node *)malloc(sizeof(Node));
			new (&newnode->_data) T(x);

			// prev newnode pos
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = pos;
			pos->_prev = newnode;
		}

	private:
		Node *_head;
	};
}


//*--------------------------------------------------------------------------------------------------------------
//todo 新增的两个默认函数：移动构造与移动赋值函数

//? 默认生成移动构造和移动赋值的规则：只有当自己没有写移动构造，且都没有写拷贝构造、赋值重载、析构时会自动生成。
//? 默认的移动构造和移动赋值对内置类型会逐字节的进行拷贝，对于自定义类型回去调用他们自己的移动构造或移动赋值。


//*--------------------------------------------------------------------------------------------------------------
//todo 可变模板参数的提取
void ShowList()
{
	cout << endl;
}
template<class T, class ...Args>
void ShowList(T value, Args ...args)
{
	cout << value << ' ';
	ShowList(args...);
}
void test4()
{
	ShowList(1, 'A', "hello", 1.47);
	ShowList(58, 8.77, "yhh");
}


//*--------------------------------------------------------------------------------------------------------------
//todo lambda表达式
struct Goods
{
	string _name;
	double _price;
	int _nums;
};

struct ComparePriceLess
{
	bool operator()(const Goods& gl, const Goods& gr)
	{
		return gl._price < gr._price;
	}
};
struct ComparePriceGreater
{
	bool operator()(const Goods& gl, const Goods& gr)
	{
		return gl._price > gr._price;
	}
};

void test5()
{
	vector<Goods> v = { { "苹果", 2.1, 300 }, { "香蕉", 3.3, 100 }, { "橙子", 2.2, 1000 }, { "菠萝", 1.5, 1 } };
	//* 要求分别按名字、价格、数量进行排序，升序或降序
	//? 三种办法：1.仿函数 2.函数指针 3.lambda表达式
	//std::sort(v.begin(), v.end(), ComparePriceLess());
	//std::sort(v.begin(), v.end(), ComparePriceGreater());

	//* lambda表达式
	//* []         ()        ->             {}
	//* 捕捉列表   函数参数      函数返回值   函数体
	std::sort(v.begin(), v.end(), [](const Goods& gl, const Goods& gr)->bool{return gl._price < gr._price;});
	std::sort(v.begin(), v.end(), [](const Goods& gl, const Goods& gr)->bool{return gl._price > gr._price;});
}

int main()
{
	test5();
	return 0;
}