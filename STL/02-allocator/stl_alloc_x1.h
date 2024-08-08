#ifndef SGI_STL_ALLOC_X1_H
#define SGI_STL_ALLOC_X1_H

#if 0
    #include <new>
    #define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
    #include <iostream> // <iostream.h>
    #define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1);
#endif

// malloc_based allocator 通常比default alloc慢
// 一般而言是thread-safe, 并且对于空间的运用比较高效
// 以下是第一级配置器
// 注意, 无template型别参数, 至于非型别参数inst, 则完全没用
template <int inst>
class __malloc_alloc_template {
private:
    // 以下都是函数指针, 所代表的函数将来用于处理内存不足的情况
    // oom: out of memory.
    static void *oom_malloc(size_t);
    static void *oom_realloc(void*, size_t);
    static void (* __malloc_alloc_oom_handler)();
public:
    static void* allocate(size_t)
    {
        void* result = malloc(n); // 第一级配置器直接用malloc()
        // 以下无法满足需求时, 改用oom_malloc(n);
        if (!result) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void*p, size_ /* n */)
    {
        free(p); // 第一级配置器直接使用free()
    }

    static void* reallocate(void* p, size_t /* old_sz */, szie_t new_sz)
    {
        void *result = realloc(p, new_sz); // 第一级配置器直接使用realloc()
        if (!result) result = oom_realloc(p, new_sz);
    }

    // 以下仿真C++的set_new_handler() 换句话说, 你可以通过它指定自己的
    // out-of-memeory handler
    static void (* set_malloc_handler(void(*f)()))()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return(old);
    }
};

// malloc_alloc out-of-memory handling
// 初始时0, 有待客户端设定
template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (* my_malloc_handler)();
    void *result;

    for (;;) { // 不断尝试释放、配置、再释放、在配置...
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (!my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)(); // 调用处理例程, 企图释放内存
        result = malloc();      // 再次尝试配置内存
        if (result) return result;
    }
}

template <int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
    void (* my_malloc_handler)();
    void* result;

    for (;;) { // 不断尝试释放、配置、再释放、在配置...
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (!my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)(); // 调用处理例程, 企图释放内存
        result = realloc(p, n); // 再次尝试配置内存
        if (result) return result;
    }
}

// 注意, 以下直接将参数inst指定为0
typedef __malloc_alloc_template<0> malloc_alloc;

#endif // SGI_STL_ALLOC_X1_H