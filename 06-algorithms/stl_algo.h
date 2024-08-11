#ifndef SGI_STL_ALGO_H
#define SGI_STL_ALGO_H


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
 
/*

#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

using namespce std;

template <class T>
struct display {
    void operator()(const T& x) const { cout << x << ' '; }
};

struct even {
    bool operator()(int x) const { return x%2 ? false : true; }
};

struct event_by_two {
    int operator()() cosnt { return _x += 2; }
private:
    static int _x;
};

int event_by_two::_x = 0;

int main() 
{
    int ia[] = { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
    vector<int> iv(ia, ia+sizeof(ia)/sizeof(int));

    //找出iv之中相邻元素值相等的第一个元素
    cout << *adjacent_find(iv.begin(), iv.end()) << endl; //6

    //找出iv之中相邻元素值相等的第一个元素
    cout << *adjacent_find(iv.begin(), iv.end(), equal_to<int>()) 
    << endl; //6

    //找出iv之中元素值为6的元素个数
    cout << *count(iv.begin(), iv.end(), 6) << endl; //3

    //找出iv之中元素值小于7的元素个数
    cout << *count_if(iv.begin(), iv.end(), bind2nd(less<int>()), 7) 
    << endl; //9

    //找出iv之中元素值为4的第一个元素的所在位置的值
    cout << *find(iv.begin(), iv.end(), 4) << endl; //4

    //找出iv之中元素值大于2的第一个元素的所在位置的值
    cout << *find_if(iv.begin(), iv.end(), bind2nd(greater<int>(), 2)) 
    << endl; //3

    //找出iv之子序列iv2所出现的最后一个位置(再往后3个位置的值)
    vector<int> iv2(ia+6, ia+8); //(6,6)
    cout << *(find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end())+3) 
    << endl; //8

    //找出iv之子序列iv2所出现的第一个位置(再往后3个位置的值)
    cout << *(find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end())+3) 
    << endl; //7

    //迭代器遍历整个iv区间,对每个元素施行display操作(不得改变元素内容)
    for_each(iv.begin(), iv.end(), display<int>()); cout << endl;
    //iv: 0 1 2 3 4 5 6 7 8

    //以下错误: generate的第三个参数(仿函数)本身不得有任何参数
    //generate(iv.begin(), iv.end(), bind2nd(plus<int>(), 3)); //error
    //以下迭代遍历整个iv2区间,对每个元素施行even_by_two操作(得改变元素内容)
    generate(iv.begin(), iv2.end(), even_by_two());
    for_each(iv2.begin(), iv2.end(), display<int>()); cout << endl;
    //iv2: 2 4

    //迭代遍历所指区间(起点及长度),对每个元素施行even_by_two操作(得改变元素值)
    generate_n(iv.begin(), 3, even_by_two());
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 6 8 10 3 4 5 6 6 6 7 8

    //删除(但不删除)元素6,尾端可能有残余数据(可另以容器之erase函数去除之)
    remove(iv.begin(), iv.end(), 6);
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 8 10 3 4 5 7 8 6 6 7 8

    //删除(但不删除)元素6,结果置于另一区间
    vector<int> iv3(12);
    remove_copy(iv.begin(), iv.end(), iv3.begin(), 6);
    for_each(iv3.begin(), iv3.end(), display<int>());
    cout << endl; // iv3: 8 10 3 4 5 7 8 7 8 0 0 0

    //删除(但不删除)小于6的元素,尾端可能有残余数据
    remove_if(iv.begin(), iv.end(), bind2nd(less<int>(), 6));
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 8 10 7 8 6 6 7 8 6 7 8

    //删除(但不删除)小于7的元素,结果置于另一区间
    remove_copy_if(iv.begin(), iv.end(), iv3.begin(), bind2nd(less<int>(), 7));
    for_each(iv3.begin(), iv3.end(), display<int>());
    cout << endl; // iv: 8 10 7 8 7 8 7 8 8 0 0 0

    //将所有的元素值6,改为元素值3
    replace(iv.begin(), iv.end(), 6, 3);
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 8 10 7 8 3 3 7 8 3 7 8 8

    //将所有的元素值3,改为元素值5,结果置于另一区间
    replace_copy(iv.begin(), iv.end(), iv3.begin(), 3, 5);
    for_each(iv3.begin(), iv3.end(), display<int>());
    cout << endl; // iv3: 8 10 7 8 5 5 7 8 5 7 8 0

    //将所有小于5的元素,改为元素值2
    replace_if(iv.begin(), iv.end(), bind2nd(less<int>(), 5), 2);
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 8 10 7 8 2 2 7 8 2 7 8

    //将所有等于8的元素,改为元素值9,结果置于另区间
    replace_copy_if(iv.begin(), iv.end(), iv3.begin(), bind2nd(equal_to<int>(), 8), 9);
    for_each(iv3.begin(), iv3.end(), display<int>());
    cout << endl; // iv3: 9 10 7 9 2 2 7 9 2 7 9 0

    //逆向重排每个元素
    reserve(iv.begin(), iv.end());
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 8 7 2 8 7 2 2 8 7 10 8

    //逆向重排每个元素,结果置于另一区间
    reserve_copy(iv.begin(), iv.end(), iv3.begin());
    for_each(iv3.begin(), iv3.end(), display<int>());
    cout << endl; // iv3: 8 10 7 8 2 2 7 8 2 7 8 0

    //旋转(互换元素)[first, middle)和[middle, last)
    rotate(iv.begin(), iv.begin()+4, iv.end());
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 7 2 2 8 7 10 8 8 7 2 8

    //旋转(互换元素)[first, middle)和[middle, last),结果置于另一区间
    rotate_copy(iv.begin(), iv.begin()+5, iv.end(), iv3.begin());
    for_each(iv3.begin(), iv3.end(), display<int>());
    cout << endl; // iv3: 10 8 8 7 2 8 7 2 2 8 7 0

    //查找某个子序列第一次出现位置
    int ia2[3] = { 2, 8 };
    vector<int> iv4(ia2, ia2+2); // iv4: { 2, 8 }
    cout << *search(iv.begin(), iv.end(), iv4.begin(), iv4.end()) << endl; // 2

    //查找连续出现2个8的子序列起点
    cout << *search_n(iv.begin(), iv.end(), 2, 8); // 8

    //查找连续出现3个小于8的子序列起点
    cout << *search_n(iv.begin(), iv.end(), 3, 8, less<int>()) << endl; // 7

    //将两个区间内元素互换.第二区间的元素个数不应小于第一区间的元素个数
    swap_ranges(iv4.begin(), iv4.end(), iv.begin());
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 2 8 2 8 7 10 8 8 7 2 8
    for_each(iv4.begin(), iv4.end(), display<int>()); //iv4: 7 2
    cout << endl;

    //改变区间的值,全部减2
    transform(iv.begin(), iv.end(), iv.begin(), bind2nd(minus<int>(), 2));
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 0 6 0 6 5 8 6 6 5 0 6

    //改变区间的值,令第二区间的元素值加到第一区间的对应元素身上
    //第二区间的元素个数不应小于第一区间的个数
    transform(iv.begin(), iv.end(), iv.begin(), iv.end(), plus<int>());
    for_each(iv.begin(), iv.end(), display<int>());
    cout << endl; // iv: 0 12 0 12 10 16 12 12 10 0 12

    //********************

    vector<int> iv5(ia, ia+sizeof(ia)/sizeof(int));
    vector<int> iv6(ia+4, ia+8);
    vector<int> iv7(15);




}

 */

 //==== 单纯的数据处理
 //==== 线性移动、线性查找、计数、循环遍历、逐一对元素施行指定运算等操作




#endif // SGI_STL_ALGO_H