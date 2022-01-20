<!--
 * @Author: your name
 * @Date: 2022-01-19 07:07:39
 * @LastEditTime: 2022-01-19 07:07:41
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \EX-220119\note.md
-->
## C++11的新特性

---

###  一、{ }初始化列表

在C++98中，{ }初始化初始值被用在结构体与数组的初始值设定，但是在C++11中，这种方式可以用于 ==所有的内置类型与自定义类型==，使用初始化列表时，可以添加=，也可以不添加。
```c++ { .line-numbers}
    int x{1};

    int arr[]{1, 2, 3, 4, 5};

    int* p = new int[5]{1, 2, 3, 4, 5};
    int* q = new int{1};

    Date d1{2022, 2, 22};
    Date d2 = {2021, 1, 21};
```
---

### 二、initializer_list

实际上initiallizer_list在代码中不会以全名的形式出现，大多都是表现为 { }，实际上，initiallizer_list就是提供了一个列表给一个容器做初始化，例如 {1, 2, 3, 4, 5} 就是一个initiallizer_list，模板参数为int,这个列表中的数据会去初始化对应的容器。
``` C++ { .line-numbers}
    auto a = {1, 2, 3};
    cout << typeid(a).name() << endl; //St16initializer_listIiE
    std::initializer_list<int> il{1, 2, 3, 4, 5};
    cout << typeid(il).name() << endl; //St16initializer_listIiE

    vector<int> v = {1, 3, 4};
    list<int> l = {1, 3, 4};
    for(auto& e: v)
        cout << e << ' '; // 1 3 4
    cout << endl;
    for(auto& e: l)
        cout << e << ' '; // 1 3 4
    cout << endl;
```

---

### 三、decltype关键字
decltype关键字把类型声明为表达式指定的类型。

```c++ { .line-numbers}
    int x;
    double y;
    decltype(x*y) z;
    cout << typeid(z).name() << endl; // d

    decltype(z) a;
    cout << typeid(a).name() << endl; // d
```

---

### 四、array
array是STL中的静态数组，相对于C语言中的静态数组来说，它增加了迭代器，并且它的边界检查更为严格，一旦越界就会报错。

```c++ { .line-numbers}
    array<int, 5> a{1, 2, 3, 4, 5};
    a.at(5); //*terminate called after throwing an instance of 'std::out_of_range'
    //*what():  array::at: __n (which is 5) >= _Nm (which is 5)
```