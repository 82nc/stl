#ifndef SGI_STL_UNINITIALIZED_H
#define SGI_STL_UNINITIALIZED_H

#include "02-allocator/stl_construct.h"
#include <string.h>

template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, 
ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
    // 利用value_type萃取出result的value_type
}

// 针对char*和wchar_t*两种类型, 可以采用最具效率的memmove(直接移动内容)来执行复制行为,
// 因此SGI得以为这两种类型设计一份特化版本

// 以下是针对const char*特化版本
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

// 以下是针对const wchar_t*的特化版本
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, 
ForwardIterator result, T*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, n, x, is_POD());
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, 
ForwardIterator result, __true_type)
{
    return copy(first, last, result); // STL算法批量处理
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, 
ForwardIterator result, __false_type)
{
    ForwardIterator cur = first;
    // 为了阅读方便, 省略了异常处理
    for (; first != last; ++first, ++cur)
        construct(&*cur, *first); // 必须一个个构造，不能批量处理
    return cur;
}

// ************************************* /
template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
    return __uninitialized_fill(first, last, x, value_type(first));
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T*)
{
   typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
    fill(first, last, x); // STL算法
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
    ForwardIterator cur = first;
    // 为了阅读方便, 省略了异常处理
    for (; cur != last; ++cur)
        construct(&*cur, x); // 必须一个个构造，不能批量处理
}


// ************************************* /
template <class ForwardIterator, class Size, class T>
inline void uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, value_type(first));
    // 利用value_type萃取出first的value_type
}

template <class ForwardIterator, class Size, class T>
inline void __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
// POD_type
template <class ForwardIterator, class Size, class T>
inline void __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
    return fill_n(first, n, x); // 调用高阶函数
}

// Not POD_type
template <class ForwardIterator, class Size, class T>
inline void __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
    ForwardIterator cur = first;
    // 为了阅读方便, 省略了异常处理
    for (; n>0; --n, ++cur)
        construct(&*cur, x);
    return cur;
}

#endif // SGI_STL_UNINITIALIZED_H