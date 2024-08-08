// 我们不赞成包含此文件. 这是原始的HP default alloctor.
// 提供它只是为了回溯兼容

// HERE JUST FOR COMPITABLE TRACE
// DO NOT USE THIS FILE


#ifndef SGI_STD_ALLOC_H
#define SGI_STD_ALLOC_H


// SGI专门定义的符合标准的名为alloctor配置器
// 但SGI从未使用过, 也不建议我们使用. 主要是效率不佳, 
// 只把C++的::operator new和::operator delete做了一层薄的封装


#include <new>       // for placement new
#include <stddef.h>  // for ptrdiff_t, size
#include <stdlib.h>  // for exit()
#include <limits.h>  // for UINT_MAX
#include <iostream>  // for cerr
#include <algobase.h>  

namespace SGI // SGI
{

// DO NOT USE THIS FILE 不要使用这个文件, 除非你手上的容器是旧式做法完成
// 那就需要一个拥有HP-style interface的空间配置器. SGI STL使用不同的allocator接口
// SGI-type allocators 不带有任何与对象型别有关的参数; 他们只响应void*指针(侯捷注: 
// 如果是标准接口的alloctor，就会响应一个“指向对象型别”的指针T*). 
// 此文件并不包含于其他任何SGI STL头文件

template <class T>
inline T* _allocate(ptrdiff_t size, T*) {
    std::set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (!tmp) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <class T>
inline void _deallocate(T* buffer) {
    ::operator delete(buffer);
}

template <class T1, class T2>
inline void _construct(T1* p, const T2& v) {
    new(p) T1(v); // placement new invoke ctor of T1
}

template <class T>
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
    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

// hint used for locallity. ref.[Austern], p189
pointer allocate(size_type n, const void* hint=0) {
    return _allocate((difference_type)n, (pointer)0);
}

void deallocate(pointer p, size_type n) { _deallocate(p); }

void construct(pointer p, const T& v) { _construct(p, v); }

void destory(pointer p) { _destory(p); }

pointer address(reference r) { return (pointer)&r; }

const_reference const_address(const_reference r) { return (const_reference)&r; }

size_type init_page_size() {
    return max(size_type(1), size_type(4096/sizeof(T)));
}

size_type max_size() const { 
    return max(size_type(1), size_type(UINT_MAX/sizeof(T)); 
}

};

// 特化版本
template <>
class allocator<void> {
public:
    typedef void* pointer;
};

} // namespace SGI
#endif // SGI_STD_ALLOC_H