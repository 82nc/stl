#ifndef SGI_STL_NUMERIC_H
#define SGI_STL_NUMERIC_H

/*
 * 统称为数值算法,STL规定,欲使用他们,客户端必须包含头文件<numeric>.
 * SGI 将他们实现于 <stl_numeric.h>
 */

// demo
#include <numeric>
#include <vector>
#include <functional>
#include <iostream>
#include <iterator> //iostream_iterator 

using namespace std;

int main()
{
    int ia[5] = { 1, 2, 3, 4, 5 };
    vector<int> iv(ia, ia+5);

    cout << accumulate(iv.begin(), iv.end(), 0);
    // 15: 0 + 1 + 2 + 3 + 4 + 5;

    cout << accumulate(iv.begin(), iv.end(), 0, minus<int>()) << endl;
    // -15: 0 - 1 - 2 - 3 - 4 - 5

    cout << inner_product(iv.begin(), iv.end(), iv.begin(), 10) << endl;
    // -20: 10 + 1*1 + 2*2 + 3*3 + 4*4 + 5*5

    cout << inner_product(iv.begin(), iv.end(), iv.begin(), 10,
                          minus<int>(), plus<int>()) << endl;
    // -20: 10 - 1+1 - 2+2 - 3+3 - 4+4 -5+5

    //以下这个迭代器将绑定到cout,作为输出用
    ostream_iterator<int> oite(cout, "");

    partial_sum(iv.begin(), iv.end(), oite);
    // 1 3 6 10 15 (第n个新元素是前n个旧元素的相加总计)

    partial_sum(iv.begin(), iv.end(), oite, minus<int>());
    // 1 -1 -4 -8 -13 (第n个元素是前n个旧元素的运算总计)

    adjacent_difference(iv.begin(), iv.end(), oite, minus<int>());
    // 1 1 1 1 1 (#1元素照录, #n新元素等于#n旧元素 - #n-1旧元素)
    adjacent_difference(iv.begin(), iv.end(), oite);
    // 1 3 5 7 9 (#1元素照录, #n新元素等于op(#n旧元素, #n-1旧元素))

    cout << power(10, 3) << endl;  // 1000, 10*10*10
    cout << power(10, 3, plus<int>()) << endl;  // 30, 10+10+10

    int n = 3;
    iota(iv.begin(), iv.end(), n); //在指定区间内填入n,n+1,n+2...
    for (int i=0; i<iv.size(); ++i)
        cout << iv[i] << ' ';      // 3 4 5 6 7
}

//以下是一些数值算法的具体实现

//========== accumulate ==============
//version 1
template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init)
{
    for (; first != last; ++first)
        init = init + *first; //将每个元素值累加到初值init上
    return init;
}

//version 2
template <class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
{
    for (; first != last; ++first)
        init = binary_op(init + *first); //对每个元素执行二元操作
    return init;
//binary_op: 不需要满足交换律和结合律
}

//========== adjacent_difference ============
//用来计算[first,last]中相邻元素的差额,即: 将*first
//赋值给*result,并针对[first+1,last)内的每一个迭代器
//i,将 *i - *(i-1)之值赋值给*(result+(i-first))
//注意: 你可以采用就地运算方式(即令result=first),
//这样它就成为了一个质变算法(mutating algorithm)
//如果加法与减法的定义如常规定义,那么adjacent_difference
//与partial_sum互为逆运算

//version 1
template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
{
    if (first == last; ) return result;

    *result = *first; //首先记录第一个元素
    return __adjacent_difference(first, last, result, value_type(first));

    //侯捷注: 经验证不需要上行那样调用,可改用以下写法(整个函数)
    //if (first == last) return result;
    //*result = *first;
    //iterator_traits<InputIterator>::value_type value = *first;
    //while (++first != last) { //走过整个区间
    // ...以下同 __adjacent_difference()对应内容
    //}
    //
    //这样的观念和做法,适用于本文件所有函数.以后不再赘述
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, 
OutputIterator result, T*)
{
    T value = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = tmp - value; //将相邻元素的差额(后-前)赋值给目的端
        value = tmp;
    }
    return ++result;
}

//version 2
template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, 
OutputIterator result, BinaryOperation binary_op)
{
    if (first == last; ) return result;

    *result = *first; //首先记录第一个元素
    return __adjacent_difference(first, last, result, value_type(first), binary_op);
}

template <class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, 
OutputIterator result, T*, BinaryOperation binary_opt)
{
    T value = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = binary_opt(tmp, value); //将相邻元素的运算结果赋值给目的端
        value = tmp;
    }
    return ++result;
}

//========== inner_product ============
//version 1
template <class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, 
InputIterator2 first2, InputIterator2 last2, T init)
{
    for (; first1 != last1; ++first1, ++first2)
        init = init + (*first1 * first2); //执行两个序列的一般内积
    return init;
}

//version 2
template <class InputIterator1, class InputIterator2, class T,
          class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last, 
InputIterator2 first2, InputIterator2 last2, T init, BinaryOperation1 binary_op1,
BinaryOperation1 binary_op2)
{
    //以第一序列之元素为据,将两个序列都走一遍
    for (; first1 != last1; ++first1, ++first2)
        //以外界提供的仿函数来取代第一版本中的operator*和operator+
        init = binary_op1(init, binary_op2(*first1, *first2));
    return init;
//binary_op1&2: 不需要满足交换律和结合律
}

//========== partial_sum ============
//注意result可以等于first,这样我们可以完成就地计算,这样它就成了一个质变算法
//binary_op1不需要满足交换律和结合律

//version 1
template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, 
OutputIterator result)
{
    if (first == last) return result;
    *result = *first;
    return __partial_sum(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, T*)
{
    T value = *first;
    while (++first != last) {
        value = value + *first; //前n个元素的总和
        *++result = value;
    }
    return result;
}

//version 2
template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
OutputIterator result, BinaryOperation binary_op)
{
    if (first == last) return result;
    *result = * first;
    return __partial_sum(first, last, result, value_type(first), binary_op);
}

template <class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
OutputIterator result, T*, BinaryOperation binary_op)
{
    T value = *first;
    while (++first != last) {
        value = binary_op(value, *first);
        *++result = value;
    }
//binary_op: 不需要满足交换律和结合律
}

//========== power ============
//SGI专属,并不在STL标准之列.它用来计算某数的n幂次方,
//这里所谓的n幂次是指自己对自己进行某种运算,达n次.运算类型可由外界指定
//如果指定为乘法,那就是乘幂.

//version 1: 乘幂
template <class T, class Integer>
inline T power(T x, Integer n)
{
    return power(x, n, multiplies<T>()); //指定运算型式为乘法
}

//version 2: 幂次方,如果指定为乘法运算,则当n>=0时返回x^n
//注意"MonoidOperation"必须满足结合律(associative),但不满足交换律(commutative)
template <class T, class Integer, class MonoidOperation>
T power(T x, Integer n, MonoidOperation op)
{
    if (n == 0)
        return identity_element(op); //取出"政同元素"identity element
    else {
        while ((n & 1) == 0) {
            n >>= 1;
            x = op(x, x);
        }
        T result = x;
        n >>= 1;
        while (n != 0) {
            x = op(x, x);
            if ((n & 1) != 0)
                result = op(result, x);
            n >>= 1;
        }
        return result;
    }
}

//========== iota ============
//SGI专属,并不在STL标准之列.它用来设定某个区间的内容,使其内的每一个元素
//从指定的value值开始,呈现递增状态.它改变了区间内容,所以是一种质变算法
//函数意义:在[first,last)区间内填入value,value+1,value+2...
//侯捷:iota是什么的缩写?
template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
    while (first != last) *first++ = value++;
}

#endif // SGI_STL_NUMERIC_H