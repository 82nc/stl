#ifndef SGI_STL_ALGO_H
#define SGI_STL_ALGO_H

#include "03-iterator/stl_iterator.h"


/* ===========================================================
 * set union/intersection/difference/symmetric-difference
 * 以上4个算法接受的set必须是有序空间,元素可以重复,即可以接受set/multiset,
 * hash_set/hash_multiset则不能使用
 * =========================================================== */

//========== set_union ============
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
    //以下尚未到达尾端的区间的所有剩余元素拷贝到目的端
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
    for_each(iv5.begin(), iv5.end(), display<int>()); 
    cout << endl; // iv5: 0 1 2 3 4 5 6 6 6 7 8
    for_each(iv6.begin(), iv6.end(), display<int>()); 
    cout << endl; // iv6: 4 5 6 6

    cout << *max_element(iv5.begin(), iv5.end()) << endl; // 8
    cout << *min_element(iv5.begin(), iv5.end()) << endl; // 0

    //判断是否iv6内所有元素都出现iv5中
    //注意两个序列都是sorted ranges
    cout << include(iv5.begin(), iv5,end(), iv6.begin(), iv6.end()) <<
    endl; // 1 (true)

    //将两个序列合并成一个序列
    //注意两个序列都必须是sorted ranges,结果也是sorted ranges
    merge(iv5.begin(), iv5.end(), iv6.begin(), iv6.end(), iv7.begin());
    for_each(iv7.begin(), iv7.end(), display<int>()); 
    cout << endl; // iv7: 0 1 2 3 4 4 5 5 6 6 6 6 7 8

    //符合条件的元素放在容器前段,不符合的元素放在容器后段,不保证保留原相对次序
    partition(iv7.begin(), iv.end(), even());
    for_each(iv7.begin(), iv7.end(), display<int>()); 
    cout << endl; // iv7: 0 8 2 6 4 4 6 6 6 6 5 5 3 7 1

    //去除连续而重复的数据(残余数据在后面)
    unique(iv5.begin(), iv5.end());
    for_each(iv5.begin(), iv5.end(), display<int>()); 
    cout << endl; // iv5: 0 1 2 3 4 5 6 7 8 7 8

    //去除连续而重复的数据,将结果放在另一处(残余数据在后面)
    unique_copy(iv5.begin(), iv5.end(), iv7.begin());
    for_each(iv7.begin(), iv7.end(), display<int>()); 
    cout << endl; // iv5: 0 1 2 3 4 5 6 7 8 7 8 5 3 7 1
}

 */

//==== 单纯的数据处理
//==== 线性移动、线性查找、计数、循环遍历、逐一对元素施行指定运算等操作

//=== adjacent_find ====
//找出第一组满足条件的相邻元素
//version 1: 查找相邻的重复元素
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
{
    if (first == last) return last;
    ForwardIterator next = first;
    while (++next != last) {
        if (*first == *next) 
            return first; //如果找到就结束
        first = next;
    }
    return last;
}

//version 2: 查找相邻的重复元素
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, 
BinaryPredicate binary_pred)
{
    if (first == last) return last;
    ForwardIterator next = first;
    while (++next != last) {
        if (binary_pred(*first, *next)) 
            return first; //如果找到就结束
        first = next;
    }
    return last;
}

//=== count ====
//返回区间内等于value的元素个数
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value)
{
    //以下声明一个计数器n
    typename iterator_traits<InputIterator>::diference_type n = 0;
    for (; first != last; ++first)
        if (*first == value)
            ++n;
    return n;
}

//count()有一个早期版本
template <class InputIterator, class T, class Size>
void count(InputIterator first, InputIterator last, const T& value, Size& n)
{
    for (; first != last; ++first)
        if (*first == value)
            ++n;
}

//=== count_if ====
//返回区间内pred执行为true的元素个数
template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first)
        if (pred(*first))
            ++n;
    return n;
}

//count_if()有一个早期版本
template <class InputIterator, class Predicate, class Size>
void count_if(InputIterator first, InputIterator last, Predicate pred, Size& n)
{
    for (; first != last; ++first)
        if (pred(*first))
            ++n;
}

//=== find ====
//找出第一个匹配value的元素
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value)
{
    while (first != last && *first != value)
        ++first;
    return first;
}

//=== find_if ====
//找出第一个pred执行为true的元素
template <class InputIterator, class T>
InputIterator find_if(InputIterator first, InputIterator last, const T& value)
{
    while (first != last && *first != value)
        ++first;
    return first;
}

//=== find_end ====
//在序列一中,查找序列二的最后一次出现点
template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
ForwardIterator2 first2, ForwardIterator2 last2)
{
    typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
    typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

    return __find_end(first1, last1, first2, last2, category1(), category2());
}

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag)
{
    if (first2 == last2)
        return last1;
    else {
        ForwardIterator1 result = last1;
        while (1) {
            ForwardIterator1 new_result = search(first1, last1, first2, last2);
            if (new_result == last1)
                return result;
            else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag)
{
    typedef reverse_iterator<BidirectionalIterator1> reviter1;
    typedef reverse_iterator<BidirectionalIterator2> reviter2;

    reviter1 rlast1(first1);
    reviter2 rlast2(first2);

    reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);

    if (rresult == rlast1)
        return last1;
    else {
        BidirectionalIterator1 result = rresult.base(); //转回正常(非逆序)迭代器
        advance(result, -distance(first2, last2)); //调整回到子序列的起头处
        return result;
    }
    //为什么要将逆向迭代器转成正向迭代器,而不直接移动逆向迭代器？
    //因为正逆向迭代器之间有奇妙的"实体关系"和"逻辑关系"
}

//=== find_first_of ====
//在序列一中,查找序列二的首次出现点
//version 1
template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
ForwardIterator first2, ForwardIterator last2)
{
    for (; first1 != last1; ++first1)
        for (ForwardIterator i=first2; i != last2; ++i)
            if (*first1 == *i)
                return first1;
    return last1;
}

//version 2
template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp)
{
    for (; first1 != last1; ++first1)
        for (ForwardIterator i=first2; i != last2; ++i)
            if (comp(*first1, *i))
                return first1;
    return last1;
}

//=== find_each ====
//将仿函数施加于区间上每个元素
template <class InpuIterator, class Function>
Function for_each(InpuIterator first, InpuIterator last, Function f)
{
    for (; first != last; ++first)
        f(*first); //invoke f's function call[operator()]
    return f;
}

//==== generate ====
//将仿函数运算结果填写在区间所有元素上
template <class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, generate gene)
{
    for (; first != last; ++first)
        *first = gen();
}

//=== generate_n ====
//将仿函数运算结果填写在区间开始的n个元素上
template <class OutputIterator, class Size, class Generator>
void generate(OutputIterator first, Size n, generate gene)
{
    for (; n > 0; --n, ++first)
        *first = gen();
    return first;
}

//=== includes =====
//判断序列二是否涵盖于序列一中,S1和S2必须都是有序(可重复)集合
//version 1: operator<
template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2)
{
    while (first1 != last1 && first2 != last2)
        if (*first2 < *first1)
            return false;
        else if (*first1 < *first2)
            ++first1;
        else
            ++first1, ++first2;
    return first2 == last2;
}

//version 2: comp
template <class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2, Comapre comp)
{
    while (first1 != last1 && first2 != last2)
        if (comp(*first2, *first1))
            return false;
        else if (comp(*first1, *first2))
            ++first1;
        else
            ++first1, ++first2;
    return first2 == last2;
}

//=== max_element =====
//version 1
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
{
    if (first == last) return first;
    ForwardIterator result = first;
    while (++first != last)
        if (*result < *first) 
            result = first;
    return result;
}

//version 2
template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, 
Compare comp)
{
    if (first == last) return first;
    ForwardIterator result = first;
    while (++first != last)
        if (comp(*result, *first))
            result = first;
    return result;
}

//=== min_element =====
//version 1
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
{
    if (first == last) return first;
    ForwardIterator result = first;
    while (++first != last)
        if (*first < *result) 
            result = first;
    return result;
}

//version 2
template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, 
Compare comp)
{
    if (first == last) return first;
    ForwardIterator result = first;
    while (++first != last)
        if (comp(*first, *result))
            result = first;
    return result;
}

//==== merge =====
//将两个经过排序的集合S1和S2合并起来置于另一个空间
//version 1
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2, OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//version 2
template <class InputIterator1, class InputIterator2, 
class OutputIterator, class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2, 
OutputIterator result, Compare comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//=== partition =====
//对区间元素重新排列,被一元条件运算符pred判定为true的放置在前端,false的放置在后端
//该算法不保证保留元素的原始相对位置,对应稳定算法有stable_partition
template <class BidirectionalIterator first, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, 
Predicate pred)
{
    while (true) {
        while (true)
            if (first == last) 
                return first;
            else if (pred(*first))
                ++first;
            else
                break;
        --last;

        while (true)
            if (first == last) 
                return first;
            else if (pred(*last))
                --last;
            else
                break;
        iter_swap(first, last);
        ++first;
    }
}

//=== remove移除(但不删除) =====
//移除区间内所有等于value的元素
template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
{
    first = find(first, last, value);
    ForwardIterator next = first;
    //利用remove_copy允许新旧容器重叠的性质进行移除操作
    return first == last ? first : remove_copy(++next, last, first, value);
}

//=== remove_if移除(但不删除) =====
//移除区间内所有被仿函数核定为true的元素
template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
Predicate pred)
{
    first = find_if(first, last, pred);
    ForwardIterator next = first;
    //利用remove_copy_if允许新旧容器重叠的性质进行移除操作
    return first == last ? first : remove_copy_if(++next, last, first, pred);
}

//=== remove_copy移除(但不删除) =====
//移除区间内所有等于value的元素,将结果置于另一容器
template <class InputIterator, class OutputIterator, class T>
ForwardIterator remove_copy(InputIterator first, InputIterator last, 
OutputIterator result, const T& value)
{
    for (; first != last; ++first) 
        if (*first != value) {
            *result = *first;
            ++result;
        }
    return result;
}

//==== remove_copy_if移除(但不删除) ======
//移除区间内所有被仿函数核定为true的元素,将结果置于另一容器
template <class InputIterator, class OutputIterator, class Predicate>
ForwardIterator remove_copy_if(InputIterator first, InputIterator last, 
OutputIterator result, Predicate pred)
{
    for (; first != last; ++first) 
        if (!pred(*first, value)) {
            *result = *first; //赋值给新容器(保留不删除)
            ++result;
        }
    return result;
}

//=== replace =====
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, 
const T& old_value, const T& new_value)
{
    for (; first != last; ++first)
        if (*first == old_value) *first = new_value;
}

//=== replace_copy =====
template <class InputIterator, class OutputIterator, class T>
void replace_copy(InputIterator first, InputIterator last, 
OutputIterator result, const T& old_value, const T& new_value)
{
    for (; first != last; ++first, ++result)
        *result = *first == old_value ? new_value : *first;
}

//=== replace_if =====
template <class ForwardIterator, class Predicate, class T>
void replace(ForwardIterator first, ForwardIterator last, 
Predicate pred, const T& new_value)
{
    for (; first != last; ++first)
        if (pred(*first)) *first = new_value;
}

//==== replace_copy_if =====
template <class Iterator, class OutputIterator, class Predicate, class T>
void replace_copy(Iterator first, Iterator last, 
OutputIterator result, Predicate pred, const T& new_value)
{
    for (; first != last; ++first, ++result)
        *result = pred(*first) ? new_value : *first;
}

//==== reverse =====
//将区间元素在原容器里颠倒重排
template <class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last)
{
    __reverse(first, last, iterator_category(first));
}

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, 
bidirectional_iterator_tag)
{
    while (true)
        if (first == last || first == --last)
            return;
        else
            iter_swap(first++, last);
}

template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last, 
random_access_iterator_tag)
{
    while (first < last) //只有random iterator才可以这样的判断
        iter_swap(first++, --last);
}

//==== reverse_copy ======
template <class BidirectionalIterator, class OutputIterator>
void reverse_copy(BidirectionalIterator first, BidirectionalIterator last, 
OutputIterator result)
{
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;   
}

//==== rotate ======
//将[first, middle)和[middle, last)元素互换
template <class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle, 
ForwardIterator last)
{
    if (first == middle || middle == last) return;
    __rotate(first, middle, last, distance_type(first), iterator_category(first));
}

template <class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle, 
ForwardIterator last, Distance*, forward_iterator_tag)
{
    for (ForwardIterator i = middle; ;) {
        iter_swap(first, i);
        ++first;
        ++i;
        if (first == middle) {
            if (i == last) return;
            middle = i;
        } else if (i == last)
            i = middle;
    }
}

template <class BidirectionalIterator, class Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle, 
BidirectionalIterator last, Distance*, bidirectional_iterator_tag)
{
    reverse(first, middle);
    reverse(middle, last);
    reverse(first, last);
}

template <class RandomAccessIterator, class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle, 
RandomAccessIterator last, Distance*, random_access_iterator_tag)
{
    Distance n = __gcd(last - first, middle - first);
    while (n--)
        __rotate_cycle(first, last, first + n; middle - first, value_type(first));
}

//=== 最大公因子,利用辗转相除法 =====
template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
{
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <class RandomAccessIterator, class Distance, class T>
void __rotate__cycle(RandomAccessIterator first, RandomAccessIterator last, 
RandomAccessIterator initial, Distance shift, T*)
{
    T value = *initial;
    RandomAccessIterator p1 = initial;
    RandomAccessIterator p2 = p1 + shift;
    while (p2 != initial) {
        *p1 = *ps;
        p1 = p2;
        if (last - p2 > shift)
            p2 += shift;
        else    
            p2 = first + (shift -(last - p2));
    }
    *p1 = value;
}

//==== rotate_copy =====
template <class ForwardInterator, class OutputIterator>
OutputIterator rotate_copy(ForwardInterator first, ForwardInterator middle,
ForwardInterator last, OutputIterator result)
{
    return copy(first, middle, copy(middle, last, result));
}

//==== search ======
//在序列1中查找序列2的首次出现点
//version 1
template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
ForwardIterator2 first2, ForwardIterator2 last2)
{
    return __search(first1, last1, first2, last2, 
    distance_type<first>(first1), distance_type(first2));
}

template <class ForwardIterator1, class ForwardIterator2, 
class Distance1, class Distance2>
inline ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
ForwardIterator2 first2, ForwardIterator2 last2, Distance1*, Distance2*)
{
    Distance1 d1 = 0;
    distance(first1, last1, d1);
    Distance1 d2 = 0;
    distance(first1, last1, d2);

    if (d1 < d2) return last1;

    ForwardIterator1 c1 = first1;
    ForwardIterator1 c2 = first2;

    while (c1 != last2)
        if (*c1 == *c2) { //equality
            ++c1;
            ++c2;
        } else {
            if (d1 == d2)
                return last1;
            else {
                c1 = ++first1;
                c2 = first2;
                --d1;
            }
        }
        return first1;
}

//version 2: pred 略

//==== search_n ======
//version 1: 查找元素value连续出现count次的子序列,返回其发生位置
template <class ForwardIterator, class Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, 
Integer count, const T& value)
{
    if (count <= 0)
        return first;
    else {
        first = find(first, last, value); //首次出现点
        while (first != last) {
            Integer n = count - 1;
            ForwardIterator i = first;
            ++i;
            while (i != last && n != 0 && *i == value) {
                ++i;
                --n;
            }
            if (n == 0)
                return first;
            else
                first = find(i, last, value);
        }
        return last;
    }
}

//version 2: pred
template <class ForwardIterator, class Integer, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
Integer count, const T& value, BinaryPredicate binary_pred) 
{
    if (count <= 0)
        return first;
    else {
        while (first != last) {
            if (binary_pred(*first, value)) 
                break;
            ++first;
        }
        while (first != last) {
            Integer n = count - 1;
            ForwardIterator i = first;
            ++i;
            while (i != last && n != 0 && binary_pred(*i, value)) {
                ++i;
                --n;
            }
            if (n == 0)
                return first;
            else {
                while (i != last) {
                    if (binary_pred(*i, value)) break;
                    ++i;
                }
                first = i; //回到外循环
            }
        }
        return last;
    }
}

//===== swap_ranges =======
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
ForwardIterator2 first2)
{
    for (; first1 != last1; ++first1, ++first2)
        iter_swap(first1, first2);
    return first2;
}

//==== transform ======
//以仿函数op作用域区间内每个元素,并将结果回写至一个新序列(也可以原序列)
//version 1
template <class Inputiterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, Inputiterator last, 
OutputIterator result, UnaryOperation op)
{
    for (; first != last; ++first, ++result)
        *result = op(*first);
    return result;
}

//version 2
template <class Inputiterator1, class Inputiterator2, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first1, Inputiterator last1, 
InputIterator first2, OutputIterator result, UnaryOperation op)
{
    for (; first1 != last1; ++first1, ++first2, ++result)
        *result = op(*first1, *first2);
    return result;
}

//===== unique ======
//移除序列内重复元素
//stable
//version 1: equality
template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last)
{
    first = adjacent_find(first, last);
    return unique_copy(first, last, first);
}

//version 2: binary predicate 略

//===== unique_copy =======
//version 1: equality
template <class InputIterator, class OutputIterator>
inline OutputIterator unique_copy(InputIterator first, InputIterator last, 
OutputIterator result)
{
    if (first == last) return result;
    return __unique_copy(first, last, result, iterator_category(result));
}

//version 2: binary predicate 略

template <class InputIterator, class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
ForwardIterator result, forward_iterator_tag)
{
    *result = *first;
    while (++first != last)
        if (*result != *first) *++result = *first;
    return ++result;
}

template <class InputIterator, class OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
OutputIterator result, output_iterator_tag)
{
    //以下output iterator有一些功能限制,所以必须知道其value type
    return __unique_copy(first, last, result, value_type(first));
}


template <class InputIterator, class OutputIterator, class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
OutputIterator result, T*)
{
    //output iterator为write only, 无法像forward iterator那样可以读取
    //所以不能有类似*result != *first这样的判断,所以才需要有这一版辅助函数
    //例如ostream_iterator就是一个output iterator

    //T为output iterator的value_type
    T value = *first;
    *result = value;
    while (++first != last) 
        if (value != *first) {
            value = *first;
            *++result = value;
        }
    return ++result;
}

//以下算法工作原理比较复杂,以下先举例说明
//**********************************/
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

struct even {
    bool operator()(int x) const { return x%2 ? false : true; }
};


int main()
{
    int ia[] = { 12, 17, 20, 22, 23, 30, 33, 40 };
    vector<int> iv(ia, ia+sizeof(ia)/sizeof(int));

    cout << *lower_bound(iv.begin(), iv.end(), 21) << endl; //22
    cout << *upper_bound(iv.begin(), iv.end(), 21) << endl; //22
    cout << *lower_bound(iv.begin(), iv.end(), 22) << endl; //22
    cout << *upper_bound(iv.begin(), iv.end(), 22) << endl; //23

    //面对有序区间(sorted range),可以二分查找法寻找某个元素
    cout << binary_search(iv.begin(), iv.end(), 33) << endl; //1-true
    cout << binary_search(iv.begin(), iv.end(), 34) << endl; //0-false

    //下个排列组合
    next_permutation(iv.begin(), iv.end());
    copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //40 33 30 23 22 20 17 12

    //在iv尾端附加新元素,使成为40 33 30 23 22 20 17 12 22 30 17
    iv.push_back(22);
    iv.push_back(30);
    iv.push_back(17);

    //排序并保持"原相对位置"
    stable_sort(iv.begin(), iv.end());
    copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //12 17 17 20 22 22 23 30 33 40

    //面对一个有序区间,找出其中一个子区间,其内每个元素都与某特定元素值相同
    //返回该子区间的头尾迭代器.如果没有这样的子区间,返回的头尾迭代器均指向
    //该特定元素可插入(可仍保持排序)的地点
    pair<vector<int>::iterator, vector<int>::iterator> pairIte;
    pairIte = equal_ranges(iv.begin(), iv.end(), 22);
    cout << *(pairIte.first) << endl; //22(lower_bound)
    cout << *(pairIte.second) << endl; //23(upper_bound)

    pairIte = equal_range(iv.begin(), iv.end(), 25);
    cout << *(pairIte.first) << endl; //30(lower_bound)
    cout << *(pairIte.second) << endl; //30(upper_bound)

    random_shuffle(iv.begin(), iv.end());
    copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    cout << endl; //22 30 30 17 33 40 17 23 22 12 20

    //将小于*(iv.begin()+5)(本例40)的元素置于该元素之左
    //其余置于该元素之右,不保证维持原有的相对位置
    nth_element(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    cout << endl; //20 12 22 17 17 22 23 30 33 40

    //将大于*(iv.begin()+5)(本例为22)的元素置于该元素之左
    //其余置于该元素之右,不保证维持原有的相对位置
    nth_element(iv.begin(), iv.end()+5, iv.end(), greater<int>());
    copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    //以"是否符合even条件"为依据,将符合者置于左段,不符合这置于右段
    //保证维持原有的相对位置.如不需要"维持原有的相对位置",可改用partition()
    stable_partition(iv.begin(), iv.end()+5, iv.end(), even());
    copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
    cout << endl; //40 30 30 22 22 12 20 33 23 17 17

}

//===== lower_bound(应用于有序空间) =====
//version 1: operator<
template <class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first,
ForwardIterator last, const T& value)
{
    return __lower_bound(first, last, value, distance_type(first),
    iterator_category(first));
}

//version 2: comp
template <class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
const T& value, Compare comp)
{
    return __lower_bound(first, last, value, comp, distance_type(first),
    iterator_category(first));
}

//__lower_bound: forward_iterator
template <class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, 
ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
{
    Distance len = 0;
    distance(first, last, len); //求整个区间的长度len
    Distance half;
    ForwardIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

//__lower_bound: random_access_iterator
template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first, 
RandomAccessIterator last, const T& value, Distance*, random_access_iterator)
{
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if (*middle < value) {
            first = middle + 1;
            len = len - half - 1;
        } else 
            len = half;
    }
    return first;
}

//====== upper_bound(应用于有序空间) =======
//version 1: operator<
template <class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
const T& value)
{
    return __upper_bound(first, last, value, distance_type(first),
    iterator_category(first));
}

//version 2: comp
template <class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
const T& value, Compare comp)
{
    return __upper_bound(first, last, value, comp, distance_type(first),
    iterator_category(first));
}

//__upper_bound: forward_iterator
template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, 
const T& value, Distance*, forward_iterator_tag)
{
    Distance len = 0;
    distance(first, last, len); //求整个区间的长度len
    Distance half;
    ForwardIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (value < *middle) {
            len = half;
        } else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

//__upper_bound: random_access_iterator
template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first, 
RandomAccessIterator last, const T& value, Distance*, random_access_iterator)
{
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if (value < *middle) {
            len = half;
        } else {
            first = middle + 1;
            len = len - half - 1;
        }
    }
    return first;
}

//===== binary_search(应用于有序空间) ======
//version 1
template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value)
{
    ForwardIterator i = lower_bound(first, last, value);
    return i !=last && !(value < *i);
}

//version 2
template <class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, 
const T& value, Compare comp)
{
    ForwardIterator i = lower_bound(first, last, value, comp);
    return i !=last && !(value < *i);
}

//===== next_permutation =======
//会取得[first,last)所标识之序列的下一个排列组合,如果没有下一个
//排列组合,便返回false;否则返回true
//version 1: less-than
template <class BidirectionalIterator>
bool next_permmutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last) 
        return false;
    BidirectionalIterator i = first;
    ++i;
    if (i == last) 
        return false;
    i = last;
    --i;
    for (;;) {
        BidirectionalIterator ii = i;
        --i;
        //以上锁定一组(两个)相邻元素
        if (*i < *ii) {
            BidirectionalIterator j = last;
            while (!(*i < *--j)); //由尾端往前找,直到遇上比*i大的元素
            iter_swap(i, j);
            reverse(ii, last); //ii之后的元素全部逆向重排
            return true;
        }
        if (i == first) { //进行至最前面
            reverse(first, last); //全部逆向重排
            return false;
        }
    }
}

//version 2: comp 略

//===== prev_permutation =======
//version: less-than
template <class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last) 
        return false;
    BidirectionalIterator i = first;
    ++i;
    if (i == last) return false;
    i = last;
    --i;

    for (;;) {
        BidirectionalIterator ii = i;
        --i;
        if (*ii < *i) {
            BidirectionalIterator j = last;
            while (!(*--j< *i));
            iter_swap(i, j);
            reserve(ii, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

//version 2: comp 略

//===== random_shuffle =======
//元素次序随机重排
//version 1: 使用内部随机数产生器
template <class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first,
RandomAccessIterator last)
{
    __random_shuffle(first, last, distance_type(first));
}

template <class RandomAccessIterator, class Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
Distance*)
{
    if (first == last) return;
    for (RandomAccessIterator i=first+1; i!=last; ++i)
    #ifdef __STL_NO_DRAND48
        iter_swap(i, first + Distance(rand()%((i-first) + 1)));
    #else
        iter_swap(i, first + Distance(rand48()%((i-first) + 1)));
    #endif
    //注意在我的GCC2.91.57中,__STL_NO_DRAND48是未定义的,因此上述实现代码
    //会采用lrand48()那个版本,单编译时却说lrand48() undeclared
}

//version 2
template <class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
RandomNumberGenerator& rand)
{
    if (first == last) return;
    for (RandomAccessIterator i=first+1; i!=last; ++i)
        iter_swap(i, first + rand((i - first) + 1));
}

//===== partial_sort/partial_sort_copy
//本算法middle迭代器(位于序列[first,last)),然后重新安排[first,last)
//然后重新安排[first,last),使序列中的middle-first个最小元素以递增顺序排序
//置于[first,midlle)内,其余last-middle个元素安置于[middle,last)中,不保证
//有任何特定顺序

//version 1
template <class RandomAccessIterator, class T>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
RandomAccessIterator last)
{
    __partial_sort(first, last, value_type(first));
}

template <class RandomAccessIterator, class T>
inline void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
RandomAccessIterator last, T*)
{
    make_heap(first, middle);
    //注意以下的i<last判断操作只适用于random iterator
    for (RandomAccessIterator i = middle; i < last; ++i)
        if (*i < *first)
            __pop_heap(first, middle, i, T(*i), distance_type(first));
    sort_heap(first, middle);
}

//version 1
template <class InputIterator, class RandomAccessIterator>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator middle,
RandomAccessIterator result_first, RandomAccessIterator result_last)
{
    //...
}

//version 2
template <class InputIterator, class RandomAccessIterator, class Compare>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator middle,
RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp)
{
    //...
}

/*
int ia[12] = { 69, 23, 80, 42, 17, 15, 26, 51, 19, 12, 35, 8 };
vector<int> vec(ia, ia+12);
ostream_iterator<int> oite(cout, " ");

partital_sort(vec.begin(), vec.begin()+7, vec.end());
copy(vec.begin(), vec.end(), oite); cout << endl;
//8 12 15 17 19 23 26 80 69 51 42 35

vector<int> res(7);
partital_sort_copy(vec.begin(), vec.begin()+7, res.begin());
copy(res.begin(), res.end(), oite); cout << endl;
//26 23 19 17 15 12 8

 */


//========== SORT ===========
/*
 1.STL所有的关系型容器都有自动排序功能(底层采用RB-Tree),不需要这个算法
 2.序列式容器stack,queue,priority-queue都有特别的出入口,不允许对元素修改
 3.序列式容器vector,deque,list.前两者迭代器属于RandomAccessIterator,适合
   sort算法;list的迭代器属于bidirectionalIterator,不适用sort算法,包括slist
   其迭代器是ForwardIterator,只能用他们自己提供的member function sort
 */

//STL sort算法,数据量大的时候采用Quick Sort,分段递归排序,一旦分段后的数据量
//小于某个门槛,为避免Quick Sort的递归调用带来过大的额外负荷,就改用Insertion
//Sort.如果递归层次过深,还会改用Heap Sort.

/*
Insertion Sort
以双层循环的形式进行,外循环遍历整个序列,每次迭代决定出一个子区间;
内层循环遍历子区间,将子区间内每个元素"逆转对"(迭代器i,j, i<j, *i>*j)倒转过来
一旦不存在"逆转对",则序列排列完毕.算法复杂度O(n^2)
STL并不开放Insertion Sort,故其函数仅为内部使用
*/

//version 1: operator <
template <class RandomAccessIterator>
void __sertionion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first == last) return;
    for (RandomAccessIterator i = first +1; i != last; ++i) //外循环
        __linear_insert(first, i, value_type(first));
        //以上[first,i)形成一个子区间
}

//version 2: comp 略

template <class RandomAccessIterator, class T>
inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    T value = *last;
    if (value < *first) {
        copy_backward(first, last, last+1); //将整个区间向右移个位置
        *first = value;
    } else {
        __unguarded_linear_insert(last, value);
    }
}

template <class RandomAccessIterator, class T>
void __unguarded_linear_insert(RandomAccessIterator last, T value)
{
    RandomAccessIterator next = last;
    --next;
    //insertion sort 的内循环
    while (value < *next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
/*
上述函数之所以命名成unguarded_x是因为,一般insertion sort在内循环原本需要做两次判断
判断是否两相邻元素是"逆转对",同时也判断循环的行进是否过边界.但由于上述所示源代码会导致
最小值必然在内循环子区间的最边缘,所以两个判断可合为一个判断,所以称unguarded_(忽略边界检查),
省下一个判断操作.
*/

/*
Quick Sort
大量数据情况下,insert sort算法的复杂度O(n^2)太高,可采用目前已知最快的排序算法
quick sort,其平均复杂度O(nlogn),最坏的情况将达O(n^2),不过introSort(极类似
media-of-three QuickSort的一种排序算法)可将最坏情况推进到O(nlogn)

算法说明如下. 假设S代表将被处理的序列
1.如果S的元素个数为0或者1,结束.
2.取S中的任何一个元素,当作枢轴(pivot)v.
3.将S分割成L,R两段,使L内的每个元素都小于或等于v,R内的每个元素都大于或等于v
4.对L,R递归执行quick sort

其本质就是将大区间分割成小区间,分段排序;每个小区间的排序完成后,串联起来的
大区间就完成了排序.最坏的情况发生在分割时产生了一个空区间,那样的话就没达到
分割的预期效果.

Media-of-Tree(三点中值)
任何一个元素都可以被选中当枢轴(pivot),但是其是否合适会影响快排的效率,为了避免
"元素输入时不够随机"所带来的恶化效应,最为理想稳妥的方式便是取整个序列的头、尾、中央
三个位置的元素,一起中间值作为枢轴,为了能够快速取出中央位置的元素,显然迭代器必须能够
随即定位,亦即必须是个RandomAccessIterators
*/

//返回a,b,c中间值
template <class T>
inline const T& __media(const T& a, const T& b, const T& c)
{
    if (a < b)
        if (b < c) return b;
        else if (a < c) return c;
        else return a;
    else if (a < c) return a;
    else if (b < c) return c;
    else return b;
}

//Partitioning(分割)
/* 
分割方法不止一种,以下SGI STL提供的简单有良好成效的做法
*/
//version 1
template <class RandomAccessIterator, class T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first,
RandomAccessIterator last, T pivot)
{
    while (true) {
        while (*first < pivot) ++first;
        --last;
        while (pivot < *last) --last;
        //以下判断只适用于random iterator
        if (!(first < last)) return first; //交错就停止
        iter_swap(first, last);
        ++first;
    }
    //返回分割后的右段第一个元素位置
}


//SGI STL Quick Sort

/*
threshold
面对一个只有十来个元素的小型序列,使用像quick sort的这样复杂而需要大量运算的排序算法
可能也是不划算的.那么适度评估序列以选择Quick Sort或者Insertion Sort怎么判定呢?

final insertion sort
如果我们令某个大小以下的序列滞留在几近完成的状态,最后再以一次insertion sort完成
将所有这些几近完成但尚未成功的子序列做一次完整排序,其效率一般认为会比将所有子序列
彻底排序要更好.这是因为insertion sort在面对几近排序的序列时有很好表现.

introsort
不当的枢轴选取会导致quick sort恶化为O(n^2)
David R.Musser于1996年提出一种混合式排序算法:Introspective Sorting(内省式排序)
其行为在大部分情况下与media-of-three quick sort完全相同.但当分割行为(partitioning)
有恶化为二次行为的倾向时,能够自我侦测,转而改用Heap Sort,是其效率维持在Heap Sort的O(nlogn)
又比一开始就是用Heap Sort来得好.
*/

//=== sort ====
//version 1
template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first != last) {
        __introsort_loop(first, last, value_type(first), __lg(last-first)*2);
        __final_insertion_sort(first, last);
    }
}

//__lg()控制分割恶化的情况
//找出2^k <= n的最大值k
template <class Size>
inline Size __lg(Size n)
{
    Size k;
    for (k=0; n>1; n>>=1) ++k;
    return k;
}

//比如last-first=40,则__introsort_loop的最后一个参数为5*2,表示最多允许分割10层

//version 1
const int __stl_threshold = 16;
template <class RandomAccessIterator, class T, class Size>
void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last,
T*, Size depth_limit)
{
    //以下__stl_threshold是个全局常数,稍早定义为const int 16
    while (last - first > __stl_threshold) { //> 16
        if (depth_limit == 0) {
            partial_sort(first, last, last); //改用heapsort
            return;
        }
        --depth_limit;
        //以下是media-of-three partition,选择一个较好的枢轴并决定分割
        RandomAccessIterator cur = __unguarded_partition(
            first, last, 
            T(__media(*first, *(first+(last-first)/2), *(last-1))));
        //对右半段递归进行sort
        __introsort_loop(cur, last, value_type(first), depth_limit);
        last = cur;
        //现在回到while循环,准备对其左半段递归进行sort
        //这种写法可读性差,效率并没有比较好
        //RW STL采用一般教科书写法(直观地对左半段和右半段递归)
    }
}

//version 1
template <class RandomAccessIterator>
void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (last - first > __stl_threshold) { //>16
        __insertion_sort(first, first + __stl_threshold, last);
        __unguarded_insertion_sort(first + __stl_threshold, last);
    } else {
        __insertion_sort(first, last);
    }
}

//version 1
template <class RandomAccessIterator>
inline void __unguarded_insertion_sort(RandomAccessIterator first,
RandomAccessIterator last)
{
    __unguarded_insertion_sort_aux(first, last, value_type(first));
}

//version 1
template <class RandomAccessIterator, class T>
inline void __unguarded_insertion_sort_aux(RandomAccessIterator first,
RandomAccessIterator last, T*)
{
    for (RandomAccessIterator i=first; 1!=last; ++i)
        __unguarded_linear_insert(i, T(*i));
}

//对比的话RW STL的sort则是纯粹的quick sort,不是introSort
template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (!(first == last)) {
        __quick_sort_loop(first, last);
        __final_insertion_sort(first, last); //其内部操作与SGI STL完全相同
    }
}

template <class RandomAccessIterator>
inline void __quick_sort_loop(RandomAccessIterator first,
RandomAccessIterator last)
{
    __quick_sort_loop_aux(first, last, __RWSTD_VALUE_TYPE(first));
}

template <class RandomAccessIterator, class T>
void __quick_sort_loop_aux(RandomAccessIterator first, 
RandomAccessIterator last, T*)
{
    while (last - first > stl_htreshold) {
        //media-of-3 partition
        RandomAccessIterator cut = __unguarded_partition(
            first, last,
            T(__media(*first, *(first+(last-first)/2, *(last-1)))));
        if (cut - first >= last - cut) {
            __quick_sort_loop(cut, last); //右段递归
            last = cut;
        } else {
            __quick_sort_loop(first, cut); //左段递归
            first = cut;
        }
    }
}

//==== equal range ======
/*
二分查找的一个版本,试图在已排序序列[first,last)中寻找value,他返回一对迭代器i和j
i:在不破坏原次序的的前提下,value可插入的第一个位置(亦即lower_bound)
j:在不破坏原次序的的前提下,value可插入的最后个位置(亦即upper_bound)
[i,i)内每个元素都等同于value,该区间是[first,last)内符合此性质的的最大子区间
*/

//version 1: operator<
template <class ForwardIterator, class T>
inline pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value)
{
    //根据迭代器的种类类型(category),采用不同策略
    return __equal_range(first, last, value, distance_type(first),
        iterator_category(first));
}

//version 2: comp

//version 1: random_access_iterator
template <class RandomAccessIterator, class T, class Distance>
pair<RandomAccessIterator, RandomAccessIterator>
__equal_range(RandomAccessIterator first, RandomAccessIterator last,
const T& value, Distance*, random_access_iterator_tag)
{
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle, left, right;

    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if (*middle < value) {
            first = middle + 1;
            len = len - half - 1;
        } else if (value < *middle) {
            len = half;
        } else {
            left = lower_bound(first, middle, value);
            right = upper_lower(++middle, first+len, value);
            return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
        }
    }
    //整个区间内都没有匹配的值,那么应该返回一对迭代器,指向第一个大于value的元素
    return pair<RandomAccessIterator, RandomAccessIterator>(first, last);
}

//version 2: forward_iterator
template <class ForwardIterator, class T, class Distance>
pair<ForwardIterator, ForwardIterator>
__equal_range(ForwardIterator first, ForwardIterator last,
const T& value, Distance*, forward_iterator_tag)
{
    Distance half;
    Distance len = last - first;
    ForwardIterator middle, left, right;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else if (value < *middle) {
            len = half;
        } else {
            left = lower_bound(first, middle, value);
            advance(first, len);
            right = upper_lower(++middle, first+len, value);
            return pair<ForwardIterator, ForwardIterator>(left, right);
        }
    }
    //整个区间内都没有匹配的值,那么应该返回一对迭代器,指向第一个大于value的元素
    return pair<ForwardIterator, ForwardIterator>(first, first);
}

//inplace_merge(应用于有序空间)
template <class BidirectionalIterator>
inline void inplace_merge(BidirectionalIterator first, 
BidirectionalIterator middle, BidirectionalIterator last)
{
    //只要有任何一个序列为空,就什么都不必做
    if (first == middle || middle == last) return;
    __inplace_merge_aux(first, middle, last, value_type(first), distance_type(first));
}

template <class BidirectionalIterator, class T, class Distance>
inline void inplace_merge_aux(BidirectionalIterator first, 
BidirectionalIterator middle, BidirectionalIterator last, T*, Distance*)
{
    Distance len1 = 0;
    distance(first, middle, len1);
    Distance len2 = 0;
    distance(middle, last, len2);

    temporary_buffer<BidirectionalIterator, T> buf(first, last);
    if (buf.begin() == 0) { //内存配置失败
        __merge_without_buffer(first, middle, last, len1, len2);
    } else { //有暂时缓存区的情况下进行
        __merge_adaptive(first, middle, last, len1, len2, 
        buf.begin(), Distance(buf.size()));
    }
}

//辅助函数:有缓存区的情况下
template <class BidirectionalIterator, class Distance, class Pointer>
void __merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle,
BidirectionalIterator last, Distance len1, Distance len2, 
Pointer buffer, Distance buffer_size)
{
    if (len1 < len2 && len1 <= buffer_size) {
        Pointer end_buffer = copy(first, middle, buffer);
        merge(buffer, end_buffer, middle, last, first);
    } else if (len2 <= buffer_size) {
        Pointer end_buffer = copy(middle, last, buffer);
        __merge_backward(first, middle, buffer, end_buffer, last);
    } else {
        BidirectionalIterator first_cut = first;
        BidirectionalIterator second_cut = middle;

        Distance len11 = 0;
        Distance len22 = 0;
        if (len1 > len2) {
            len11 = len1/2;
            advance(first_cut, len11);
            second_cut = lower_bound(middle, last, *first_cut);
            distance(middle, second_cut, len22);
        } else {
            len22 = len2/2;
            advance(second_cut, len22);
            first_cut = upper_bound(first, middle, *second_cut);
            distance(first, first_cut, len11);
        }
        BidirectionalIterator new_middle = __rotate_adaptive(
            first_cut, middle, second_cut, len1 - len11, len22,
            buffer, buffer_size);
        //针对左段递归调用
        __merge_adaptvie(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
        //针对右段递归调用
        __merge_adaptvie(new_middle, second_cut, last, len1-len11, len2-len22, buffer, buffer_size);
    }
}

template <class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
BidirectionalIterator1 __rotate_adaptive(BidirectionalIterator1 first,
BidirectionalIterator1 middle, BidirectionalIterator last, Distance len1,
Distance len2, BidirectionalIterator2 buffer, Distance buffer_size)
{
    BidirectionalIterator2 buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {
        //缓存区足够安置序列二(较短)
        buffer_end = copy(middle, last, buffer);
        copy_backward(first, middle, last);
        return copy(buffer, buffer_end, first);
    } else if (len1 <= buffer_size) {
        //缓存区足够安置序列一
        buffer_end = copy(first, middle, buffer);
        copy(middle, last, first);
        return copy_backward(buffer, buffer_end, last);
    } else {
        //缓存区不足.改用rotate算法(不需要缓存区)
        rotate(first, middle, last);
        advance(first, len2);
        return first;
    }
}

//nth_element
//重复排列[first,last)使迭代器nth所指的元素,与整个序列完整排序后,
//同一位置的元素同值,此外并保证[nth,last)内任何一个元素不大于[first,nth)内元素

//version 1: operator<
template <class RandomAccessIterator, class T>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
RandomAccessIterator last)
{
    __nth_element(first, nth, last, value_type(first));
}

//辅助函数version 1
template <class RandomAccessIterator, class T>
void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
RandomAccessIterator last, T*)
{
    while (last - first > 3) {
        //采用media-of-tree partitioning
        //返回一个迭代器,指向分割后的右段的第一参数
        RandomAccessIterator cut = __unguarded_partition(
            first, last, T(__media(*first, 
                                   *(first+(last-first)/2),
                                   *(last-1))));
        if (cut <= nth)  //如果右段起点<=指定位置(nth落于右段)
            first = cut; //再对右段实施分割(partitioning)
        else             //否则(nth落于左段)
            last = cut;  //对左段实施分割(partitioning)
    }
    __insertion_sort(first, last);
}

//==== merge sort ======
/*
SGI STL所采用的排序法是IntroSort,不过另一个很有名的算法Merge Sort,
很轻易就可以利用STL算法inplace_merge实现出来.

既然我们知道,将两个有序区间归并成一个有序区间,效果不错,那么我们利用
"分而治之"的概念,以各个击破的方式来对一个区间进行排序.首先将区间对半分割
左右两段各自排序,再利用inplace_merge重新组合为一个完整的有序序列.对半分
割的操作可以递归进行,直到每一个小段的长度为0或1.

复杂度为O(nlogn),虽然这个quick-sort是一样的,但因为MergeSort需要额外
的内存,而且在内存之间移动(复制)数据也会耗费不少时间,所以MergeSort效率
比不上QuickSort.实现简单,概念简单是MergeSort的两大优点.
*/

template <class BidirectionalIter>
void mergesort(BidirectionalIter first, BidirectionalIter last)
{
    typename iterator_traits<BidirectionalIter>::difference_type n
    = distance(first, last);
    if (n == 0 || n == 1) 
        return;
    else {
        BidirectionalIter mid = first + n/2;
        mergesort(first, mid);
        mergesort(mid, last);
        inplace_merge(first, mid, last);
    }
}

#endif // SGI_STL_ALGO_H