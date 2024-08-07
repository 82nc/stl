#ifndef SGI_STL_HASH_FUN_H
#define SGI_STL_HASH_FUN_H

#include "01-config/stl_config.h"
#include <stddef.h>

template <class Key> 
struct hash { };

inline size_t __stl_hash_string(const char* s)
{
    unsigned long h = 0;
    for (; *s; ++s)
        h = 5 * h + *s;
    return size_t(h);
}

__STL_TEMPLATE_NULL struct hash<char*>
{
    size_t operator()(const char* s) const { return __stl_hash_string(s); }
}

__STL_TEMPLATE_NULL struct hash<const char*>
{
    size_t operator()(const char* s) const { return __stl_hash_string(s); }
}

__STL_TEMPLATE_NULL struct hash<char>
{
    size_t operator()(char x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<unsigned char>
{
    size_t operator()(unsigned char x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<signed char>
{
    size_t operator()(signed char x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<short>
{
    size_t operator()(short x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<unsigned short>
{
    size_t operator()(unsigned short x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<int>
{
    size_t operator()(int x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<unsigned int>
{
    size_t operator()(unsigned int x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<long>
{
    size_t operator()(long x) const { return x; }
}

__STL_TEMPLATE_NULL struct hash<unsigned long>
{
    size_t operator()(unsigned long x) const { return x; }
}

//由此观之: SGI hashtable无法处理上述所列各型别以外的元素如: string, double, float.
//欲处理这些型别,用户必须自行为他们定义hash function

#endif // SGI_STL_HASH_FUN_H