#ifndef JIE_ALLOC_H
#define JIE_ALLOC_H


// 按照STL标准规范, 以下是alloctor接口定义
/*
alloctor::value_type
alloctor::pointer
alloctor::const_pointer
alloctor::reference
alloctor::const_reference
alloctor::size_type
alloctor::difference_type

// 嵌套类模板, 拥有唯一的成员other(typedef): alloctor<U>
alloator::rebind 

alloctor::allocator() // default constructor
alloctor::alloctor(const alloctor&) // copy constructor
template <class U>alloctor::allocator(const allocator<U>&) // 泛化的constructor
allocator::~allocator() // default destructor
pointer allocator::address(reference x) const // 返回某个对象的地址
const_pointer allocator::address(const_reference x) const // 返回某个const对象的地址

// 配置空间, 足以存储n个T对象, 第二个参数是提示, 
// 实现上可能会利用它来增进区域性(locality), 或完全忽略之
pointer allocator::allocate(size_type n, const void*=0)

void allocator::deallocate(pointer p, size_type n) // 归还先前配置的空间

size_type allocator::max_size() const // 返回可成功配置的最大值

void allocator::construct(pointer p, const T& x) // same as: new((const void*) p) T(x)

void allocator::destory(pointer p) // same as: p->~T()
*/

#include <new>      // for placement new
#include <cstddef>  // for ptrdiff_t, size
#include <cstdlib>  // for exit()
#include <climits>  // for UINT_MAX
#include <iostream> // for cerr


/* 题外话: what does Foo* p = new Foo() do?

1. Foo* p;
2. void* raw = operator::new(sizeof(Foo));
3. 
   try {
        p = new(raw) Foo();
   } catch { // constructor threw an exception
        operator delete(raw);
        throw();
   }

*/

namespace JJ {

template <typename T>
inline T* _allocate(ptrdiff_t size, T*) 
{
    std::set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (!tmp) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <typename T>
inline void _deallocate(T* buffer)
{
    ::operator delete(buffer);
}

template <typename T1, typename T2>
inline void _construct(T1* p, const T2& v)
{
    new(p) T1(v); // placement new invoke ctor of T1
}

// 以下是设计简单的一个alloctor
template <typename T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    // rebind allocator of Type U
    template <typename U>
    struct rebind { 
        typedef allocator<U> other; 
    };

// hint used for locallity. ref.[Austern], p189
pointer allocate(size_type n, const void* hint=0) 
{
    return _allocate((difference_type)n, (pointer)0);
}

void deallocate(pointer p, size_type n) { _deallocate(p); }

void construct(pointer p, const T& v) { _construct(p, v); }

void destory(pointer p) { _destory(p); }

pointer address(reference r) { return (pointer)&r; }

const_reference const_address(const_reference r) { return (const_reference)&r; }

size_type max_size() const { return size_type(UINT_MAX/sizeof(T)); }

};

} // namespace JJ
#endif // JIE_ALLOC_H