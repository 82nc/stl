#ifndef SGI_STL_QUEUE_H
#define SGI_STL_QUEUE_H

#include "stl_deque.h"

template <class T, class Sequence = deque<T>>
class queue {
    //以下的__STL_NULL_TMPL_ARGS会展开为<>,见1.9.1节
    friend bool operator== __STL_NULL_TMPL_ARGS(const queue& x, const queue& y);
    friend bool operator< __STL_NULL_TMPL_ARGS(const queue& x, const queue& y);
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c; //底层容器
public:
    //以下完全利用Sequence c的操作, 完成queue的操作
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    //deque是两头可进出,queue是末端进,前端出(所以先进者先出)
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};

template <class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
{
    return x.c == y.c;
}

template <class T, class Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
{
    return x.c < y.c;
}

//queue不提供遍历功能, 也不提供迭代器
//底层容器也可以是list

#endif // SGI_STL_QUEUE_H