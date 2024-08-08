#ifndef SGI_STL_CONSTRUCT_H

#include <new.h> // for placement new

template <class T1, class T2>
inline void construct(T1* p, const T2& v)
{
    new (p) T1(v); // placement new invoke T1::T1(v);
}

// 以下是destory()的第一个版本, 接受一个指针
template <class T1>
inline void destory(T1* p)
{
    p->~T1(); // invoke ~T1();
}

// 以下是destory的第二个版本, 接受两个迭代器. 此函数设法找出
// 元素的数值类别, 进而利用__type_traits<> 求取最适当措施。
template <class ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last)
{
    __destory(first, last, value_type(first));
}

// 判断元素的数值类别(value type)是否有trivial destructor
template <class ForwardIterator, class T>
inline void __destory(ForwardIterator first, ForwardIterator last, T* v)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destory_aux(first, last, trivial_destructor));
}

// 如果元素的数值类别(value type)有non-trivial destructor
template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first < last; ++first) {
        destory(&*first);
    }
}

// 如果元素的数值类别(value type)有trivial destructor
template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

// 以下是destory()第二版本对char*, wchar_t*的特化版
inline void destory(char*, char*) {}
inline void destory(wchar_t*, wchar_t*) {}

#define SGI_STL_CONSTRUCT_H
#endif // SGI_STL_CONSTRUCT_H