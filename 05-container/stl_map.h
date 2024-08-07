#ifndef SGI_STL_MAP_H
#define SGI_STL_MAP_H

#include "05-container/stl_tree.h"

/*
 * 我们可以通过map的迭代器改变map的元素?
 * 如果是修正元素的键值那是不行的,跟set的同样的理由: 键值关系到元素的排列顺序
 * 如果是修正元素的实值那是可以的,因为元素的实值不影响map元素的排列顺序
 * map iterators既不是一种constant iterators, 也不是mutable iterators
 * 
 * map所有开放的操作接口,RB-tree也都提供了,所以几乎所有map的操作行为,都是转调RB-tree的操作行为
 * 
 * SGI STL map以红黑树为底层机制,每个节点的内容是一个pair<key, value>
 */

template <class Key, class T, class Comapre = less<Key>,        //缺省采用递增排序
          class Alloc = alloc>
class map {
public:
    // typedefs:
    typedef Key key_type;   //键值型别
    typedef T data_type;    //数据(实值)型别
    typedef T mapped_type;  //
    typedef pair<const Key, T> value_type; //元素型别(键值/实值)
    typedef Compare key_compare; //键值比较函数

    //以下定义一个functor,其作用是调用"元素比较函数"
    class value_compare : public binary_function<value_type, value_type, bool> {
    friend class map<Key, T, Compare, Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        bool operator()(const value_type& x, const value_type& y) const
        {
            return comp(x.first, y.first); 
        }
    };

private:
    //以下定义表述差别(representation type), 以map元素型别(一个pair)
    //的第一型别,作为RB-tree节点的键值类型
    typedef rb_tree<key_type, value_type, 
    select1st<value_type>, key_compare, Alloc> rep_type;
    rep_type t; //以红黑树表现map
public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::iterator iterator;
    //注意上一行,map并不像set一样将iterator定义为RB-tree的const_iterator
    //因为它允许用户通过其迭代器修改元素的实值(value)
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    //allocation/deallocation
    /* 
     * 注意map一定使用底层的RB-tree的insert_unique()而非insert_equal(),
     * multimap才使用insert_equal()
    */

    map() : t(Compare()) {}
    explicit map(const Compare& comp) : t(comp) {}

    template <class InputIterator>
    map(InputIterator first, InputIterator last) :
        t(Compare()) { t.insert_unique(first, last); }
    
    template <class InputIterator>
    map(InputIterator first, InputIterator last, const Comapre& comp) :
        t(comp) { t.insert_unique(first, last); }
    
    map(const map<Key, T, Compare, Alloc>& x) : t(x.t) {}

    map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Comapre, Alloc>& x) 
    { 
        t = x.t; 
        return *this; 
    }

    // accessors
    // 以下所有的map操作行为,RB-tree都已提供,map只要转调用即可

    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    reverse_iterator rbegin() { return t.rbegin(); }
    reverse_iterator rend() { return t.rend(); }
    const_reverse_iterator rbegin() { return t.rbegin(); }
    const_reverse_iterator rend() { return t.rend(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    //注意以下下标(subscript)操作符
    T& operator[] (const key_type& k) { 
        return (*((insert(value_type(k, T()))).first)).second;
        //根据键值与实值做出一个元素,因为实值未知,所以先产生一个与实值类型相同的临时对象
        //value_type(k, T()),再将该元素插入到map里,插入操作返回一个pair,其第一元素是个
        //迭代器,指向插入妥当的新元素,或指向插入失败(键值重复)的旧元素.
        //如果下标运算符作为左值运用(通常表示要添加新元素),我们正好以此"实值待填"的元素
        //位置卡好;如果下标操作符作为右值运用(通常表示要根据键值取其实值),此时的插入操作
        //所返回的pair的第一元素(是个迭代器)恰指向简直复合的旧元素.
    }
    void swap(mao<Key, T, Compare, Alloc>& x) { t.swap(x.t); }

    //insert/erase
    pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
    iterator insert(iterator position, cosnt value_type& x) { return t.insert_unique(position, x); }
    template <class InputIterator>
    void insert(InputIterattor first, InputIterator last) { t.insert_unique(first, last); }

    void erase(iterator position) { t.erase(position); }
    size_type erase(const key_type& x) { return t.erase(x); }
    void erase(iterator first, iterator last) { t.erase(first, last); }
    void clear() { t.clear(); }

    //map operations
    iterator find(cosnt key_type& x) { return t.find(x); }
    const_iterator find(cosnt key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) cosnt { return t.count(x); }
    iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
    const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
    const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
    pair<iterator, iterator> equal_range(const key_type& x) { return t.equal_range(x); } 
    pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

    friend bool operator== <> (const map&, const map&);
    friend bool operator<  <> (const map&, const map&);
};

template <class Key, class T, class Compare, class  Alloc>
inline bool operator== (const map<Key, Compare, Alloc>& x, const map<Key, Compare, Alloc>& y)
{
    return x.t == y.t;
}

template <class Key, class T, class Compare, class  Alloc>
inline bool operator< (const map<Key, Compare, Alloc>& x, const map<Key, Compare, Alloc>& y)
{
    return x.t < y.t;
}

#endif // SGI_STL_MAP_H

/*
 * ========= TEST DEMO ============

#include <map>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    map<string, int> simap;
    simap[string("jjhou")] = 1;
    simap[string("jerry")] = 2;
    simap[string("jason")] = 3;
    simap[string("jimmy")] = 4;

    pair<string, int> value(string("david"), 5);
    simap.insert(value);

    map<string, int>::iterator simap_iter = simap.begin();
    for (; simap_iter != simap.end(); ++simap_iter)
        cout << simap_iter->first << ' ' << simpa_iter->second << endl;
    // david 5
    // jason 3
    // jerry 2
    // jimmy 4
    // jjhou 1

    int number = simap[string("jjhou")];
    cout << number << endl;     // 1

    //面对关联容器,应使用其所提供的find函数来搜寻元素,会比STL算法find更有效率
    it1 = simap.find(string("mchen"));
    if (it1 == simap.end())
        cout << "mchen not found" << endl;  // mchen not found

    it1 = simap.find(string("jerry"));
        cout << "jerry found" << endl;      // jerry found
    
    it1->second = 9; //通过map迭代器修改value(not key)
    int number2 = simap[string("jerry")];
    cout << number2 << endl;
}

 */