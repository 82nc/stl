/* NOTE: This is an internal header file, included by other STL headers.
 * You should not attempt to use it directly.
 */

#ifndef SGI_STL_TYPE_TRAITS_H
#define SGI_STL_TYPE_TRAITS_H

struct __true_type { };
struct __false_type { };

template <class type>
struct __type_traits {
    typedef __true_type this_dummy_member_must_be_first;
    /* 不要移除这个成员, 它通知"有能力自动将__type_traits特化"的编译器说:
     * 我们现在将看到这个_type_traits template是
     * 特殊的. 这是为了确保万一编译器也使用一个名为__type_traits
     * 而其实于此定义并无任何关联的template时, 所有的事情仍将顺利运作 
     */

    /* 以下条件应该被遵守, 因为编译器有可能自动为各类别产生专属的__type_traits的特化版本:
     * - 你可以重新排列以下成员次序
     * - 你可以移除以下任何成员
     * - 绝对不可以将以下成员重新命名而却没有改变编译器中的对应名称
     * - 新加入的成员被视为一般成员, 除非你在编译器中加上适当支持
     */

    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;

    /* 一般模板实例, 内含对所有类别都必定有效的保守值. 
     * 上述各个has_trivial_xxx型别被定义为__false_type
     * 就是对所有型别必定有效的保守值经过声明的特化版本,
     * 例如<type_traits.h>内对所有C++标量型别提供对应特化声明
     * 某些编译器会自动为所有型别提供适当的特化版本
     */

    /* 以下针对C++基本型别char, signed char, unsigned char, short, unsigned short
     * int, unsigned int, long, unsigned long, float, double, long double提供特化版本
     * 注意, 每个成员的值都是__true_type, 表示这些型别都可以采用最快速方式(例如memory)来进行
     * 拷贝或赋值操作
     */

    /* 注意, SGI_STL<stl_config.h>将以下出现的__STL_TEMPLATE_NULL
     * 定义为template<>, 见1.9.1, 是所谓的class template explicit specialization
     */

    #define __STL_TEMPLATE_NULL template<>
    __STL_TEMPLATE_NULL struct __type_traits<char> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<signed char> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<short> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<int> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<long> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<float> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<double> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };

    __STL_TEMPLATE_NULL struct __type_traits<long double> {
        typedef __true_type has_trivial_default_constructor;
        typedef __true_type has_trivial_copy_constructor;
        typedef __true_type has_trivial_assignment_operator;
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
};

// 注意, 以下针对原生指针设计__type_traits偏特化版本
// 原生指针亦被视为一种标量型别
template <class T>
struct __type_traits<T*> {
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};


#endif  // SGI_STL_TYPE_TRAITS_H