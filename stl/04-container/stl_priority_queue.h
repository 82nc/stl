#ifndef SGI_STL_PRIORITY_QUEUE_H
#define SGI_STL_PRIORITY_QUEUE_H

#include "stl_vector.h"

template <class T, class Sequeue = vector<T>, class Compare = less<typename Sequence::value_type>>
class priority_queue {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c; //底层元素
    Compare comp; //元素比较大小标准
public:
    priority_queue() : c() {}
    explicit priority_queue(const Compare& x) : c(), comp(x) {}

    //以下用到的make_heap(), push_heap(), pop_heap()都是泛型算法
    //注意:任何一个构造函数都立刻于底层容器内产生一个implicit representation heap
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const Compare& x)
        : c(first, last), comp(x) { make_heap(c.begin(), c.end(), comp); }

    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last)
        : c(first, last) { make_heap(c.begin(), c.end(), comp); }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    void push(const value_type& x) {
        __STL_TRY {
            //push_heap是泛型算法,先利用底层容器的push_back()将元素压入末端,再重排heap
            c.push_back(x);
            push_heap(c.begin(), c.end(), comp);
        }
        __STL_UNWIND(c.clear());
    }

    void pop() {
        __STL_TRY {
            //pop_heap()是泛型算法,从heap内取出一个元素.它并不是真正将元素弹出
            //而是重排heap,然后再以底层容器的pop_back()取得被弹出的元素
            pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
        __STL_UNWIND(c.clear());
    }
};

// priority_queue 不提供遍历功能, 不提供迭代器

#endif // SGI_STL_PRIORITY_QUEUE_H