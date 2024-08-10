#ifndef SGI_STL_ALGO_BASE_H
#define SGI_STL_ALGO_BASE_H

/*
 * STL标准规格中并没有区分基本算法或者复杂算法,然后SGI却把常用的一些
 * 算法定义于<stl_algobase.h>之中,其他算法定义于<stl_algo.h>中.
 */

#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>

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
}


#endif // SGI_STL_ALGO_BASE_H