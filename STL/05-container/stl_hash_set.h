#ifndef SGI_STL_HASH_SET_H
#define SGI_STL_HASH_SET_H

#include "02-allocator/stl_alloc.h"
#include "05-container/stl_hash_fun.h"
#include "05-container/stl_hashtable.h"

//hash set接口转调hashtable接口
//RB-tree有自动排序功能而hashtable没有,所以set元素有自动排序而hash_set没有
//hash_set的使用方式与set完全相同

template <class Value,
          class HashFcn = hash<Value>,
          class EqualKey = equal_to<Value>,
          class Alloc = alloc>
class hash_set {
private:
    //以下使用的identity<>定义于<stl_function.h>中
    typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
    ht rep; //底层机制以hash table完成
public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_reference reference;
    typedef typename ht::const_reference const_reference;

    typedef typename ht::const_iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }
public:
    // 缺省使用大小100的表格,将被hash table调整为最接近且较大之质数
    hash_set() : rep(100, hasher(), key_equal()) {}
    explicit hash_set(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_set(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
    hash_set(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) {}

    // 以下插入操作全部使用insert_unique(),不允许键值重复
    template <class InputIterator>
    hash_set(InputIterator f, InputIterator l) : 
        rep(100, hasher(), key_equal()) { rep.insert_unique(f, l); }
    
    template <class InputIterator>
    hash_set(InputItertor f, InputIterator l, size_type n) :
        rep(n, hasher(), key_equal()) { rep.insert_unique(f, l); }

    template <class InputIterator>
    hash_set(InputItertor f, InputIterator l, size_type n, const hasher& hf) :
        rep(n, hf, key_equal()) { rep.insert_unique(f, l); }
    
    template <class InuputIterator>
    hash_set(InuputIterator f, InuputIterator l, size_type n,
             const hasher& hf, const key_equal& eql) :
        rep(n, hf, eql) { rep.insert_equal(f, l); }
public:
    //所有操作几乎都有hash table对应版本,传递调用就行
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_set& hs) { rep.swap(hs.rep); }
    friend bool operator ==__STL_NULL_TMPL_ARGS (const hash_set&, const hash_set&);

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }
public:
    pair<iterator, bool> insert(const value_type& obj)
    {
        pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
        return pair<iterator, bool>(p.first, p.second);
    }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l) { rep.insert_unique(f, l); }

    pair<iterator, bool> insert_noresize(const value_type& obj)
    {
        pair<typename ht::iterator, bool> p = rep.insert_unique_noresize(obj);
        return pair<iterator, bool>(p.first, p.second);
    }

    iterator find(const key_type& key) const { return rep.find(key); }
    size_type count(const key_type& key) const { return rep.count(key); }
    pair<iterator, iterator> equal_range(const key_type& key) { return rep.equal_range(key); }
    size_type erase(const key_type& key) { rep.erase(key); }
    void erase(iterator i) { rep.erase(i); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }
public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_buckets_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator ==(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1, 
const hash_set<Value, HashFcn, EqualKey, Alloc>& hs2)
{
    return hs1.rep == hs2.rep;
}

#endif // SGI_STL_HASH_SET_H

/* 
 * ========== TEST DEMO ==========

#include <iostream>
#include <hash_set>
#include <cstring>

using namespace std;

struct eqstr {
    bool operator()(const char* s1, const char* s2) const { return strcmp(s1, s2); }
};

void lookup(const hash_set<const char*, hash<const char*>, eqstr>& set, const char* word)
{
    hash_set<const char*, hash<const char*>, eqstr>::const_iterator it = set.find(word);
    cout << " " << word << ": " << (it != set.end() ? "present" : "not present") << endl;
}

int main()
{
    hash_set<const char*, hash<const char*>, eqstr> set;
    set.insert("kiwi");
    set.insert("plum");
    set.insert("apple");
    set.insert("mango");
    set.insert("apricot");
    set.insert("banana");

    lookup(set, "mango"); // mango: present
    lookup(set, "apple"); // apple: present
    lookup(set, "durian"); // durian: not present

    hash_set<const char*, hash<const char*>, eqstr>::iterator c = set.begin();
    hash_set<const char*, hash<const char*>, eqstr>::iterator e = set.end();
    for (; c != e; ++c)
        cout << *c << ' '; //banana plum mango apple kiwi apricot

    {
        //以上例子不好证明hash_set没有排序,下面以int作为元素验证
        hash_set<int> set;
        set.insert(59);
        set.insert(63);
        set.insert(108);
        set.insert(2);
        set.insert(53);
        set.insert(55);

        hash_set<int>::iterator c = set.begin();
        hash_set<int>::iterator e = set.end();
        for (; c != e; ++c)
            cout << *c << ' '; //2 53 55 59 63 108
        cout << endl;

        //这里输出的结果有序是假象,因为默认hashtable的大小100,所以内部采用了质数193
        //这样193以内的数值hash后仍旧与原数据一致,所以仍旧有序.
    }
}

 */