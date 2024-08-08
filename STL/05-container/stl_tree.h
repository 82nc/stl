#ifndef SGI_STL_TREE_H
#define SGI_STL_TREE_H

#include "03-iterator/stl_iterator.h"
#include "02-allocator/stl_alloc.h"

#include "05-container/stl_pair.h" //zyw自己手动加的

/* 
 * RB_TREE RULES
 * #1 每个节点都有颜色, 非红即黑
 * #2 根节点为黑
 * #3 父子节点不能同时为红
 * #4 任意节点到NULL节点(树末端)之任一路径, 所包含黑节点数量必须相同
 * #5 NULL节点为黑色 
 * 注意: 叶节点是树最深的节点, 它没有子节点, 只有两个NULL节点.
 */

// see https://oi-wiki.org/ds/rbtree/

/* ============================================
 * 现在总结一下插入结点面临的几种情况以及采取的措施:
 * 1.树为空，插入的结点为根结点
 *   直接将插入的结点变成黑色
 * 2.父亲结点为黑色结点
 *   不需要任何操作
 * 3.父亲结点为红色结点的情况下：
    3.1 叔叔结点也为红色结点
    将叔叔和父亲结点改为黑色，爷爷结点改为红色，未完，然后又将爷爷结点当作插入结点看待，
    一直进行上面的操作，直到当前结点为根结点，然后将根结点变成黑色
    3.2 叔叔结点为黑色结点的情况下：
      3.2.1 (父亲结点为左孩子，插入结点为左孩子) || (父亲结点为右孩子，插入结点为右孩子)
      将父亲结点和爷爷结点的颜色互换，然后针对爷爷结点进行一次右/左旋
      3.2.2 (父亲结点为左孩子，插入结点为右孩子) || (父亲结点为右孩子，插入结点为左孩子）
      针对父结点进行左旋，此时左旋后的情况必定是3.2.1的情况(此时父节点看成当前节点)，然后按照3.2.1的情况处理
 * =============================================
 */

/*
现在我们来讨论一下，为什么插入的情况只有上面这些：
1.爷爷结点为红色结点的情况下，父亲结点只能为黑色（红黑树的性质4），处理操作：上面情况2
2.爷爷结点为黑色的情况下，父亲结点和叔叔结点：可以为红色，也可以为黑色
  2.1 父亲结点为黑，叔叔结点为黑：处理操作：上面情况2
  2.2 父亲结点为黑，叔叔结点为红：处理操作：上面情况2
  2.3 父亲结点为红，叔叔结点为红：处理操作：上面情况3.1
    //上面3种情况都是不用考虑左右的

  2.4 父亲结点为红，叔叔结点为黑：
     2.4.1 父亲结点为左孩子，叔叔结点为左孩子：处理操作：上面情况3.2.1
     2.4.2 父亲结点为右孩子，叔叔结点为右孩子：处理操作：上面情况3.2.1
     2.4.3 父亲结点为左孩子，插入结点为右孩子：处理操作：上面情况3.2.2
     2.4.4 父亲结点为右孩子，插入结点为左孩子：处理操作：上面情况3.2.2
 */

/*
 * 新增节点X, 父节点P, 祖父节点G, 伯父节点S, 曾祖父节点GG
 *
 * 根据二叉搜索树的规则, 新增节点X必须为叶节点;
 * 根据红黑树规则4, X必为红, 若P亦为红(违反规则3,必须调整树形), 则G必为黑(因为原树要符合规则3)
 * 根据X的插入位置即外围节点(S和GG)的颜色,有以下四种考虑
 * 状况1: S为黑且X为外侧插入(左左、右右)
 *     先对P,G做一次单旋转,再改变P,G颜色,即可满足红黑树规则.
 * 状况2: S为黑且X为内侧插入()
 *     先对P,G做一次单旋转,再改变G,X颜色,再将结果对G做一次单旋转.
 * 状况3: S为红且X为外侧插入
 *     先对P,G做一次单旋转,再改变X的颜色,此时如果GG为黑,一切搞定.
 *     如果GG为红,问题大一些,见状况4
 * 状况4: S为红且X为外测插入
 *     先对P,G做一次单旋转, 再改变X的颜色,此时如果GG亦为红, 
 *     还得持续往上做,直到不再有父子连续为红的情况.
 */

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;   //红色为0
const __rb_tree_color_type __rb_tree_black = false; //黑色为1

struct __rb_tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color; //节点颜色,非红即黑
    base_ptr parent;  //RB树的许多操作必须知道父节点
    base_ptr left;    //指向左节点
    base_ptr right;   //指向右节点

    static base_ptr minimum(base_ptr x)
    {
        while (x->left != 0) 
            x = x->left; //一直向左走找到最小值
        return x;
    }
    static base_ptr maximum(base_ptr x)
    {
        while (x->right != 0) 
            x = x->right; //一直向右走找到最大值
        return x;
    }
};

template <class Value>
struct __rb_tree_node : public __rb_tree_node_base {
    typedef __rb_tree_node<Value>* link_type;
    Value value_field; //节点值
};

//基层迭代器
struct __rb_tree_base_iterator {
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    base_ptr node; //用来与容器之间产生一个连结关系(make a reference)
    //迭代器与__rb_tree_node一一对应, 且__rb_tree_node地址也是base_ptr

    //以下其实可实现于operator++内,因为再无他处会调用此函数
    void increment()
    {
        if (node->right != 0) {         //如果有右节点(状况1)
            node = node->right;         //就向右走
            while (node->left != 0)     //然后一直往左子树走到底
                node = node->left;      //即是解答
        } else {                        //没有右子节点(状况2)
            base_ptr y = node->parent;  //找出父节点
            while (node == y->right) {  //如果现行节点本身是个右子节
                node = y;               //就一直上溯,直到"不为右子节点"止
                y = y->parent;
            }
            if (node->right != y)       //若此时的右节点不等于此时的父节点(状况3)
                node = y;               //此时的父节点即为解答,否则此时的node为解答(状况4)
        }
        //注意: 以上判断"若此时的右子节点不等于此时的父节点", 是为了应付一种特殊情况
        //我们欲寻找根节点的下一个节点, 而恰巧根节点无右子节点.
        //当然,以上特殊做法必须配合RB-tree跟节点与特殊之header之间的特殊关系
    }

    //以下其实可实现于operator--内,因为再无他处会调用此函数了
    void decrement()
    {
        if (node->color == __rb_tree_red && //如果是红节点,且父节点的父节点等于自己
            node->parent->parent == node) { //父节点的父节点等于自己
            node = node->right;             //状况(1)右子节点即为解答
        //以上情况发生于node为header时(亦即node为end时)
        //注意,header之右子节点即mostright, 指向整棵树的max节点
        } else if (node->left != 0) {       //如果有左子节点(状况2)
            base_ptr y = node->left;        //令y指向左子节点
            while (y->right != 0)           //当y有右子节点时
                y = y->right;               //一直往右子节点走到底
            node = y;                       //最后即为答案
        } else {                            //既非根节点,亦无左子节点
            base_ptr y = node->parent;      //状况3,找出父节点
            while (node == y->left) {       //当现行节点身为左子节点
                node = y;                   //一直交替往上走,直到现行节点
                y = y->parent;              //不为左子节点
            }
            node = y;                       //此时之父节点即为答案          
        }
    }
};
//以上双层架构与slist极相似

//RB-tree的迭代器
template <class Value, class Ref, class Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
    typedef __rb_tree_iterator<Value, Ref, Ptr> self;
    typedef __rb_tree_node<Value>* link_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) { node = x; }
    __rb_tree_iterator(cosnt iterator& i) { node = i.node; }

    reference operator*() const { return link_type(node)->value_field; }
    #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const { return &(operator*()); }
    #endif // __SGI_STL_NO_ARROW_OPERATOR

    self& operator++() { increment(); return *this; }
    self& operator--() { decrement(); return *this; }
    self operator++(int) { self tmp = *this; increment(); return tmp; }
    self operator--(int) { self tmp = *this; decremnet(); return tmp; }

    //当迭代器指向根节点而后者无右子节点时,若对迭代器进行++操作,
    //会进入__rb_tree_base_iterator::increment()的状况2,4

    //当迭代器为end(),若对迭代器进行操作,
    //会进入__rb_tree_base_iterator::decrement()的状况1
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree {
protected:
    typedef void* void_pointer;
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef simple_alloc<rb_tree_node, alloc> rb_tree_node_allocator;
    typedef __rb_tree_color_type color_type;
public:
    //注意没有定义iterator(定义在后面)
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    link_type get_node() { return rb_tree_node_allocator::allocate(); }
    void put_node(link_type p) { rb_tree_node_allocator::deallocator(p); }

    link_type create_node(const value_type& x) { 
        link_type tmp = get_node(); //配置空间
        __STL_TRY {
            construct(&tmp->value_field, x); //构造函数
        }
        __STL_UNWIND(put_node(tmp));
        return tmp;
    }

    link_type clone_node(link_type x) { //复制一个节点(的值和色)
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = 0;
        tmp->right = 0;
        return tmp;
    }

    void destory_node(link_type p) {
        destory(&p->value_field); //析构内存
        put_node(p); //释放内存
    }
protected:
    //RB-tree只以三笔数据表现
    link_type header;       //这是实现上的一个技巧
    size_type node_count;   //追踪记录树的大小(节点数量)
    Compare key_compare;    //节点间的键值大小比较准则,应该会是function object

    //以下三个函数用来方便取得header的成员
    link_type& root() const { return (link_type&) header->parent; }
    link_type& leftmost() const { return (link_type&) header->left; }
    link_type& rightmost() const { return (link_type&) header->right; }

    //以下六个函数用来方便取得节点x的成员
    static link_type& left(link_type x) { return (link_type&)(x->left); }
    static link_type& right(link_type x) { return (link_type&)(x->right); }
    static link_type& parent(link_type x) { return (link_type&)(x->parent); }
    static reference value(link_type x) { return (link_type&)(x->value_field); }
    static const Key& key(link_type x) { return KeyOfValue(value(x)); }
    static color_type& color(link_type x) { return (color_type&)(x->color); }

    static link_type& left(base_ptr x) { return (link_type&)(x->left); }
    static link_type& right(base_ptr x) { return (link_type&)(x->right); }
    static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
    static reference value(base_ptr x) { return (link_type&)(x->value_field); }
    static const Key& key(base_ptr x) { return KeyOfValue(value(x)); }
    static color_type& color(base_ptr x) { return (color_type&)(x->color); }

    static link_type minimum(link_type x) {
        return (link_type) __rb_tree_node_base::minimum(x);
    }
    static link_type maximum(link_type x) {
        return (link_type) __rb_tree_node_base::maximum(x);
    }

public:
    typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef __rb_tree_iterator<const value_type, const reference, const pointer> const_iterator;

private:
    //真正执行插入操作的函数
    iterator __insert(base_ptr x, base_ptr y, const Value& v);
    link_type __copy(link_type x, link_type p);
    void __erase(link_type x);
    void init() {
        header = get_node(); //产生一个节点空间,令header指向它
        color(header) = __rb_tree_red; //令header为红色, 
        //用来区分header和root(在iterator.operator++中)
        root() = 0;
        leftmost() = header;  //令header的左子节点为自己
        rightmost() = header; //令header的右子节点为自己

        //注意: 树状结构的各种操作, 最需要注意的就是边界情况的发生, 也就是
        //走到根节点时要有特殊处理, 为了简化处理, SGI STL特别为根节点再设计
        //了一个父节点: header节点.
    }
public:
    rb_tree(const Compare& comp = Compare())
        : node_count(0), key_compare(comp) { init(); }
    ~rb_tree() { clear(); put_node(header); }

    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);
public:
    //accessors
    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); } //RB树的起头最左(最小)节点处
    iterator end() { return header; } //RB树的终点为header所指处
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }
public:
    //insert/erase
    //将x插入到RB-tree中(保持节点值独一无二)
    pair<iterator, bool> insert_unique(const Value& x);
    //将x插入到RB-tree中(允许节点重复)
    iterator insert_equal(const Value& x);
    //查找操作
    iterator find(const Key& k);
    //删除操作
    void erase(const iterator& x);
};

//全局函数: 新节点必为红节点, 如果插入处之父节点亦为红节点,就违反红黑树规则
//此时必须做树形旋转(及颜色改变,在程序它处)
inline void
__rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
    //x为旋转点
    __rb_tree_node_base* y = x->right; //令y为旋转点的右子节点
    x->right = y->left;
    if (y->left != 0)
        y->left->parent = x; //别忘了回马枪设定父节点
    y->parent = x->parent;

    //令y完全顶替x地位(必须将x对应父节点的关系完全接过来)
    if (x == root) //x为根节点
        root = y;
    else if (x == x->parent->left) //x为其父节点的左子节点
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

//全局函数: 新节点必为红节点,如果插入处之父节点亦为红节点,就违反红黑树规则,此时必须
//做树形旋转(及颜色改变,在程序其他处)
inline void 
__rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
    //x为旋转点
    __rb_tree_node_base* y = x->left; //y为旋转点的左子节点
    x->left = y->right;
    if (y->right != 0)
        y->right->parent = x; //别忘了回马枪设定父节点
    y->parent = x->parent;

    //令y完全顶替x的地位(必须将x对其父节点的关系完全接收过来)
    if (x == root)
        root = y;
    else if (x == x->parent->right) //x为其父节点的右子节点
        x->parent->right = y;
    else //x为其父节点的左子节点
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

//全局函数: 重新令树形平衡(改变颜色及旋转树形)
//参数一: 新增节点, 参数二: root
inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
    x->color = __rb_tree_red; //新节点必为红
    while (x != root && x->parent->color == __rb_tree_red) { //父节点为红
        if (x->parent == x->parent->parent->left) { //父节点为祖父节点之左子节点
            __rb_tree_node_base* y = x->parent->parent->right; //令y为伯父节点
            if (y && y->color == __rb_tree_red) { //伯父节点存在,且为红
                x->parent->color = __rb_tree_black; //更改父节点为黑
                y->color = __rb_tree_black; //更改伯父节点为黑
                x->parent->parent->color = __rb_tree_red; //更改祖父节点为红
                x = x->parent->parent;
            } else { //无伯父节点或者伯父节点为黑
                if (x == x->parent->right) { //如果新节点为父节点之右子节点
                    x = x->parent;
                    __rb_tree_rotate_left(x, root); //第一参数为左旋点
                }
                x->parent->color = __rb_tree_black; //改变颜色
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root); //第一参数为右旋点
            }
        } else { //父节点为祖父节点之右子节点
            __rb_tree_node_base* y = x->parent->parent->left; //令y为伯父节点
            if (y && y->color == __rb_tree_red) { //有伯父节点,且为红
                x->parent->color = __rb_tree_red; //更改父节点为黑
                y->color = __rb_tree_black;       //更改伯父节点为黑
                x->parent->parent->color = __rb_tree_red; //更改祖父节点为红
                x = x->parent->parent; //准备继续往上层检查
            } else { //无伯父节点, 或伯父节点为黑
                if (x == x->parent->left) { //如果新节点为父节点之左节点
                    x = x->parent;
                    __rb_tree_rotate_right(x, root); //第一参数为右旋节点
                }
                x->parent->color = __rb_tree_black; //改变颜色
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root); //第一参数为左旋点
            }
        }
    } //while结束
    root->color = __rb_tree_black; //根节点永远为黑
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value &v)
{
    //参数x_为新值插入点, 参数y_为插入点之父节点, 参数v为新值
    link_type x = (link_type) x_; //maybe root
    link_type y = (link_type) y_; //maybe head 
    link_type z;

    //key_compare键值大小比较准则(function object)
    if (y == header || x || key_compare(KeyOfValue()(v), key(y))) {
        z = create_node(v); //产生一个新节点
        left(y) = z; //这使得y即为header时, leftmost() = z
        if (y == header) {
            root() = z;
            rightmost() = z;
        } else if (y == leftmost()) //如果y为最左节点
            leftmost() = z; //维护leftmost(),使它永远指向最左节点
    } else {
        z = create_node(v); //产生一个新节点
        right(y) = z; //令新节点成为插入点之父节点y的右子节点
        if (y == rightmost())
            rightmost() = z; //维护rightmost(),使它永远指向最右节点
    }
    parent(z) = y; //设定新节点的父节点
    left(z) = 0;   //设定新节点的左子节点
    right(z) = 0;  //设定新节点的右子节点
    //新节点的颜色将在__rb_tree_reblance()设定(并调整)
    __rb_tree_rebalance(z, header->parent); //参数一为新增节点, 参数二为root
    ++node_count; //节点数累加
    return iterator(z); //返回迭代器,指向新增节点
}

//插入新值, 节点值允许重复, 返回值是一个RB-tree迭代器,指向新增节点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v)
{
    link_type y = header;
    link_type x = root();
    while (x != 0) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        //以上遇大则往左,遇小则往右
    }
    return __insert(x, y, v);
    //以上, x为新增插入点, y为插入点之父节点, v为新值
}

//插入新值: 节点值不允许重复,若重复则插入无效
//注意: 返回值是个pair,第一个是个RB-tree迭代器,指向新增节点
//第二叉元素表示插入成功与否
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
{
    link_type y = header;
    link_type x = root(); //从根节点开始
    bool comp = true;
    while (x != 0) { //从根节点开始,往下寻找适当的插入点
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x)); //v键值小于目前节点之键值?
        x = comp ? left(x) : right(x); //遇大于则往左,遇小于等于则往右
    }
    //离开循环后y即为插入点之父节点(此时它必为叶节点)

    iterator j = iterator(y); //令迭代器j指向插入点之父节点y
    if (comp) //如果离开while循环时comp为真(表示遇大,将插入于左侧)
        if (j == begin()) //如果插入点之父节点为最左节点
            return pair<iterator, bool>(__insert(x, y, v), true);
            //以上x为插入点,y为插入点之父节点,v为新值
        else //否则(插入点之父节点不为最左节点)
            --j; //调整j,回头准备测试...
    if (key_compare(key(j.node), KeyOfValue()(v))) //小于新值(表示遇到小值,将插入于右侧)
        return pair<iterator, bool>(__insert(x, y, v), true);
        //以上x为新值插入点,y为插入点之父节点,v为新值
    
    //进行至此,表示新值一定与树中键值重复,那么就不应该插入新值
    return pair<iterator, bool>(j, false);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key &k)
{
    link_type y = header; //Last node which is not less than k.
    link_type x = root(); //Current node

    while (x != 0) //以下: key_compare是节点值大小的比较准则,应该是个function object
        if (!key_compare(key(x), k)) //到这里表示x键值大于k,遇到大值就向左走
            y = x, x = left(); //注意语法
        else //进行到这里, 表示x键值小于k,遇到小值就向右走
            x = right(x);
    iterator j = iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

#endif // SGI_STL_RB_TREE_H


/*
 * ========= TEST DEMO ============
 *
rb_tree<int, int, identity<int>, less<int>> itree;
itree.insert_unique(10); //__rb_tree_rebalance
itree.insert_unique(7); //__rb_tree_rebalance
itree.insert_unique(8); //__rb_tree_rebalance __rb_tree_rotate_left __rb_tree_rotate_right
itree.insert_unique(15); //__rb_tree_rebalance
itree.insert_unique(5); //__rb_tree_rebalance
itree.insert_unique(6); //__rb_tree_rebalance __rb_tree_rotate_left __rb_tree_rotate_right
itree.insert_unique(11); //__rb_tree_rebalance __rb_tree_rotate_right __rb_tree_rotate_left
itree.insert_unique(13); //__rb_tree_rebalance
itree.insert_unique(12);

//测试颜色和operator++ (亦即__rb_tree_iterator_base::increment)
rb_tree<int, int, identity<int>, less<int>>::iterator it1 = itree.begin();
rb_tree<int, int, identity<int>, less<int>>::iterator it2 = itree.end();
__rb_tree_base_iterator rbtite;

for (; it1 != it2; ++it1) {
    rbtite = __rb_tree_base_iterator(it1);
    std::cout << *it1 << '(' << rbtite.node->color << ") ";
}
std::cout << std::endl;

*/