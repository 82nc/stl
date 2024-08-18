#ifndef SGI_STL_FUNCTION_H
#define SGI_STL_FUNCTION_H

/*

STL仿函数分类,
若以操作数个数划分,可分为一元和二元仿函数
若以功能划分,可分为算术运算,关系运算,逻辑运算

任何应用程序欲使用STL内建仿函数,都必须含入<functional>
头文件,SGI则将他们定义于<stl_function.h>

//可配接(Adaptable)的关键

STL六大组件中,仿函数体积最小,观念上最简单,实现最容易其扮演一种策略角色
指算法可因为不同的仿函数的介入而有不同的变异行为

它可以让STL算法有更灵活的演出,而更加灵活的关键在于STL仿函数的可配接性

*/

//unary_function
//用来呈现一元函数的参数型别和返回值型别
//STL规定每个Adaptable unary function都应该继承此类别
template <class Arg, class Result>
struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};

//binary_function
//用来呈现二元函数的第一参数型别、第二参数型别、返回值型别
//STL规定每个Adaptable binary function都应该继承此类别
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

//==== 算术运算类仿函数 ======
//加法: plus<T>
//减法: minus<T>
//乘法: multiplies<T>
//除法: divides<T>
//模取: modulus<T>
//否定: negate<T>

template <class T>
struct plus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x+y; }
};

template <class T>
struct minus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x-y; }
};

template <class T>
struct multiplies : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x*y; }
};

template <class T>
struct divides : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x/y; }
};

template <class T>
struct modulus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x%y; }
};

template <class T>
struct negate : public unary_function<T, T> {
    T operator()(const T& x, const T& y) const { return -y; }
};

//证同元素
template <class T>
inline T identity_element(plus<T>) { return T(0); }
//SGI STL并未实际运用这个函数

template <class T>
inline T identity_element(multiplies<T>) { return T(1); }
//乘法的证同元素用于<stl_numerics.h>的power


//==== 关系运算类仿函数 ======
//等于: equal_to<T>
//不等于: not_equal<T>
//大于: greater<T>
//大于或等于: greater_equal<T>
//小于: less<T>
//小于或等于: less_equal<T>

template <class T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x==y; }
};

template <class T>
struct not_equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x!=y; }
};

template <class T>
struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x>y; }
};

template <class T>
struct less : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x<y; }
};

template <class T>
struct greater_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x>=y; }
};

template <class T>
struct less_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x<=y; }
};


//==== 逻辑运算类仿函数 ======
//逻辑或运算: logical_or<T>
//逻辑与运算: logical_and<T>
//逻辑非运算: logical_not<T>
template <class T>
struct logical_and : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x&&y; }
};

template <class T>
struct logical_or : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x||y; }
};

template <class T>
struct logical_not : public binary_function<T, T, bool> {
    bool operator()(const T& x) const { return !y; }
};


//==== 证同(identity)/选择(select)/投射(project)

//证同函数: 任何数值通过此函数后,不会有任何改变
//此式用于<stl_set.h>,用来指定RB-tree所需KeyOfValue op,
//那是因为set元素的键值即实值,所以采用identity
template <class T>
struct identity : public unary_function<T, T> {
    const T& operator()(const T& x) const { return x; }
};

//选择函数: 接受一个pair,传回其第一元素
//此式用于<stl_map.h>,用来指定RB-tree所需KeyOfValue op,
//由于map系以pair元素的第一元素为其键值,所以采用select1st
template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

//选择函数: 接受一个pair,传回其第二元素
//SGI STL未运用此式
template <class Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
    const typename Pair::second_type& operator()(const Pair& x) const { return x.second; }
};

//投射函数: 传回第一参数,忽略第二参数
//SGI STL未运用此式
template <class Arg1, class Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

//投射函数: 传回第二参数,忽略第一参数
//SGI STL未运用此式
template <class Arg1, class Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
    Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};


/*

#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

int mina()
{
    //outite绑定到cout,每次对outite指派一个一元素,然后接一个" "
    ostream_iterator<int> outite(cout, " ");

    int ia[6] = { 2, 21, 12, 7, 19, 23 };
    vector<int> iv(ia, ia+6);

    //欲于每个元素v身上执行(v+2)*3
    //注意for_each()是nomutating algorithm
    for_each(iv.begin(), iv.end(), 
        compose1(bind2nd(multiplies<int>(), 3), bind2nd(plus<int>(), 2)));

    copy(iv.begin(), iv.end(), outite);
    cout << endl; // 2 21 12 7 19 23

    //如果像这样，输出另一地点cout,是可以的
    transform(iv.begin(), iv.end(), outite, 
        compose1(bind2nd(multiplies<int>(), 3), bind2nd(plus<int>(), 2)));
    cout << endl; // 12 69 42 27 63 75
}

//STL组件内任何算法接受一个仿函数时,总是在其演算过程中调用该仿函数的operator()
这使得不具备仿函数之形,却有真正函数之实的"一般函数"和"成员函数"感到为难.为此,
STL又提供了为数众多的配接器,使得"一般函数"和"成员函数"得以在变化纷歧的各种应用
场合完全满足你的要求

请注意任何期望获得配接能力的组件,其本身都必须是可配接的(adaptable)
换句话说,一元仿函数必须继承自unary_funtion,二元函数必须继承自binary_funtion
成员函数必须以mem_fun处理过,一般函数必须以ptr_fun处理

//============ 注意 ==============
这里的mem_fun,ptr_fun, bind2nd
等仿函数适配器自c++11开始均不再推荐使用,自c++17开始已被移除
故这里不再详细讨论

*/


#endif // SGI_STL_FUNCTION_H