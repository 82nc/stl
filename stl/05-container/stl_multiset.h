#ifndef SGI_STL_MULTI_SET_H
#define SGI_STL_MULTI_SET_H

#include "02-allocator/stl_alloc.h"

/*
 * multiset的特性以及用法和set完全相同,唯一的差别在于它允许键值重复
 * 因为它的插入操作采用的是底层机制RB-tree的insert_equal()而非insert_unique()
 * 下面是multiset的源代码提要,只列出与set的不同之处.
*/

template <class Key, class Compare = less<Key>, class Alloc = alloc>
class multiset {
public:
    // typedefs:
    //...(与set相同)

    // allocation/deallocation
    // 注意使用的是insert_equal()而不是insert_unique()

    template <class InputIterator>
    multiset(InputIterator first, InputIterator last) :
        : t(Compare()) { t.insert_equal(first, last); }
    
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last, const Compare& comp)
        : t(comp) { t.insert_equal(first, last); }
    
    //其他与set相同

    // insert/erase
    iterator insert(const value_type& x) { return t.insert_equal(x); }
    iterator insert(iterator position, const value_type& x) {
        typedef typename rep_type::iterator rep_iterator;
        return t.insert_equal((rep_iterator&)position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) { t.insert_equal(first, last); }
    // ...其他与set相同
};

#endif // SGI_STL_MULTI_SET_H