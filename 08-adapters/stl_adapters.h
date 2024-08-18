#ifndef SGI_STL_ADAPTERS_H
#define SGI_STL_ADAPTERS_H

/*
Adapter是一个设计模式
将一个class的接口转换为另一个class的接口,
使原本因接口不兼容而不能合作的classes可以一起运作

STL所提供的各种配接器:

一、改变仿函数接口者,我们称为function adaptive
其价值在于用过它们之间的绑定,组合,修饰能力,几乎可以无限地创造出
各种表达式,搭配STL算法一起演出,配接灵活度是最高的.
配接操作包括系结(bind),否定(negate),组合(compose)
以及对一般函数或成员函数的修饰(使其成为一个仿函数),C++standard规定这些
配接器的接口可由<functiona>获得,SGI STL将他们实际定义于<stl_function.h>

二、改变容器接口者,我们称为container adapter
1.queue
2.stack
他们修饰deque的接口而成就出另一个容器的风貌

三、改变迭代器接口者,我们称为iterator adapter
STL提供许多应用于迭代器身上的配接器
insert iterators: //将一般迭代器的赋值操作转变为插入操作
尾端插入: back_insert_iterator
    头部插入: front_insert_iterator
    任意插入: insert_iterator

reverse iterators: 可以将一般迭代器的行进方向逆转,
使原本应该前进的operator++变成后退操作,
使原本应该后退的operator--变成前进操作

iostream iterators: 可以将迭代器绑定到某个iostream对象上
绑定到istream对象称为istream_iterator
绑定到ostream对象称为ostream_iterator

C++Standard规定它们的接口可藉由<iterator>获得,SGI STL
则将他们实际定义于<stl_iterator.h>
*/

#endif // SGI_STL_ADAPTERS_H