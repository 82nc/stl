#ifndef SGI_STL_SET_H
#define SGI_STL_SET_H

#include "02-allocator/stl_alloc.h"
#include "05-container/stl_tree.h"

template <class Key, class Compare = less<Key>, //缺省情况下采用递增排序
          class Alloc = alloc>
class set {
public:
    //typedefs:
    typedef Key key_type;
    typedef Key value_type;
    //注意:以下key_compare合value_compare使用同一个比较函数
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    //注意:以下identity定义于<stl_function.h>参见第7章, 其定义为
    /*
    template <class T>
    struct identity : public unary_function<T, T> {
        const T& operator()(const T& x) cosnt { return x; }
    }; 
    */

    typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
    rep_type t; //采用红黑树来表现set
public:
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    /* 注意上一行iterator定义为RB-tree的const_iterator,
       这表示set的迭代器无法执行写入操作.这是因为set元素有一定的次序安排,
       不允许用户在任意处进行写入操作
    */
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::const_reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    /* allocator/deallocator
     * 注意set一定使用RB-tree的insert_unique()而非insert_equal()
     * multiset才使用RB-tree的insert_equal()
     * 因为set不允许相同键值存在, multiset才允许相同键值存在
    */
    set() : t(Compare()) {}
    explicit set(const Compare& comp) : t(comp) {}
    template <class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp) : 
        t(comp) { t.insert_unique(first, last); }
    set(const set<Key, Compare, Alloc>& x) : t(x.t) {}
    set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) { t = x.t; return *this; }

    //以下所有set的操作行为,RB-tree都已提供,所以set只要传递调用即可
    
    //accessors
    key_compare key_comp() const { return t.key_comp(); }
    //注意以下set的value_comp()事实上为RB-tree的key_comp()
    value_compare value_comp() const { return t.begin(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() const { return t.rend(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

    //insert/erase
    typedef pair<iterator, bool> pair_iterator;
    pair<iterator, bool> insert(const value_type& x) 
    {
        pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
        return pair<iterator, bool>(p.first, p.second);
    }
    iterator insert(iterator position, const value_type& x)
    {
        typedef typename rep_type::iterator rep_iterator;
        return t.insert_unique((rep_iterator&)position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) 
    {
        t.insert_unique(first, last);
    }
    void erase(iterator position)
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)position);
    }
    size_type erase(const key_type& x) { return t.erase(x); }
    void erase(iterator first, iterator last) 
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)first, (rep_iterator&)last);
    }
    void clear() { t.clear(); }

    //set operations:
    iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
    pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }
    //以下__STL_NULL_TMPL_ARGS被定义为<>,详见1.9.1节
    #define __STL_NULL_TMPL_ARGS <> //zyw因未添加第一章配置,此处定义该宏
    friend bool operator== __STL_NULL_TMPL_ARGS(const set&, const set&);
    friend bool operator< __STL_NULL_TMPL_ARGS(const set&, const set&);
};

template <class Key, class Compare, class Alloc>
inline bool operator== (const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y)
{
    return x.t == y.t;
}

template <class Key, class Compare, class Alloc>
inline bool operator< (const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y)
{
    return x.t < y.t;
}

#endif //SGI_STL_SET_H


/*
 * test demo
#include <set>
#include <iostream>

using namespace std;

int main()
{
    int i;
    int i1[5] = { 0, 1, 2, 3, 4 };
    set<int> iset(ia, ia+5);

    cout << "size=" << iset.size() << endl;             // size=5
    cout << "3 count=" << iset.count(3) << endl;        // 3 count=1
    iset.insert(3);
    cout << "size=" << iset.size() << endl;             // size=5
    cout << "3 count=" << iset.count(3) << endl;        // 3 count=1
    iset.insert(5);
    cout << "size=" << iset.size() << endl;             // size=6
    cout << "3 count=" << iset.count(3) << endl;        // 3 count=1
    iset.erase(1);
    cout << "size=" << iset.size() << endl;             // size=5
    cout << "3 count=" << iset.count(3) << endl;        // 3 count=1
    cout << "1 count=" << iset.count(1) << endl;        // 1 count=0

    set<int>::iterator it1 = iset.begin();
    set<int>::iterator it2 = iset.end();
    for (; it1 != it2; ++it1)
        cout << *it1;   // 0 2 3 4 5
    cout << endl;

    //使用STL算法find()来搜寻元素,可以有效运作,但不是好办法
    it1 = find(iset.begin(), iset.end(), 3);
    if (it1 != iset.end())
        cout << "3 found" << endl;          // 3 found
    it1 = find(iset.begin(), iset.end(), 1);
    if (it1 == iset.end())
        cout << "1 not found" << endl;      // 1 not found

    //面对关联容器,应该使用其做提供的find函数来搜寻元素,会比使用STL算法更有效率.
    //因为STL算法只是循环搜寻
    it1 = iset.find(3);
    if (it1 != iset.end())
        cout << "3 found" << endl;          //3 found
    
    it1 = iset.find(1);
    if (it1 == iset.end())
        cout << "1 found" << endl;          //1 not found
    
    //企图通过迭代器来改变set的元素,是不被允许的
    *it1 = 9; // error, assignment of read-only location
}
*/
