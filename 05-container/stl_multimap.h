#ifndef SGI_STL_MULTI_MAP_H
#define SGI_STL_MULTI_MAP_H

#include "02-allocator/stl_alloc.h"

/*
 * multimap的特性以及用法和map完全相同,唯一的差别在于它允许键值重复
 * 因为它的插入操作采用的是底层机制RB-tree的insert_equal()而非insert_unique()
 * 下面是multimap的源代码提要,只列出与map的不同之处.
*/

template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class multimap {
public:
    // typedefs
    // ...(与map相同)

    // allocation/deallocation
    // 注意使用的是insert_equal()而不是insert_unique()

    template <class InputIterator>
    multimap(InputIterator first, InputIterator last) :
        : t(Compare()) { t.insert_equal(first, last); }
    
    template <class InputIterator>
    multimap(InputIterator first, InputIterator last, const Compare& comp)
        : t(comp) { t.insert_equal(first, last); }

    // 其他与map相同

    // insert/erase
    iterator insert(const value_type& x) { return t.insert_equal(x); }
    iterator insert(iterator position, const value_type& x) {
        return t.insert_equal(position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) { t.insert_equal(first, last); }
    // ...其他与map相同
};

#endif //SGI_STL_MULTI_MAP_H