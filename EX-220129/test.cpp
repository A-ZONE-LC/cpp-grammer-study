#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<initializer_list>
#include<algorithm>
#include<functional>
#include<map>
#include<thread>
#include<mutex>
#include<atomic>
using namespace std;
//-------------------------------------------------------------------------------------------
//可变模板参数的测试
void ShowList()
{
	cout << endl;
}

template<class T, class ...Args>
void ShowList(T value, Args... args)  // 打印参数列表
{
	cout << value << ' ';
	return ShowList(args...);
}

template<class ...Args>
void InitVector(Args... args) // 用可变模板参数结合initializer_list给vector初始化
{
	vector<int> v = { args... };
	for (auto& num : v)
	{
		cout << num << ' ';
	}
	cout << endl;
}


template<class T>
void PrintList(T value)
{
	cout << value << ' ';
}
template<class ...Args>
void InitArray(Args... args)
{
	int arr[] = { (PrintList(args),0)... }; // 使用逗号表达式与可变模板参数可以初始化静态数组，传了几个参，数组的大小就是多少
	cout << endl;
	int n = sizeof(arr) / sizeof(int);
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << ' ';
	}
} 


template<class T>
int PrintArg(T value)
{
	cout << value << " ";
	return 0;
}
template<class ...Args>
void ShowListQuote(Args&&... args) // 这里的&&是万能引用，实参传进来的是左值，形参就是左值引用，实参传进来的是右值，形参就是右值引用
{
	int arr[] = { PrintArg(args)... };
	int n = sizeof(arr) / sizeof(int);
	cout << endl;
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

void test_Args()
{
	//ShowList('a', 'b', 'c', 1, 2, 3);
	//InitVector(1, 4, 3, 7, 6);
	//InitArray(1, 2, 3, 4, 5);
	int a = 10;
	ShowListQuote(a, "abcd", 11);
}
//-------------------------------------------------------------------------------------------
//*emplace_back的测试
void test_empBack()
{
	list<pair<int, char>> lt;
	lt.push_back(make_pair(1, 'q'));
	lt.emplace_back(2, 'w'); // emplace_back的真正意义在于参数可以是参数包，直接拿参数包内的参数进行构造，没有了拷贝，效率更高
	lt.push_back({ 3,'e' });
	lt.emplace_back(make_pair(4, 'r'));
	
	for (auto& e : lt)
	{
		cout << e.first << ' ' << e.second << endl;
	}
}

//-------------------------------------------------------------------------------------------
//*lambda表达式
struct Goods
{
	string name;
	double price;
	int num;
	Goods(string name_in, double price_in, int num_in)
		:name(name_in)
		, price(price_in)
		, num(num_in)
	{}
};
void test_lambda()
{
	vector<Goods> v = { {"iphone",9.99,234},{"ipad",11.99,321},{"macbook",18.99,20} };
	v.emplace_back("macbookpro", 20.99, 20);
	sort(v.begin(), v.end(), [](const Goods& g1, const Goods& g2)->bool {return g1.price > g2.price; });
	for (auto& e : v)
	{
		cout << e.name << ":" << "price:" << e.price << " " << "nums:" << e.num << "   ";
	}
	cout << endl;

	sort(v.begin(), v.end(), [](const Goods& g1, const Goods& g2)->bool {return g1.price < g2.price; });
	for (auto& e : v)
	{
		cout << e.name << ":" << "price:" << e.price << " " << "nums:" << e.num << "   ";
	}
	cout << endl;

	sort(v.begin(), v.end(), [](const Goods& g1, const Goods& g2)->bool {return g1.num > g2.num; });
	for (auto& e : v)
	{
		cout << e.name << ":" << "price:" << e.price << " " << "nums:" << e.num << "   ";
	}
	cout << endl;
}

void test_lambda_add()
{
	int x = 10;
	int y = 20;
	auto add1 = [](int& x, int& y)->int {return x + y; };
	cout << add1(x ,y) << endl;

	auto add2 = [&x ,&y] {return x + y; };
	cout << add2() << endl;

	auto add3 = [&] {return x + y; };
	cout << add3() << endl;
}

void test_lambda_swap()
{
	int a = 10;
	int b = 0;
	auto swap1 = [](int& a, int& b)->void {int tmp = a; a = b; b = tmp; };
	swap1(a, b);
	cout << a << ' ' << b << endl;

	auto swap2 = [&a, &b] {int tmp = a; a = b; b = tmp; };
	swap2();
	cout << a << ' ' << b << endl;

	auto swap3 = [&] {int tmp = a; a = b; b = tmp; };
	swap3();
	cout << a << ' ' << b << endl;

	auto swap4 = [=]()mutable {int tmp = a; a = b; b = tmp; }; // 不可以交换
	swap4();
	cout << a << ' ' << b << endl;
}

//------------------------------------------------------------------------------------------------
//*包装器
double func(double i)
{
	return i / 2;
}

struct functions
{
	double operator()(const double i)
	{
		return i / 2;
	}
};


template<class F, class T>
T useF(F func, T value)
{
	static int count = 0;
	cout << "实例化次数: " << ++count << ' ';
	cout << &count << endl;
	return func(value);
}

class Plus
{
public:
	static int plusi(int x, int y)
	{
		return x + y;
	}
	double plusd(double x, double y)
	{
		return x + y;
	}
};

void test_function()
{
	cout << useF(func, 12.99) << endl;   //00ACE3E8   实例化出三份
	cout << useF(functions(), 12.99) << endl;  //0026E3EC
	cout << useF([](double i)->double {return i / 2; }, 12.99);  //0026E70C

	std::function<double(double)> f1 = func;
	std::function<double(double)> f2 = functions();
	std::function<double(double)> f3 = [](double d)->double {return d / 2; };
	std::function<int(int, int)> f4 = Plus::plusi; // 包装静态成员函数
	std::function<double(Plus, double, double)> f5 = &Plus::plusd; //包装成员函数（参数列表里要加上对象的类型，在函数名的类名前要取地址）
	cout << endl;
	cout << useF(f1, 12.99) << endl;  //002E1574  使用包装器以后，useF看到的都是包装器，只会实例化出一份
	cout << useF(f2, 12.99) << endl;  //002E1574
	cout << useF(f3, 12.99) << endl;  //002E1574
	cout << f4(1, 2) << endl;
	cout << f5(Plus(), 2.99, 2.99) << endl;

	//bind绑定固定的可调用对象
	std::function<double(double)> f6 = bind(func, 20);
	cout << f6(1) << endl; //10

	std::function<double(double, double)> f7 = bind(&Plus::plusd, Plus(), placeholders::_1, placeholders::_2);
	cout << f7(1,2) << endl;
}


int sub_func(int x, int y)
{
	return x - y;
}

class sub_struct
{
public:
	int sub(const int x, const int y)
	{
		return x - y;
	}
};
void test_bind()
{
	std::function<int(int, int)> f1 = bind(sub_func, placeholders::_1, placeholders::_2);
	std::function<int(int, int)> f2 = bind(sub_func, placeholders::_2, placeholders::_1); //可调用对象可互换位置
	std::function<int(int)> f3 = bind(sub_func, 10, placeholders::_1); //可固定一个可调用对象
	cout << f1(1, 2) << endl; //1-2
	cout << f2(1, 2) << endl; //2-1
	cout << f3(2) << endl;    //10-2

	//调用类的成员函数时，可以将对象与成员函数的地址绑定到可调用对象中，这样调用的时候就只用传需要操作的两个数
	//注意，参数列表中的&sub_struct::sub 和 sub_struct 的位置不能反
	std::function<int(int, int)> f4 = bind(&sub_struct::sub, sub_struct(), placeholders::_1, placeholders::_2); 
	std::function<int(int, int)> f5 = bind(&sub_struct::sub, sub_struct(), placeholders::_2, placeholders::_1);
	std::function<int(int)> f6 = bind(&sub_struct::sub, sub_struct(), 10, placeholders::_1);
	cout << f4(1, 2) << endl; //1-2
	cout << f5(1, 2) << endl; //2-1
	cout << f6(2) << endl;    //10-2
}

void test_function_clc()
{
	std::map<char, std::function<double(double, double)>> m =
	{
		{'+', [](double x, double y)->double {return x + y; }},
		{'-', [](double x, double y)->double {return x - y; }},
		{'*', [](double x, double y)->double {return x * y; }},
		{'/', [](double x, double y)->double {return x / y; }}
	};

	cout << m['+'](1, 2) << endl;
	cout << m['-'](1, 2) << endl;
	cout << m['*'](1, 2) << endl;
	cout << m['/'](1, 2) << endl;
}

//----------------------------------------------------------------------------------------------------
//*thread
mutex mtx;
void thread_func(int N)
{
	mtx.lock();
	for (int i = 0; i <= N; i++)
	{
		cout << this_thread::get_id() << ':' << i << endl;
	}
	mtx.unlock();
}
void test_thread()
{
	int n;
	cin >> n;
	vector<thread> vth;
	vth.resize(n);
	for (auto& th : vth)
	{
		th = thread(thread_func, 10000);
	}
	for (auto& th : vth)
	{
		th.join();
	}

}


void test_safe()
{
	int n;
	cin >> n;
	vector<thread> vth;	
	vth.resize(n);
	mutex mtx;
	int N = 100;
	int x = 0;
	for (auto& th : vth)
	{
		th = thread([&N, &mtx, &x] {
			for (int i = 1; i <= N; i++) 
			{ 
				mtx.lock();
				cout << this_thread::get_id() << ": " << x << endl;
				++x; 
				this_thread::sleep_for(chrono::milliseconds(300)); //休眠500ms
				mtx.unlock();
			}
			});
	}
	for (auto& th : vth)
	{
		th.join();
	}
	cout << n << "个线程对x++了" << N << "次" << " k:" << x << endl;
}
void test_atomic()
{
	int n;
	cin >> n;
	vector<thread> vth;
	mutex mtx;
	int N = 100;
	atomic<int> x = 0; // x是原子的
	vth.resize(n);
	for (auto& th : vth)
	{
		th = thread([&N, &mtx, &x] {
			for (int i = 1; i <= N; i++) 
			{ 
				++x; 
			}
			}); 
	}
	for (auto& th : vth)
	{
		th.join();
	}
	cout << n << "个线程对x++了" << N << "次" << " x:" << x << endl;
}

//------------------------------------------------------------------------------------------
//* lock_guard

namespace cga
{
	template<class Lock>
	class lock_guard
	{
	public:
		lock_guard(Lock& lock)
			:_lock(lock)
		{
			_lock.lock();
			cout << "locked" << endl;
		}

		~lock_guard()
		{
			_lock.unlock();
			cout << "unlocked" << endl;
		}
	private:
		Lock& _lock;
	};


	template<class Lock>
	class unique_lock
	{
	public:
		unique_lock(Lock& lock)
			:_lock(lock)
		{
			_lock.lock();
			cout << "locked" << endl;
		}

		~unique_lock()
		{
			_lock.unlock();
			cout << "unlocked" << endl;
		}

		void lock()
		{
			_lock.lock();
		}

		void unlock()
		{
			_lock.unlock();
		}
	private:
		Lock& _lock;
	};
}


void test_unique_lock()
{
	mutex mtx;
	cga::unique_lock<mutex> ulock(mtx); // locked
										// unlocked
}
int main()
{
	//test_Args();
	//test_empBack();
	//test_lambda_add();
	//test_lambda_swap();
	//test_function();
	//test_bind();
	//test_thread();
	//test_atomic();
	//test_safe();
	test_unique_lock();
	return 0;
}