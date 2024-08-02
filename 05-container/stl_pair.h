#ifndef SGI_STL_PAIR_H
#define SGI_STL_PAIR_H

//仅是为了rb_tree内使用新加的
template <class K, class V>
struct pair {
private:
    K _k;
    V _v;
};

#endif // SGI_STL_PAIR_H