#ifndef SGI_STL_DEQUE_H
#define SGI_STL_DEQUE_H

#include "02-allocator/stl_alloc.h"
#include "03-iterator/stl_iterator.h"


//每个节点缓存区的元素个数
inline size_t __deque_buf_size(size_t n, size_t sz)
{
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator { //未继承 std::iterator
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;

    static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

    // 未继承std::iterator, 所以必须自行撰写五个迭代器相应类别(第三章)
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;

    typedef __deque_iterator self;

    //保持与容器的关联
    T* cur;   //此迭代器所指缓存区的当前元素
    T* first; //此迭代器所指缓存区的头
    T* last;  //此迭代器所指缓存区的尾
    map_pointer node; //该缓存区域地址在管控中心的地址

    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }

    //以下各重载运算子是__deque_iterator<>成功运作的关键
    reference operator*() const { return *cur; }
    pointer operator->() const { return &(operator*()); }
    difference_type operator-(const self& x) const 
    {
        return difference_type(buffer_size()) * (node - x.node - 1) + 
        (cur - first) + (x.last - x.cur);
    }

    self& operator++()
    {
        ++cur;
        if (cur == last) {
            set_node(node + 1); //如果已到达所在缓存区的尾端
            cur = first;
        }
        return *this;
    }
    self operator++(int) 
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self operator--()
    {
        if (cur == first) {     //如果已达所在缓存区的头端,
            set_node(node -1);  //就切换至前一节点(亦即缓存区)
            cur = last;         //的最后一个元素
        }
        --cur;                  //切换至前一个元素
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
    //以下实现随机存取. 迭代器可以直接跳跃n个距离
    self& operator+=(difference_type n)
    {
        difference_type offset = n + (cur - first);
        if (offset >= 0 && offset < difference_type(buffer_size())) //目标位置在同一缓存区
            cur += n; 
        else { //目标位置不在同一缓存区
            difference_type node_offset = offset > 0 ? 
                offset / difference_type(buffer_size()) :
                -difference_type((-offset - 1) / buffer_size()) - 1;
            //切换至正确节点(亦即缓存区)
            set_node(node + node_offset);
            //切换至正确的元素
            cur = first + (offset - node_offset * difference_type(buffer_size));
        }
        return *this;
    }

    self& operator+=(difference_type n) const
    {
        self tmp = *this;
        return tmp += n; //调用operator+=
    }

    self& operator-=(difference_type n) { return *this += -n; }
    self operator-(difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }
    //调用operator*, operator+
    reference operator[](difference_type n) const { return *(*this + n); }
    bool operator==(const self& x) const { return cur == x.cur; }
    bool operator!=(const self& x) const { return !(*this == x); }
    bool operator<(const self& x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
};

template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque {
public:
    typedef T                                   value_type;
    typedef value_type*                         pointer;
    typedef size_t                              size_type;
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
protected:
    typedef ptrdiff_t   difference_type;
    //元素的指针的指针
    typedef pointer*    map_pointer; //T** same as __deque_iterator::map_pointer
    //专属之空间配置器, 每次配置一个元素大小
    typedef simple_alloc<value_type, Alloc> data_allocator;
    //专属之空间配置器, 每次配置一个指针大小
    typedef simple_alloc<pointer, Alloc> map_allocator;
protected:
    iterator    start;      //表现第一个节点
    iterator    finish;     //表现最后个节点
    map_pointer map;        //指向map(一块连续空间, 其内的每个元素都是一个指针(节点)指向一块缓存区)
    size_type   map_size;

    void fill_initialize(size_type n, const value_type& value);
    void create_map_and_nodes(size_type num_elements);
    void reserve_map_at_back(size_type nodes_to_add = 1)
    {
        if (nodes_to_add + 1 > map_size - (finsih.node - map)) {
            //如果map尾端的节点备用空间不足
            //符合以上条件则必须重换一个map(配置更大的,拷贝原来的,释放原来的)
            reallocate_map(nodes_to_add, false);
        }
    }
    void reserve_map_at_front(size_type nodes_to_add = 1)
    {
        if (nodes_to_add > start.node - map) {
            //如果map前端的节点备用空间不足
            //符合以上条件则必须重换一个map(配置更大的, 拷贝原来的, 释放原来的)
            reallocate_map(nodes_to_add, true);
        }
    }
    void reallocate_map(size_type nodes_to_add, bool add_at_front);

    void push_back_aux(const value_type& t);
    void push_front_aux(const value_type& t);
    void pop_back_aux();
    void pop_front_aux();
    iterator insert_aux(iterator pos, const value_type& x);
public:
    deque(int n, const value_type& value) :
        start(), finish(), map(0), map_size(0)
    {
        fill_initialize(n, value); //fill_initialize(20, 9)
    }

    //Example: deque<int, alloc, 32> ideq(20, 9); //缓存区大小32字节,元素个数20,初始值9

    iterator begin() { return start; }
    iterator end() { return finish; }
    //invoked __deque_iterator<>::operator[]
    reference operator[](size_type n) { return start[difference_type(n)]; }
    //invoked __deque_iterator<>::operator*
    reference front() { return *start; }
    reference back() 
    {
        iterator tmp = finish;
        --tmp; //invoked __deque_iterator<>::operator--
        return *tmp; //invoked __deque_iterator<>::operator*
    }
    //invoked __deque_iterator::operator--
    size_type size() const { return finish - start; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return finish == start; }

    void push_back(const value_type& t)
    {
        if (finish.cur != finsih.last -1) { //最后缓存区尚有一个以上的备用空间
            cosntruct(finish.cur, t); //直接在备用空间上构造元素
            ++finish.cur; //调整最后缓存区的使用状态
        } else { //最后缓存区已无(或只剩下一个)元素备用空间
            push_back_aux(t);
        }
    }
    void push_front(const value_type& t)
    {
        if (start.cur != start.first) { //第一个缓存区尚有备用空间
            construct(start.cur - 1, t) // 直接在备用空间中构建元素
            --start.cur; //调整第一缓存区的使用状态
        } else { //第一缓存区已无空间
            push_front_aux(t);
        }
    }
    void pop_back()
    {
        if (finish.cur != finish.first) { //最后缓存区有一个(或更多)元素
            --finish.cur; //调整指标, 相当于排除了最后元素
            destory(finish.cur); //将最后元素结构
        } else { //最后缓存区没有任何元素了
            pop_back_aux(); //这里将进行缓存区的释放工作
        }
    }
    void pop_front()
    {
        if (start.cur != start.last - 1) { //第一缓存区有一个(或更多)元素
            destory(start.cur); //将第一元素析构
            ++start.cur; //调整指标, 相当于排除了第一元素
        } else { //第一缓存区仅有一个元素
            pop_front_aux(); //这里将进行缓存区的释放工作
        }
    }
    void clear();
    iterator erase(iterator first, iterator last);
    iterator insert(iterator position, const value_type& x)
    {
        if (position.cur == start.cur) { //如果插入点是deque最前端
            push_front(x);
            return start;
        } else if (position.cur == finish.cur) { //如果插入点是deque最尾端
            push_back(x);
            iterator tmp = finish;
            --tmp;
            return tmp;
        } else {
            return insert_aux(position, x);
        }
    }
};

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type &value)
{
    //把deque的结构都产生并安排好
    create_map_and_nodes(n); //n=20
    map_pointer cur;
    __STL_TRY {
        //为每个节点的缓存区设定初值
        for (cur=start.node; cur<finish.node; ++cur)
            uninitialized_fill(*cur, *cur + buffer_szie(), value);
        //最后一个节点的设定稍有不同(因为尾端可能有备用空间, 不必设初值)
        uninitialized_fill(finish.first, finish.cur, value);
    } catch (...) {
        //...
    }
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type num_elements)
{
    //需要的节点数(元素个数/每个缓存区可容纳的元素个数)+1
    //如果刚好整除, 会多配置一个节点
    size_type num_nodes = num_elements / buffer_size() + 1; //num_nodes=20/8+1=3

    //一个map要管理几个节点, 最少8个, 最多是所需节点数加2
    //前后各预留一个, 扩充时可用
    map_size = max(initialial_map_size(), num_nodes + 2); //map_size=max(8, 5)=8
    map = map_allocator::allocator(map_size);
    //以上配置出一个"具有map_size个节点"的map

    //以下令nstart和nfinish指向所拥有之全部节点的最中央区段
    //保持在最中央, 可使头尾端的扩充能量一样大. 每个节点对应一个缓存区。
    map_pointer nstart = map + (map_size - num_nodes) / 2; //nstart=map+2
    map_pointer nfinish = nstart + num_nodes - 1; //nfinish=map+4

    map_pointer cur;
    try {
        //为map内每个现有节点配置缓存区. 
        //所有缓存区加起来就是deque的可用空间(最后一个缓存区可能留一些余量)
        for (cur = nstart; cur <= nfinish; ++cur)
            *cur = allocate_node();
    } catch (...) {
        //"commit or rollback" 语意
        //...
    }

    //为deque内两个迭代器start和end设定正确内容
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    //前面说过:如果正好整除,则多配置一个节点
    //此时即令cur指向这多配置的一个节点(对应缓存区)的起始处
    finish.cur = finish.first + num_elements % buffer_size(); //map[3]+20%8=map[3]+4
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;

    map_pointer new_nstart; // <++++++++*-------->
    if (map_size > 2 * new_num_nodes) {
        new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        if (new_nstart < start.node)
            copy(start.node, finish.node + 1, new_nstart);
        else
            copy_back(start.node, finish.node + 1, new_nstart + old_num_nodes);
    } else {
        size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
        //配置一块空间, 准备给新map使用
        map_pointer new_map = map_allocator::allocate(new_map_size);
        new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        //把原map内容拷贝过来
        copy(start.node, finish.node + 1, new_nstart);
        //释放原map
        map_allocator::deallocate(map, map_size);
        //设定新map的起始地址与大小
        map = new_map;
        map_size = new_map_size;
    }
    //重新设定迭代器start和finish
    start.set_node(new_nstart);
    finish.set(new_nstart + old_num_nodes - 1);
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::push_back_aux(const value_type &t)
{
    value_type t_copy = t;
    reserve_map_at_back(); //若符合某种条件则必须重换一个map
    *(finish.node + 1) = allocate_node(); //配置一个新节点(缓存区)
    try {
        construct(finish.cur, t_copy);      //针对标的元素设值
        finish.set_node(finish.node + 1);   //改变finish令其指向新节点
        finish.cur = finish.first;          //设定finish的状态
    }
    __STL_UNWIND(deallocate_node(*(finish.node + 1)));
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::push_front_aux(const value_type &t)
{
    //只有当start.cur == start.first时才会被调用
    //即只有当第一个缓存区没有任何备用元素时才会被调用
    value_type t_copy = t;
    reserve_map_at_front(); //若符合某种条件则必须重换一个map
    *(start.node - 1) = allocate_node(); //配置一个新节点(缓存区)
    __STL_TRY {
        start.set_node(start.node - 1); //改变start, 令其指向新节点
        start.cur = start.last - 1; //设定start的状态
        construct(start.cur, t_copy); //针对标的元素设值
    } catch (...) {
        //"commit or rollback"
        start.set_node(start.node + 1);
        start.cur = start.first;
        deallocate_node(*(start.node) - 1);
        throw;
    }
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::pop_back_aux()
{
    deallocate_node(finish.first); //释放最后一个缓存区
    finish.set(finish.node - 1);   //调整finish的状态, 
    finish.cur = finish.last - 1;  //使指向上一个缓存区的最后一个元素
    destory(finish.cur); //将该元素析构
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::pop_front_aux()
{
    destory(start.cur);             //将第一缓存区的第一个元素析构
    dealocate_node(stat.first);     //释放第一个缓存区
    start.set_node(start.node + 1); //调整start的状态,使指向
    start.cur = start.first;        //下一个缓存区的第一个元素
}

template <class T, class Alloc, size_t BufSiz>
inline typename deque<T, Alloc, BufSiz>::iterator
deque<T, Alloc, BufSiz>::insert_aux(iterator pos, const value_type &x)
{
    difference_type index = pos - start; //插入点之前的元素个数
    value_type x_copy = x;
    if (index < size() / 2) { //如果插入点之前的元素个数比较少
        push_front(front());  //在最前端加入与第一元素同值的元素
        iterator front1 = start; //以下标识记号,然后进行元素移动
        ++front1; 
        iterator front2 = front1;
        ++front2;
        pos = start + index;
        iterator pos1 = pos;
        ++pos1;
        copy(front2, pos1, front1); //元素移动
    } else { //插入点之后的元素个数比较少
        push_back(back()); //在最前端加入与最后元素同值的元素
        iterator back1 = finish; //以下标识记号,然后进行元素移动
        --back1;
        iterator back2 = back1;
        --back2;
        pos = start + index;
        copy_backward(pos, back2, back1); //元素移动
    }
    *pos = x_copy;
    return pos;
}

template <class T, class Alloc, size_t BufSiz>
inline void deque<T, Alloc, BufSiz>::clear()
{
    //注意,最终需要保留一个缓存区,这是deque的策略,也是deque的初始状态
    //以下针对头尾以外的每一个缓存区(他们一定都是满载的)
    for (map_pointer node = start.node + 1; node < finish.node; ++node) {
        //将缓存区内所有元素析构,注意调用的是destory()第二版本,见2.2.3节
        destory(*node, *node + buffer_size());
        //释放缓存区内存
        data_allocator::deallocte(*node, buffer_size());
    }

    if (start.node != finish.node) { //至少有头尾两个缓存区
        destory(start.cur, start.last); //将头缓存区的目前所有元素析构
        destory_node(finish.fisrt, finish.cur); //将尾缓存区的目前所有元素析构
        //以下释放尾缓存区,注意头缓存区保留
        data_allocator::deallocate(finish.first, buffer_size());
    } else { //只有一个缓存区
        destory(start.cur, finish.cur); //将此唯一缓存区内所有元素释放
        //注意,并不释放缓存取空间,这唯一的缓存区将保留
        finish = start; //调整状态
    }
}

template <class T, class Alloc, size_t BufSiz>
inline deque<T, Alloc, BufSiz>::iterator 
deque<T, Alloc, BufSiz>::erase(iterator first, iterator last)
{
    if (first == start && last == finish) { //如果清除空间就是整个deque,直接调用clear即可
        clear();
        return finish;
    } else {
        difference_type n = last - first;   //清除区间的长度
        difference_type elems_before = first - start; //清除区间的元素个数
        if (elems_before < (size() - n) / 2) { //如果前方的元素比较少
            copy_backward(start, first, last); //向后移动前方的元素(覆盖清除区间)
            iterator new_start = start + n; //标记deque的新起点
            destory(start, new_start); //移动完毕,将冗余的元素析构
            //以下将冗余的缓存区释放
            for (map_pointer cur=start.node; cur<new_start.node; ++cur)
                data_allocator::deallocate(*cur, buffer_size());
            start = new_start;  //设定deque的新起点
        } else { //如果清除区间后方的元素比较少
            copy(last, finish, first); //向前移动后方元素(覆盖清除区间)
            iterator new_finish = finish - n; //标记deque的新尾点
            destory(new_finish, finish);
            //以下将冗余的缓存区释放
            for (map_pointer cur=new_finish.node; cur<finish.node; ++cur)
                data_allocator::deallocate(*cur, buffer_size());
            finish = new_finish;  //设定deque的新尾点
        }
        return start + elems_before;
    }
}

#endif // SGI_STL_DEQUE_H
