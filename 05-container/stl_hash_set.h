#ifndef SGI_STL_HASH_SET_H
#define SGI_STL_HASH_SET_H

#include "02-allocator/stl_alloc.h"

//hash set接口转调hashtable接口
//因RB-tree有自动排序功能而hashtable没有,所以set元素有自动排序而hash_set没有
//hash_set的使用方式与set完全相同

template <class Value,
          class HashFcn = hash<Value>,
          class EqualKey = equal_to<Value>,
          class Alloc = alloc>
class hash_set {
private:
    //以下使用
};




#endif // SGI_STL_HASH_SET_H