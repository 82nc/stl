#ifndef SGI_STL_ALLOC_H
#define SGI_STL_ALLOC_H

#include "stl_alloc_x1.h"
#include "stl_alloc_x2.h"

#ifdef __USE_MALLOC
//....
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc; // 令alloc为第一级配置器
#else
//...
// 令alloc为第二级配置器
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;

#endif // ! __USE_MALLOC

template<class T, class Alloc>
class simple_alloc {
public:
    static T* allocate(size_t n)
        { return !n ? 0 : (T*) Alloc::allocate(n*szieof(T)); }
    static T *allocate(void)
        { return (T*) Alloc::allocate(sizeof(T)); }
    static void deallocate(T* p, size_t n)
        { if (n) Alloc::dealocate(p, n*sizeof(T)); }
    static void deallocate(T *p)
        { Alloc::deallocate(p, sizeof(T)); }
};


//sample: vector
template <class T, class Alloc = alloc> //缺省使用alloc为配置器
class vector {
protected:
    // 专属之空间配置器, 每次配置一个元素大小
    typedef simple_alloc<value_type, Alloc> data_allocator;

    void deallocate()
    {
        if (...)
            data_allocator::deallocate(start, end_of_storage - start);
    }
};


#endif // SGI_STL_ALLOC_H