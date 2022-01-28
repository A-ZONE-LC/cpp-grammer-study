#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<initializer_list>
#include<algorithm>
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
int main()
{
	//test_Args();
	//test_empBack();
	//test_lambda_add();
	test_lambda_swap();
	return 0;
}