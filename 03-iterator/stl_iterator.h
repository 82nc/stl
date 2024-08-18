#ifndef SGI_STL_ITERATOR_H
#define SGI_STL_ITERATOR_H

#include <stddef.h>

// 节选自SGI STL <stl_iterator.h>
// 五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 为了避免写代码挂一漏万, 自行开发的迭代器最好继承自下面这个std::iterator
template <class Category, class T, class Distance=ptrdiff_t, class Pointer=T*, class Reference=T&>
struct iterator {
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

// 萃取机 traits
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;   
};

// 针对原生指针(native pointer)而设计traits偏特化
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

// 针对原生指针pointer-to-const而设计traits偏特化
template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
};

// 这个函数可以很方便地决定某个迭代器的类型
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

// 这个函数可以很方便地决定某个迭代器的distance type
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 这个函数可以很方便地决定某个迭代器的value type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 以下是整组的distance函数
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
    iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    typedef typename Iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
}

// 以下是整组advance函数
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--) ++i;
}

template <class BidirectoionalIterator, class Distance>
inline void __advance(BidirectoionalIterator& i, Distance n, bidirectional_iterator_tag) {
    if (n >= 0)
        while (n--) ++i;
    else
        while (n++) --i;
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    __advance(i, n, iterator_category(i));
}

//===================================
//======= iterator adapters =========
template <class Container>
class back_insert_iterator {
protected:
    Container* container;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    //下面这个ctor使back_insert_iterator于容器绑定起来
    explicit back_insert_iterator(Container& x) : container(&x) {}
    back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->push_back(value); //这里调用push_back
        return *this;
    }

    //以下三个操作符对back_insert_iterator不起作用(关闭功能)
    //三个操作符返回的都是back_insert_iterator自己
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
    back_insert_iterator<Container>& operator++(int) { return *this; }
};

//这是一个辅助函数,帮助我们方便使用back_insert_iterator
template <class Container>
inline back_insert_iterator<Container> back_inserter(Container& x)
{
    return back_insert_iterator<Container>(x);
}

//-------------------------------------------
//这是一个迭代器配接器(iterator adapter)用来将某个迭代器的赋值(assign)
//操作修改为插入(insert)操作---从容器的头端插入进行(所以称为front_insert)
//注意该迭代器不适用于vector,因为vector没有提供push_front函数
template <class Container>
class front_insert_iterator {
protected:
    Container* container;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                different_type;
    typedef void                pointer;
    typedef void                reference;

    explicit front_insert_iterator(Container& x) : container(&x) {}
    front_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->push_front(value);
        return *this;
    }

    //以下三个操作符对back_insert_iterator不起作用(关闭功能)
    //三个操作符返回的都是back_insert_iterator自己
    front_insert_iterator<Container>& operator*() { return *this; }
    front_insert_iterator<Container>& operator++() { return *this; }
    front_insert_iterator<Container>& operator++(int) { return *this; }
};

//这是一个辅助函数,帮助我们方便使用front_insert_iterator
template <class Container>
inline front_insert_iterator<Container> front_inserter(Container& x)
{
    return front_insert_iterator<Container>(x);
}

//-------------------------------------------
//这是一个迭代器配接器(iterator adapter)用来将某个迭代器的赋值(assign)
//操作修改为插入(insert)操作---在指定位置上进行,并将迭代器右移一个位置
//如此方便地连续执行"表面上是赋值(覆盖)而实际上是插入"地操作
template <class Container>
class insert_iterator {
protected:
    Container* container;
    typename Container::iterator iter;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    insert_iterator(Container& x, typename Container::iterator i)
        : container(&x), iter(i) {}
    insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }

    //以下三个操作符对insert_iterator不起作用(关闭功能)
    //三个操作符返回的都是insert_iterator自己
    insert_iterator<Container>& operator*() { return *this; }
    insert_iterator<Container>& operator++() { return *this; }
    insert_iterator<Container>& operator++(int) { return *this; }
};

//这是一个辅助函数,帮助我们方便使用insert_iterator
template <class Container, class Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator i)
{
    typedef typename Container::iterator iter;
    return insert_iterator<Container>(x, iter(i));
}

/*

#include <iterator> //for iterator adapters
#include <deque>
#include <algorithm> // for copy
#include <iostream>

using namespace std;

int main()
{
    //outite绑定到cout,每次对outite指派一个元素,然后就一个" "
    ostream_iterator<int> outite(cout, " ");

    int ia[] = { 0, 1, 2, 3, 4, 5 };
    deque<int> id(ia, ia+6);

    //将所有元素拷贝到outite(那么也就是拷贝到cout)
    copy(id.begin(), id.end(), outite); //输出0 1 2 3 4 5
    cout << endl;

    //将ia[]地部分元素拷贝到id内,使用front_insert_iterator
    //注意front_insert_iterator会将assign操作改为push_front操作
    //vector不支持push_front(),这是本例不以vector为示范对象的原因
    copy(ia+1, ia+2, front_inserter(id));
    copy(id.begin(), id.end(), outite); //1 0 1 2 3 4 5
    cout << endl;

    //将ia[]地部分元素拷贝到id内,使用back_insert_iterator
    copy(ia+3, ia+4, back_inserter(id));
    copy(id.begin(), id.end(), outite); //1 0 1 2 3 4 5 3
    cout << endl;

    //搜寻元素5所在位置
    deque<int>::iterator ite = find(id.begin(), id.end(), 5);
    //将ia[]地部分元素拷贝到id内,使用insert_iterator
    copy(ia+0, ia+3, inserter(id, ite));
    copy(id.begin(), id.end(), outite); //1 0 1 2 3 4 0 1 2 5 3
    cout << endl;

    //将所有元素逆向拷贝到outite
    //rbegin()和rend()与reverse_iterator有关
    copy(id.rbegin(), id.rend(), outite); //3 5 2 1 0 4 3 2 1 0 1
    cout << endl;

    //以下将inite绑定到cin,将元素拷贝inite,直到eos出现
    istream_iterator<int> inite(cin), eos; //eos: end-of-stream
    copy(inite, eos, inserter(id, id.begin()));
    copy(id.begin(), id.end(), outite); //32 26 99 1 0 1 2 3 4 0 1 2 5 3
}




*/

#endif // SGI_STL_ITERATOR_H