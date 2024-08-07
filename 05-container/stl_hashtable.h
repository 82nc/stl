#ifndef SGI_STL_HASH_TABLE_H
#define SGI_STL_HASH_TABLE_H

#include "01-config/stl_config.h"

#include "03-iterator/stl_iterator.h"
#include "02-allocator/stl_alloc.h"
#include "04-container/stl_vector.h"

template <class Value>
struct __hashtable_node
{
    __hashtable_node* next;
    Value val;
};

//declearation
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc=alloc>
class hashtable;

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Value& reference;
    typedef Value* pointer;

    node* cur;      //迭代器目前所指之节点
    hashtable* ht;  //保持对容器的连接关系(因为可能需要从bucket跳到bucket)

    __hashtable_iterator(node* n, hashtable* tab) : cur(t), ht(tab) {}
    __hashtable_iterator() {}
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }
    iterator& operator++();
    iterator operator++(int);
    //hashtable没有后退,逆向迭代器
    bool operator ==(const iterator& it) const { return cur == it->cur; }
    bool operator !=(const iterator& it) const { return cur != it->cur; }
};

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator ++()
{
    const node* old = cur;
    cur = cur->next;
    if (!cur) {
        size_type bucket = ht->bkt_num(old->val);
        while (!cur && ++bucket < ht->bucket.size()) //注意: operator++
            cur = ht->buckets[bucket];
    }
    return *this;
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator ++(int)
{
    iterator tmp = *this;
    ++*this; //调用operator++()
    return tmp;
}

//definition:
//Value:节点的实值类别, Key:节点的键值类别, HashFcn:hash function的函数类别
//ExtractKey:从节点中取出键值的方法(函数或者仿函数)
//EqualKey:判断键值相同与否的方案(函数或者仿函数)
//Alloc:空间配置器,缺省使用std::alloc
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable {
public:
    typedef HashFcn hasher;     //为模版类型参数重新定义一个名称
    typedef EqualKey key_equal; //为模版类型参数重新定义一个名称
    typedef size_t size_type;

    //以下三者都是function object. <stl_hash_fun.h>中定义数个
    //标准型别(如int,c-style string等)的hasher
    hasher hash;
    key_equal equals;
    ExtractKey get_key;

    typedef __hashtable_node<Value> node;
    typedef simple_alloc<node, Alloc> node_allocator;

    vector<node*, Alloc> buckets; //以vector完成
    size_type num_elements;
public:
    // bucket个数即bucket vector的大小
    size_type bucket_count() const { return buckets.size(); }
    //...

    node* new_node(const value_type& obj)
    {
        node* n = node_allocator::allocate();
        n->next = 0;
        __STL_TRY {
            construct(&n->val, obj)
            return n;
        }
        __STL_UNWIND(node_allocator::deallocate(n));
    }
    void delete_node(node* n)
    {
        destory(&n->val);
        node_allocator::deallocate(n);
    }
};

#endif // SGI_STL_HASH_TABLE_H