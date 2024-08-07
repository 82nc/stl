#ifndef SGI_STL_HASH_TABLE_H
#define SGI_STL_HASH_TABLE_H

#include "01-config/stl_config.h"

#include "03-iterator/stl_iterator.h"
#include "02-allocator/stl_alloc.h"
#include "04-container/stl_vector.h"
#include "05-container/stl_pair.h"

/* inner global functions
 */

static const int __stl_num_primes = 28;
//assert(sizeof(long)>=4);
static const unsigned long __stl_prime_list[__stl_num_primes] = {
    53,         97,           193,         389,       769,
    1543,       3079,         6151,        12289,     24593,
    49157,      98317,        196613,      393241,    786433,
    1572869,    3145739,      6291469,     12582917,  25165843,
    50331653,   100663319,    201326611,   402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n)
{
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = __stl_prime_list + __stl_num_primes;
    const unsigned long* pos = lower_bound(first, last, n);
    //lower_bound()泛型算法<第六章>
    return pos = last ? *(last - 1) : *pos;
}

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

    //以下三者都是function object. <stl_hash_fun.h>中定义数个标准型别(如int,c_str等)的hasher
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
    size_type max_buckets_count() const { return __stl_prime_list[__stl_num_primes-1]; }
    // ...

    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :
        hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) { initialize_buckets(n); }

    void clear();
    void copy_from(const hashtable& ht);
    iterator find(const key_type& key)
    {
        size_type n = bkt_num_key(key); //首先寻找落在哪一个bucket内
        node* first;
        //从bucket list的头开始,一一比对每个元素的键值.比对成功就跳出
        for (first=buckets[n]; first && !equals(get_key(first->val), key); 
             first = first->next) {
        }
        return iterator(first, this);
    }

    size_type count(const key_type& key) const
    {
        const size_type n = bkt_num_key(key); //首先寻找在哪一个bucket内
        size_type result = 0;
        //以下从bucket list的头开始,一一比对每个元素的键值.比对成功就累加1.
        for (const node* cur=buckets[n]; cur; cur=cur->next)
            if (equals(get_key(cur->val), key))
                ++result;
        return result;
    }

private:
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

    void initialize_buckets(size_type n)
    {`
        const size_type n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, (node*)0);
        num_elements = 0;
        //next_size()返回最接近n并大于n的质数
    }

    size_type next_size(size_type n) const { return __stl_next_prime(n); }

    //插入元素,不允许重复
    pair<iterator, bool> insert_unique(const value_type& obj)
    {
        resize(num_elements + 1); //判断是否需要重建表格,如果需要就扩充
        return insert_unique_noresize(obj);
    }
    //插入元素,允许重复
    iterator insert_equal(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }

    //判断是否需要重建,不需要的话立刻返回,需要就处理
    void resize(size_type num_elements_hint);
    //不需要重建的情况下插入新节点,键值不允许重复
    pair<iterator, bool> insert_unique_noresize(const value_type& obj);
    //不需要重建的情况下插入新节点,键值允许重复
    iterator insert_equal_noresize(const value_type& obj);

    //version 1: 接受实值(values)和buckets的个数
    size_type bkt_num(const value_type& obj, size_t n) const { return bkt_num_key(get_key(obj), n); }
    //version 2: 接受实值(value)
    size_type bkt_num(const value_type& obj) const { return bkt_num_key(get_key(obj)); }
    //version 3: 接受键值
    size_type bkt_num_key(const key_type& key) const { return bkt_num_key(key, buckets.size()); }
    //version 4: 接受键值和buckets的个数
    size_type bkt_num_key(const key_type& key, size_t n) const { return hash(key) % n; } 
    //hash: SGI的所有内建的hash列于5.7.7节
};

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear()
{
    //针对每一个bucket
    for (size_type i=0; i<buckets.size(); ++i) {
        node* cur = buckets[i];
        //将bucket list中的每个节点删除掉
        while (cur) {
            node* next = cur->next;
            delete_node(cur);
            cur = next;
        }
        buckets[i] = 0; //令bucket内容为null指针
    }
    num_elements = 0; //令总节点数为0
    //注意buckets vector并未释放掉空间,仍保留原来大小
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable &ht)
{
    //先清除己方的buckets vector.这个操作是调用vector::clear().造成所有元素为0
    buckets.clear();
    //为己方的buckets vector保留空间,是与对方相同
    //如果己方空间大于对方就不动,反之,就增大
    buckets.reserve(ht.buckets.size());
    //从己方的buckets vector尾端开始,插入n个元素,其值为null指针
    //注意此时buckets vector为空,所以所谓尾端就是起始处
    buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
    __STL_TRY {
        //针对buckets vector
        for (size_type i=0; i<ht.buckets.size(); ++i) {
            //复制vector的每一个元素(是指针,指向hashtable节点)
            if (const node* cur = ht.buckets[i]) {
                node* copy = new_node(cur->val);
                buckets[i] = copy;
                //针对同一个bucket list,复制每一个节点
                for (node* next=cur->next; next; cur=next, next=cur->next) {
                    copy->next = new_node(next->val);
                    copy = copy->next;
                }
            }
        }
        num_elements = ht.num_elements; //重新登记节点个数(hashtable的大小)
    }
    __STL_UNWIND(clear());
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    resize(size_type num_elements_hint)
{
    //以下表格重建的规则较为奇特: 拿元素个数(新增元素计入后)和buckets vector的大小比较,如果大于后者就重建
    //因此可判断每个bucket(list)的最大容量和buckets vector的大小相同.
    const size_type old_n = buckets.size();
    if (num_elements_hint > old_n) {
        const size_type n = next_size(num_elements_hint);
        if (n > old_n) {
            vector<node*, Alloc> tmp(n, (node*)0); //建立新的buckets
            __STL_TRY {
                //以下处理每一个旧的bucket
                for (size_type bucket=0; bucket < old_n; ++bucket) {
                    node* first = buckets[bucket]; //指向节点所对应之串行的起始节点
                    //以下处理每一个旧的bucket所含(串行)的每一个节点
                    while (first) {
                        //找出节点落在哪个新的bucket内
                        size_type new_bucket = bkt_num(first->val, n);
                        //以下四个操作颇为微妙
                        //[1] 令旧bucket指向其所对应之串行的下一个节点(以便迭代处理)
                        buckets[bucket] = first->next;
                        //[2][3] 将当前节点插入到新bucket内,成为对应串行的第一个节点
                        first->next = tmp[new_bucket];
                        tmp[new_bucket] = first;
                        //[4] 回到旧bucket所指的待处理串行,准备处理下一个节点
                        first = buckets[bucket]; 
                    }
                }
                buckets.swap(tmp); //vector::swap(): 新旧两个buckets对调
                //离开时释放local var tmp的内存
            }
        }
    }
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
insert_unique_noresize(const value_type &obj)
{
    const size_type n = bkt_num(obj); //决定obj应该位于的bucket
    node* first = buckets[n]; //令first指向buckets对应串行头部
    //如果buckets[n]被占用,此时first将不为0,于是进入以下循环
    //走过bucket所对应的整个链表
    for (node* cur=first; cur; cur=cur->next)
        if (equals(get_key(cur->val), get_key(obj))) //如果发现与链表中某键值相同,就不插入,立即返回
            return pair<iterator, bool>(iterator(cur, this), false);
    //离开以上循环(或根本进入循环)时,first指向bucket所指链表的头部节点
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp; //令新节点成为链表的第一个节点
    ++num_elements;
    return pair<iterator, bool>(iterator(tmp, this), true);
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
insert_equal_noresize(const value_type &obj)
{
    const size_type n = bkt_num(obj); //决定obj应位于#n bucket
    node* first = buckets[n]; //令first指向bucket对应之链表头部
    //如果buckets[n]已被占用,此时first将不为0,于是进入以下循环
    //走过bucket所对应的整个链表
    for (node* cur=first; cur; cur=cur->next)
        if (equals(get_key(cur->val), get_key(obj))) {
            //如果发现与链表中的某键相同,就马上插入,然后返回
            node* tmp = new_node(obj); //产生新的节点
            tmp->next = cur->next; //将新节点插入于目前位置
            cur->next = tmp;
            ++num_elements;
            return iterator(tmp, this); //返回迭代器指向新增节点
        }
    //进行至此,表示没有发现重复的键值
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    ++num_elemments;
    return iterator(tmp, this); //返回迭代器指向新增节点
}

#endif // SGI_STL_HASH_TABLE_H


/*
 * ========= TEST DEMO ============

//注意: 客户端程序不能直接含入<stl_hashtable.h>,应该含入有用到hashtable的容器头文件
//例如<hash_set.h>或者<hash_map.h>

#include <hash_set> //for instance
#include <iostream>
using namespace std;

int main()
{
    //hash-table
    //<value, key, hash-func, extract-key, equal-key, allocator>
    //note: hash-table has no default ctor
    hashtable<int,
              int,
              hash<int>,
              identity<int>,
              equal_to<int>,
              alloc> iht(50, hash<int>(), equal_to<int>()); //指定保留50个buckets
    cout << iht.size() << endl; //0
    cout << iht.bucket_count() << endl; //53, STL第一个质数
    cout << iht.max_bucket_count() << //4294967291, 这是STL最后一个质数

    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);
    iht.insert_unique(2);
    iht.insert_unique(53);
    iht.insert_unique(55);
    cout << iht.size() << endl; //6. 此即hashtable<T>::num_elements

    //以下声明一个hashtable迭代器
    hashtable<int,
              int,
              hash<int>,
              identity<int>,
              equal_to<int>,
              alloc>::iterator ite = iht.begin();
    //以迭代器遍历hashtable,将所有节点值打印出来
    for (int i=0; i<iht.size(); ++i, ++ite)
        cout << *ite << ' '; // 53 55 2 108 59 63
    cout << endl;

    //遍历所有buckets,如果其节点个数不为0,就打印节点个数
    for (int i=0; i<iht.bucket_count(); ++i) {
        int n = iht.elems_in_bucket(i);
        if (n != 0)
            cout << "bucket[" << i << "] has " << n << " elems." << endl;
    }
    // bucket[0] has 1 elems.
    // bucket[2] has 3 elems.
    // bucket[6] has 1 elems.
    // bucket[10] has 1 elems.

    //为了验证 "bucket(list)的容量就是buckets vector的大小"
    //(这是从hashtable<T>::resize()得知的结果), 我刻意将元素加到54个,
    //看看是否发生"表格重建(re-hashing)"
    for (int i=0; i<47; i++)
        iht.insert_equal(i);
    cout << iht.size() << endl; //54. 元素节点的个数
    cout << iht.bucket_count() << endl; //97. buckets个数
    //遍历所有buckets,如果其节点个数不为0,就打印节点个数
    for (int i=0; i<iht.bucket_count(); ++i) {
        int n = iht.elems_in_bucket(i);
        if (n != 0)
            cout << "bucket[" << i << "] has " << n << " elems." << endl;
    }
    //打印结果: bucket[12]和bucket[11]的节点个数为2,
    //其余的bucket[0]~bucket[47]的节点个数均为1
    //此外bucket[53],[55],[59],[63]的节点个数均为1

    //以迭代器遍历hashtable,将所有节点的值打印出来
    ite = iht.begin();
    for (int i=0; i<iht.size(); ++i, ++ite)
        cout << *ite << ' '; //
    cout << endl;
    // 0  1  2  2  3  4  5  6  7  8  9  10 11 108 12 13 14 15 16 17 18 19 20 21
    // 22 23 24 25 26 27 28 29 30 31 32 33 34 35  36 37 38 39 40 41 42 43 44 45
    // 46 47 53 55 59 63

    cout << *(iht.find(2)) << endl; // 2
    cout << iht.count(2) << endl;   // 2
}

 */