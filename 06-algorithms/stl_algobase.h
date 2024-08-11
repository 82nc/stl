#ifndef SGI_STL_ALGO_BASE_H
#define SGI_STL_ALGO_BASE_H

/*
 * STL标准规格中并没有区分基本算法或者复杂算法,然后SGI却把常用的一些
 * 算法定义于<stl_algobase.h>之中,其他算法定义于<stl_algo.h>中.
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <cstring>
#include <vector>
#include <string>


/* =======================================================
                        算法总览
 * equal, fill, fill_n, iter_swap, lexicographical_compare, 
 * max, min, mismatch, swap, copy, copy_backward
 * ======================================================= */

using namespace std;

template <class T>
struct display {
    void operator()(const T& x) const { cout << x << ' '; }
};

int main()
{
    int ia[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    vector<int> iv1(ia, ia+5);
    vector<int> iv2(ia, ia+9);

    // { 0, 1, 2, 3, 4 } vs { 0, 1, 2, 3, 4, 5, 6, 7, 8 }
    cout << *(mismatch(iv1.begin(), iv1.end(), iv2.begin()).first); // ?
    cout << *(mismatch(iv1.begin(), iv1.end(), iv2.begin()).second); // 5
    //以上判断两个区间的第一个不匹配点,返回一个有两个迭代器组成的pair,其中第一个迭代器
    //指向第一区间的不匹配点,第二个迭代器指向第二区间的不匹配点
    //上述写法很危险,应该先判断迭代器是否等于容器的end(),然后才可以输出操作

    //如果两个序列在[first,last)区间内相等,equal()返回true,如果第二序列
    //的元素比较多,多出来的不予考虑.
    cout << equal(iv1.begin(), iv1.end(), iv2.begin()); // 1，true

    cout << equal(iv1.begin(), iv1.end(), &ia[3]); // 0，true
    // { 0, 1, 2, 3, 4 } 不等于 { 3, 4, 5, 6, 7 }
    cout << equal(iv1.begin(), iv1.end(), &ia[3], less<int>()); // 1
    // { 0, 1, 2, 3, 4 } 小于 { 3, 4, 5, 6, 7 }

    fill(iv1.begin(), iv1.end(), 9);
    for_each(iv1.begin(), iv1.end(), display<int>()); // 9 9 9 9 9

    fill_n(iv1.begin(), 3, 7);
    for_each(iv1.begin(), iv1.end(), display<int>()); // 7 7 7 9 9

    vector<int>::iterator i1 = iv1.begin(); // 指向7
    vector<int>::iterator i2 = i1;
    advance(i2, 3); //指向9

    iter_swap(i1, i2); //将两个迭代器所指元素对调
    cout << *i1 << ' ' << *i2 << endl; // 9 7
    for_each(iv1.begin(), iv1.end(), display<int>()); // 9 7 7 7 9

    //以下取两者之大值
    cout << max(*i1, *i2) << endl; // 9
    //以下取两者之小值
    cout << min(*i1, *i2) << endl; // 7

    //此刻状态: iv1 { 9, 7, 7, 7, 9 }, iv2 { 0, 1, 2, 3, 4, 5, 6, 7, 8 }
    swap(*iv1.begin(), *iv2.begin()); //将两值对调
    for_each(iv1.begin(), iv1.end(), display<int>()); // 0 7 7 7 9
    for_each(iv2.begin(), iv2.end(), display<int>()); // 9 1 2 3 4 5 6 7 8

    string stra1[] = { "Jamie", "JJHou", "Jason" };
    string stra2[] = { "Jamie", "JJHou", "Jerry" };

    cout << lexicographical_compare(stra1, stra1+2, stra2, stra2+2);
    // 1 (stra1 < stra2)
    cout << lexicographical_compare(stra1, stra1+2, stra2, stra2+2, greater<string>());
    // 0 (stra1 <= stra2)
}


/* =======================================================
 * equal
 * 如果两个序列在[first,last)区间相等,equal返回true,如果第二个序列元素多,则多出来的不予考虑
 * ======================================================= */

//version 1
template <class InputIterator, class InputIterator2>
inline bool equal(InputIterator first, InputIterator last1, InputIterator2 first2)
{
    //以下将序列一走一遍,序列二亦步亦趋
    for (; first1 != last1; ++first1, ++first2)
        if (*first1 != *first2) //对应元素不相等
            return false;
    return true;
}

//version 2
template <class InputIterator, class InputIterator2, class BinaryOperation>
inline bool equal(InputIterator first, InputIterator last1, 
InputIterator2 first2, BinaryOperation binary_pred)
{
    //以下将序列一走一遍,序列二亦步亦趋
    for (; first1 != last1; ++first1, ++first2)
        if (binary_pred(*first1, *first2))
            return false;
    return true;
}


/* =======================================================
 * fill
 * 将区间[first,last)内所有元素改填新值
 * ======================================================= */

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first) //迭代走过整个区间
        *first = value; //设定新值
}


/* =======================================================
 * fill_n
 * 将区间[first,last)内前n个元素改填新值,返回迭代器指向被填入的最后一个元素的下一位置
 * ======================================================= */
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
    for (; n>0; --n, ++first)
        *first = value;
    return first;
}
//每次迭代都是一次assignment(覆写操作overwrite),所以一旦操作区间超越了容器大小
//就会造成不可预期的结果,解决办法:可以用inserter()产生一个具有插入而非覆盖功能的
//迭代器. inserter可以产生一个用来修饰迭代器的配接器(iterator adapter)
/*
int ia[3] = { 0, 1, 2 };
vector<int> iv(ia, ia+3); // 0 1 2
fill_n(inserter(iv, iv+begin()), 5, 7); // 7 7 7 7 7 0 1 2
 */ 


/* =======================================================
 * iter_swap
 * 将两个ForwardIterator所指对象对调
 * ======================================================= */

template <class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
    __iter_swap(a, b, value_type(a)); //注意第三个参数的类型
}

template <class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
该函数必须知道迭代器的value_type才能据此定义一个对象,用来暂存迭代器所指对象
为此上述代码设计了一个双层构造,第一层调用第二层,并多出一个额外的参数value_type(a)

// 以下定义于<stl_iterator.h>
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}
这种写法在SGI STL源代码内很常见,其实这并非必要,可以直接这么写就行:
tempalte <class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
    typename iterator_trait<ForwardIterator1>::value_type tmp = *a;
    *a = *b;
    *b = tmp;
}
 */


/* =======================================================
 * lexicographical_compare
 * 以字典序将两个序列[first1,last1)和[first2,last2)进行比较
 * ======================================================= */

//version 1
template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2)
{
    //任何一个序列到达尾端就结束,否则两序列相应元素一一比较
    for (; first1 < last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
        //如果以上都不满足,表示相等,那就进行下一组元素比对
    }
    //进行到这里,如果到达第一序列尾端,而第二序列尚有余额,那么第一序列小于第二序列
    return first1 == last1 && first2 ! = last2;
}

//version 2
template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2, Compare comp)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2)) return true;
        if (comp(*first2, *first1)) return false;
    }
    return first1 == last1 && first2 != last2;
}

//version 3: 针对原生指针cosnt unsigned char*的特化版本
inline bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1, 
const unsigned char* first2, const unsigned char* last2)
{
    const size_t len1 = last1 - first1; //第一序列长度
    const size_t len2 = last2 - first2; //第二序列长度
    //先比较相同长度的一段,memcmp速度极快
    const int result = memcmp(first1, first2, min(len1, len2));
    //如果不相上下,则长度较长者被视为比较大
    return result != 0 ? result < 0 : len1 < len2;
}


/* =======================================================
 * min
 * 取两个对象中较小值
 * ======================================================= */

//version 1
template <class T>
inline const T& min(const T& a, const T& b)
{
    return b < a ? b : a;
}

//version 2
template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp)
{
    return comp(b, a) ? b : a;
}


/* =======================================================
 * mismatch
 * 用来比较两个序列,指出两者之间的第一个不匹配点,
 * 返回一个迭代器,分别指向两序列中不匹配点位置
 * ======================================================= */

//version 1
template <class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, 
InputIterator1 last1, InputIterator2 first2)
{
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

//version 2
template <class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, 
InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
{
    while (first1 != last1 && binary_pred(*first1, *first2)) {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}


/* =======================================================
 * swap
 * 交换/对调两个对象的内容
 * ======================================================= */

template <class T>
inline void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}


/* =======================================================
 * copy
 * 复制行为很重要,主要是运用assignment operator或copy constructor(copy算法用的是前者)
 * 某些元素类型是trivial assignment,因此,如果能够直接使用内存复制行为(如memcpy或memmove)
 * 那可以节省大量时间.因此SGI STL的copy算法用尽各种办法: 函数重载(function overloading),
 * 类型特性(type traits),偏特化(partial specialization)等编程技巧,无所不用其极地加强效率
 * 注意因内部使用memcpy和memmove的可能性因情况而定,所以当输出空间的起始处位于输入区间内,
 * 可能会因为造成错误. 因memmove会将输入空间的内容复制下来,没有被覆盖的风险.
 * ======================================================= */

/*
#include <iostream>
#include <algorithm>
#include <deque> // deque拥有RandomAccessIterator

using namespace std;

tempalte <class T>
struct display {
    void operator()(cosnt T& x) { cout << x << ' '; }
};

int main()
{
    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        //以下输出区间的终点与输入区间重叠,没问题
        copy(ia+2, ia+7, ia);
        for_each(ia, ia+9, display<int>()); // 2 3 4 5 6 5 6 7 8
        cout << endl;
    }

    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        //以下输出区间的起点与输入区间重叠,可能会有问题
        copy(ia+2, ia+7, ia+4);
        for_each(ia, ia+9, display<int>()); // 0 1 2 3 2 3 4 5 6
        cout << endl;
        //本例正确,因为调用copy算法内部使用了memmove()执行实际复制操作
    }

    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        deque<int> id(ia, ia+9);

        deque<int>::iterator first = id.begin();
        deque<int>::iterator last = id.end();
        ++++first; //advance(first, 2);
        cout << *first << endl; // 2
        ----last; //advance(last, -2);
        cout << *last << endl; // 7

        deque<int>::iterator result = id.begin();
        cout << *result << endl;

        //以下输出区间的终点与输入区间重叠,没问题
        copy(first, last, result);
        for_each(id.begin(), id.end(), display<int>()); // 2 3 4 5 6 5 6 7 8
        cout << endl;
    }
    
    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        deque<int> id(ia, ia+9);

        deque<int>::iterator first = id.begin();
        deque<int>::iterator last = id.end();
        ++++first; //advance(first, 2);
        cout << *first << endl; // 2
        ----last; //advance(last, -2);
        cout << *last << endl; // 7

        deque<int>::iterator result = id.begin();
        advance(result, 4);
        cout << *result << endl; // 4

        //以下输出区间的起点与输入区间重叠,可能会有问题
        copy(first, last, result);
        for_each(id.begin(), id.end(), display<int>()); // 0 1 2 3 2 3 2 3 2
        cout << endl;
        //本例结果错误,因为调用的copy算法不再使用memmove执行实际的复制操作

        如果你以vector替代上面的queue进行测试,则结果将是正确的,因为vector的迭代器
        是原生指针,所以copy内部调用memmove执行复制操作.
    }
}
 */

//verison: 唯一对外接口(完全泛化版本)
template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//特化版本 1: 重载形式
inline char* copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

//特化版本 2： 重载形式
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof(wchar_t)*(last - first));
    return result + (last - first);
}

//完全泛化版本
template <class InputIterator, class OutputIterator>
struct __copy_dispatch
{
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
    {
        return __copy(first, last, result, iterator_category(first));
    }
};

//__copy特化版本: InputIterator
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, 
OutputIterator result, input_iterator_tag)
{
    //以迭代器等同与否,决定循环是否继续,速度慢
    for (; first != last; ++result, ++first)
        *result = *first; // assignment operator
    return result;
}

//__copy特化版本: RandomAccessIterator
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
OutputIterator result, random_access_iterator_tag)
{
    //又划分出一个函数,为的是其它地方也可能用到
    return __copy_d(first, last, result, distance_type(first));
}

template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, 
OutputIterator result, Distance*)
{
    //以n决定循环执行次数,速度快
    for (Distance n= last - first; n > 0; --n, ++result, ++first)
        *result = *first; // assignment operator
    return result;
}

//偏特化版本1
template <class T>
struct __copy_dispatch <T*, T*>
{
    T* operator()(T* first, T* last, T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

//偏特化版本2
template <class T>
struct __copy_dispatch <const T*, T*>
{
    T* operator()(const T* first, T* last, T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

//适用指针所指对象具备trivial assignment operator
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
    memmove(result, first, sizeof(T)* (last - first));
    return result + (last - first);
}

//适用指针所指对象具备non-trivial assignment operator
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
    //原生指针也是一种RandomAccessIterator,所以交给__copy_d()完成
    return __copy_d(first, last, result, (ptrdiff_t*)0);
}

/* 
#include <iostream> //for cout
#include <algorithm> //for copy
#include <vector> 
#include <deque> 
#include <list> 
#include "6string.h" //class String by J.J hou 

using namespace std;

class C {
public:
    C() : _data(3) {}
    //this is a trivial assignment operator
private:
    int _data;
};

int main()
{
    //测试1
    const char ccs[5] = { 'a', 'b', 'c', 'd', 'e' }; //数据来源
    const char ccd[5]; //数据去处

    copy(ccs, ccs+5, ccd); 
    //调用: copy(const char*)

    //测试2
    const wchar_t cwcs[5] = { 'a', 'b', 'c', 'd', 'e' }; //数据来源
    wchar_t cwcd[5];
    copy(cwcs, cwcs+5, cwcd); 
    //调用: copy(const wchar_t*)

    //测试3
    int ia[5] = { 0, 1, 2, 3, 4, 5 };
    copy(ia, ia+5, ia); //数据来源和数据去处相同,这是允许的
    //调用版本: copy() -> __copy_dispatch() -> __copy_t(__true_type)

    //测试4
    list<int> ilsts(ia, ia+5); //list iterator归类为InputIterator
    list<int> ilstd(5);
    copy(ilsts.begin(), ilstd.end(), ilstd.begin());
    //调用: copy() -> __copy_dispatch() -> __copy(input_iterator)

    //测试5
    vector<int> ivecs(ia, ia+5); //vector iterator归类为native pointer
    //constructor会输出信息
    vector<int> ivecs(5);
    copy(ivecs.begin(), ivecs.end(), ivecs.begin());
    //调用: copy() -> __copy_dispatch(T*, T*) -> __copy_t(__true_type)

    //测试6
    C c[5];
    vector<C> Cvs(c, c+5);  //数据来源
    //constructor会输出信息
    vector<C> Cvd(5);       //数据去处
    copy(Cvs.begin(), Cvs.end(), Cvd.begin());
    //调用: copy() -> __copy_dispatch(T*, T*) -> __copy_t(__false_type) -> copy_d()
    //疑问: 不应该是random_access_iterator?, 不应该是__true_type？

    //测试7
    deque<C> Cds(c, c+5); //deque iterator归类为random_access iterator
    deque<C> Cdd(5);
    copy(Cds.begin(), Cds.end(), Cdd.begin());
    //调用: copy() -> __copy_dispatch() -> __copy(random_access_iterator) -> copy_d()

    //测试8
    //class String定义于"6string.h"内,拥有non-trivial operator=
    vector <String>
    vector<String> strvs(5);
    vector<String> strvd(5);
    strvs[0] = "jjhou";
    strvs[1] = "grace";
    strvs[2] = "david";
    strvs[3] = "jason";
    strvs[4] = "jerry";

    copy(strvs.begin(), strvs.end(), strvd.begin());
    //调用: copy() -> __copy_dispatch(T*, T*) -> __copy_t(__false_type) -> __copy_d()
    //疑问: 不应该是random_access_type？

    //测试9
    deque<String> strds(5);
    deque<String> strdd(5);
    strds.push_back("jjhou");
    strds.push_back("grace");
    strds.push_back("david");
    strds.push_back("jason");
    strds.push_back("jerry");

    copy(strds.begin(), strds.end(), strdd.begin());
    //调用: copy() -> __copy_dispatch() -> __copy(random_access_iterator) -> __copy_d()
}

疑问:

1. 测试5、6一开始的constructor为什么会制造输出信息？
2. 测试5、6、8完调用copy后为什么不走random_access_iterator的方向,而是T*方向
3. 测试6具备trivial operator=,为何却走__false_type的方向?

解答:
1. 前2个问题的答案一样
vector<int> ivecs(ia, ia+5);
//以下是输出信息
//copy()
//  __copy_dispatch(T*, T*)
//    __copy_t(__true_type)

//构造一个vector却产生上述三行输出,追踪vector ctor发现
//vector<T>::vector(first, last)
//-> vector<T>::range_initialize(first, last, forward_iterator_tag)
// -> vector<T>::allocate_and_copy(n, first, last)
//  -> ::uninitialized_copy(first, last, result)
//   -> ::__uninitialized_copy(first, last, result, value_type(result))
//    -> ::__uninitialized_copy_aux(first, last, result, is_POD())
//     -> ::copy(first, last, result)

3. 我们以为vector的迭代器是random_access_iterator,实际上它是个T*
实际上翻看前面vector源码时,可以看到期迭代器确实是T*

4. 既然class C具备trivialoperator=,为什么他仍被判定为一个__false_type?
这是因为编译器之中,有能力验证"用户自定义类型"之类型的极少(Silicon Graphics N32或N64编译器就可以)
<type_traits.h>内只针对C++标量类型(scalar type)做了类型特性记录(见3.7).因此所有用户定义的类型
都被编译器视为拥有non-trivial ctor/dtor/operator=.如果确知某个类具备trivial ctor/dtor/operator=
我们自己手动为它添加特性设定,才能保证编译器知道它的身份

需要借助<type_traits.h>(3.7节)中的__type_traits<T>实现

//当编译器无法判别class C的特性(traits),我们来自己设定
//当编译器支持偏特化时
template <> struct __type_trait<C> {
    typedef __false_type has_trivial_default_constructor;
    typedef __true_type  has_trivial_copy_constructor;
    typedef __true_type  has_trivial_assignment_operator;
    typedef __true_type  has_trivial_destructor;
    typedef __false_type is_POD_type;
    //以上每个定义都必须完成
};

//加上上面的设定后,测试6的操作输出信息为
//copy()
// __copy_dispatch(T*, T*) //合理: vector的迭代器是原生指针
//  __copy_t(__true_type) //编译器知道class c是__true_type

 */


/* ===========================================================
 * copy_backward
 * 将[first,last)区间内的每一个元素,
 * 以逆行方向复制到以result-1为起点,方向亦逆行的区间上
 * =========================================================== */
template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
BidirectionalIterator1 last, BidirectionalIterator2 result)
{
    //实现技巧与copy()十分类似,源代码就不列了
    //....
}

/*
#include <iostream>
#include <algorithm>
#include <deque> // deque拥有RandomAccessIterator

using namespace std;

tempalte <class T>
struct display {
    void operator()(cosnt T& x) { cout << x << ' '; }
};

int main()
{
    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        //以下输出区间的终点与输入区间重叠,没问题
        copy_backward(ia+2, ia+7, ia+9);
        for_each(ia, ia+9, display<int>()); // 0 1 2 3 2 3 4 5 6 
        cout << endl;
    }

    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        //以下输出区间的起点与输入区间重叠,可能会有问题
        copy_backward(ia+2, ia+7, ia+5);
        for_each(ia, ia+9, display<int>()); // 2 3 4 5 6 5 6 7 8
        cout << endl;
        //本例正确,因为调用copy算法内部使用了memmove()执行实际复制操作
    }

    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        deque<int> id(ia, ia+9);

        deque<int>::iterator first = id.begin();
        deque<int>::iterator last = id.end();
        ++++first; //advance(first, 2);
        cout << *first << endl; // 2
        ----last; //advance(last, -2);
        cout << *last << endl; // 7

        deque<int>::iterator result = id.end();
        cout << *result << endl;

        //以下输出区间的终点与输入区间重叠,没问题
        copy_backward(first, last, result);
        for_each(id.begin(), id.end(), display<int>()); // 0 1 2 3 4 5 6 5 6
        cout << endl;
    }
    
    {
        int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        deque<int> id(ia, ia+9);

        deque<int>::iterator first = id.begin();
        deque<int>::iterator last = id.end();
        ++++first; //advance(first, 2);
        cout << *first << endl; // 2
        ----last; //advance(last, -2);
        cout << *last << endl; // 7

        deque<int>::iterator result = id.begin();
        advance(result, 5);
        cout << *result << endl; // 5

        //以下输出区间的起点与输入区间重叠,可能会有问题
        copy(first, last, result);
        for_each(id.begin(), id.end(), display<int>()); // 6 5 6 5 6 5 6 7 8
        cout << endl;
        //本例结果错误,因为调用的copy算法不再使用memmove执行实际的复制操作
    }
}
 */


/* ===========================================================
 * set union/intersection/difference/symmetric-difference
 * 以上4个算法接受的set必须是有序空间,元素可以重复,即可以接受set/multiset,
 * hash_set/hash_multiset则不能使用
 * =========================================================== */

////========== set_union ============
//构造S1, S2的并集
//stable算法

//version 1: operator <
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
InputIterator1 first2, InputIterator1 last2, OutputIterator result)
{
    //当两个区间都尚未到达尾端时,执行以下操作
    while (first1 != last1 && first2 != last2) {
        //在两区间内分别移动迭代器,首先将元素较小者(假设为A区)记录在目标区,
        //然后移动A区迭代器使之前进;同时另一个区迭代器不动.然后进行新一次的
        //比大小、记录小值、迭代器移动...直到两区中有一区到达尾端.
        //如果元素相等,取S1者记录于目标区,并同时移动两个迭代器
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        } else if (*first2 < *first1) {
            *result = *first1;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    //一下尚未到达尾端的区间的所有剩余元素拷贝到目的端
    //此刻[first1,last1)和[first2,last2)之中有一个是空的
    return copy(first2, last2, copy(first1, last1, result));
}

//version 2: 仿函数comp比较
//略

//========== set_intersection ============
//构造S1, S2的交集
//stable算法

//version 1: operator <
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
InputIterator1 first2, InputIterator1 last2, OutputIterator result)
{
    //当两个区间都尚未到达尾端时,执行以下操作
    while (first1 != last1 && first2 != last2) {
        //在两区间内分别移动迭代器,直到遇到相同元素,暂停,将该值记录于目标区,
        //再继续移动迭代器...直到两区中有一区到达尾端
        if (*first1 < *first2) ++first1;
        else if (*first2 < *first1) ++first2;
        else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

//version 2: 仿函数comp比较
//略

//========== set_difference ============
//构造S1, S2的差集:出现在S1中但不在S2中
//如果某个值在S1出现n次,在S2中出现m次,那么该值在输出区间内出现max(n-m,0)次,且全部来自S1
//stable算法

//version 1
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
InputIterator1 first2, InputIterator1 last2, OutputIterator result)
{
    //当两个区间都尚未到达尾端时,执行以下操作
    while (first1 != last1 && first2 != last2) {
        //在两个区间分别移动迭代器,当第一区间的元素等于第二区间的元素(表示此值同时存在域两个区间),
        //就让两区间同时前进;当第一区间的元素大于第二区间的元素,就让第二区间前进;
        //有了这两种处理,就保证当第一区间的元素小于第二区间的元素时,第一区间的元素只存在于第一区间中,
        //不存在于第二区间,于是将它记录于目标区
        if (*first1 < *first2) {
            *result = * first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1)
            ++first2;
        else {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}

//version 2: 仿函数comp比较
//略

//========== set_symmetric_difference ============
//构造S1, S2的对称差集: (S1-S2) U (S2-S1)
//如果某个值在S1出现n次,在S2中出现m次,那么该值在输出区间内出现|m-n|次
//如果n>m:输出区间内最后n-m个元素将由S1复制而来
//如果n<m:输出区间内最后n-m个元素将由S2复制而来
//stable算法

//version 1
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
InputIterator1 first2, InputIterator1 last2, OutputIterator result)
{
    //当两个区间都尚未到达尾端时,执行以下操作
    while (first1 != last1 && first2 != last2) {
        //在两区间内分别移动迭代器,当两区间内元素相等,就让两区间同时前进
        //当两区间内元素不等,就记录较小值于目标区,并令较小值所在区间前进
        if (*first1 < *first2) {
            *result = *first;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            *result = *first1;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//version 2: 仿函数comp比较
//略


/* ===========================================================
 * heap 算法在<stl_heap.h>中
 * 04-container中说明过: make_heap/push_heap/pop_heap/sort_heap
 * SGI STL <stl_algo.h>包含<stl.heap.h>
 * =========================================================== */


/* ===========================================================
 * 其他算法
 * =========================================================== */



#endif // SGI_STL_ALGO_BASE_H