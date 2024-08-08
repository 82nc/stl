
/*
 * 不同编译器对C++语言的支持程度不尽相同, 作为一个希望具备广泛移植能力的程序库
 * SGI STL准备了一个环境组态文件 stl_config.h, 其内定义了许多常量, 标识某个组态成立与否
 * 所有的STL头文件都会间接包含该文件, 并以条件式写法, 让处理器根据各常量决定取舍哪一段程序代码
 * 
 * 起始有一份常量定义说明, 然后针对各家不同编译器及可能的版本, 给予常量设定.
 * 
 * 以下是GNU C++2.91.57 <stl_config.h>的完整内容
 */

#ifndef __STL_CONFIG_H
#define __STL_CONFIG_H

// 本文所做的事情:
// (01) 如果编译器没有定义bool,true,false, 就定义他们
// (02) 如果编译器的标准程序库未支持drand48(), 就定义__STL_NO_DRAND48
// (03) 如果编译器无法处理static members of template classess, 就定义
//      __STL_STATIC_TEMPLATE_MEMBER_BUG
// (04) 如果编译器未支持关键词typename, 就将'typename'定义为null macro
// (05) 如果编译器支持partical specialization of class templates, 就定义
//      __STL_CLASS_PARTIAL_SPECIALIZATION
// (06) 如果编译器支持partical ordering of function templates(亦称为
//      partial specialization of functions template), 就定义
//      __STL_FUNCTION_TMPL_PARTIAL_OEDER
// (07) 如果编译器允许我们在调用一个function template时可以明白指定其
//      template arguments, 就定义__STL_EXPLICIT_FUNCTION_TMPL_ARGS
// (08) 如果编译器支持template members of classes, 就定义
//      __STL_MEMBER_TEMPLATES
// (09) 如果编译器不支持关键字explicit, 就定义'explicit'为一个null macro
// (10) 如果编译器无法根据前一个template parameters设定下一个template parameters
//      默认值, 就定义__STL_LIMITED_DEFAULT_TEMPLATES
// (11) 如果编译器针对non-type template parameters执行function template
//      的参数推导(argument deduction)时有问题, 就定义__STL_NON_TYPE_TMPL_PARAM_BUG
// (12) 如果编译器无法支持迭代器的operator->, 就定义__SGI_STL_NO_ARROW_OPERATOR
// (13) 如果编译器(在你所选择的模式中)支持exceptions, 就定义__STL_USE_EXCEPTIONS
// (14) 定义__STL_USE_NAMESPACES可使我们自动获得using std::list; 之类的语句
// (15) 如果本程序库由SGI编译器来编译, 而且使用者未选择pthreads或者其他threads, 就定义
//      __STL_SGI_THREADS
// (16) 如果本程序库由一个WIN32编译器编译, 并且在多线程模式下, 就定义__STL_WIN32THREADS
// (17) 适当地定义与namespace相关的macros如__STD, __STL_BEGIN_NAMESPACE
// (18) 适当的定义exception相关的macros如__STL_TRY, __STL_UNWIND
// (19) 根据__STL_ASSERTIONS是否定义, 将__stl_assert定义为一个测试操作或者一个null macro

#ifdef _PTHREADS
    #define __STL_PTHREADS
#endif // _PTHREADS

#if defined(__sgi) && !defined(__GNUC__)
// 使用SGI STL但却不使用GNU C++

    #if !defined(_BOOL)
        #define __STL_NEED_BOOL
    #endif // !_BOOL

    #if !defined(_TYPENAME_IS_KEYWORD)
        #define __STL_NEED_TYPENAME
    #endif // !_TYPENAME_IS_KEYWORD

    #ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
        #define __STL_CLASS_PARTIAL_SPECIALIZATION
    #endif // _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES

    #ifdef _MEMBER_TEMPLATES
        #define __STL_MEMBER_TEMPLATES
    #endif // _MEMBER_TEMPLATES

    #if !defined(_EXPLICIT_IS_KEYWORD)
        #define __STL_NEED_EXPLICIT
    #endif // !_EXPLICIT_IS_KEYWORD

    #ifdef __EXCEPTIONS
        #define __STL_USE_EXCEPTIONS
    #endif // __EXCEPTIONS

    #if (_COMPILER_VERSION >= 721) && defined(_NAMESPACES)
        #define __STL_USE_NAMESPACES
    #endif // _NAMESPACES

    #if !defined(_NOTHREADS) && !defined(__STL_PTHREADS)
        #define __STL_SGI_NAMESPACES
    #endif // !_NOTHREADS && !__STL_PTHREADS

#endif // __sgi && !__GNUC__

#ifdef __GNUC__
    #include <_G_config.h>
    #if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC__MINOR__ < 8)
        #define __STL_STATIC_TEMPLATE_MEMBER_BUG
        #define __STL_NEED_TYPENAME
        #define __STL_NEED_EXPLICIT
    #else // GNUC 2.8+
        #define __STL_CLASS_PARTIAL_SPECIALIZATION
        #define __STL_FUNCTION_TMPL_PARTIAL_ORDER
        #define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
        #define __STL_MEMBER_TEMPLATE
    #endif 

    /* glibc pre 2.0 is very buggy. We have to disable thread for it.
       It should be upgraded to glibc 2.0 or later. */
    #if !defined(_NOTHREADS) && __GLIBC__ >= 2 && defined(_G_USING_THUNKS)
        #define __STL_PTHREADS
    #endif

    #ifdef __EXCEPTIONS
        #define __STL_USE_EXCEPTIONS
    #endif
#endif // __GNUC__

#if defined(__SUNPRO_CC)
// 略
#endif

#if defined(__COMO__)
// 略
#endif

#if defined(_MSC_VER)
// 略
#endif

#if defined(__BORLANDC)
// 略
#endif

#if defined(__STL_NEED_BOOL)
    typedef int bool
    #define true  1
    #define false 0
#endif

#ifdef __STL_NEED_TYPENAME
    #define typename //侯捷:难道不该是#define typename class
#endif

#ifdef __STL_NEED_EXPLICIT
    #define explicit
#endif

#ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
    #define __STL_NULL_TMPL_ARGS <>
#else
    #define __STL_NULL_TMPL_ARGS
#endif

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
    #define __STL_TEMPLATE_NULL template<>
#else
    #define __STL_TEMPLATE_NULL
#endif

// __STL_NO_NAMESPACES is a hook so that users can disable namespaces
// without having to edit library headers.
#if defined(__STL_USE_NAMESPACES) && !defined(__STL_NO_NAMESPACES)
    #define __STD std
    #define __STL_BEGIN_NAMESPACE namespace std {
    #define __STL_END_NAMESPACE }
    #define __STL_USE_NAMESPACE_FOR_RELOPS
    #define __STL_BEGIN_RELOPS_NAMESPACE namespace std {
    #define __STL_END_RELOPS_NAMESPACE }
    #define __STD_RELOPS std
#else
    #define __STD
    #define __STL_BEGIN_NAMESPACE
    #define __STL_END_NAMESPACE
    #undef  __STL_USE_NAMESPACE_FOR_RELOPS
    #define __STL_BEGIN_RELOPS_NAMESPACE
    #define __STL_END_RELOPS_NAMESPACE
    #define __STD_RELOPS
#endif

#ifdef __STL_USE_EXCEPTIONS
    #define __STL_TRY try
    #define __STL_CATCH_ALL catch(...)
    #define __STL_RETHROW throw
    #define __STL_NOTHROW throw()
    #define __STL_UNWIND(action) catch(...) { action; throw; }
#else
    #define __STL_TRY
    #define __STL_CATCH_ALL if (false)
    #define __STL_RETHROW
    #define __STL_NOTHROW
    #define __STL_UNWIND(action)
#endif

#ifdef __STL_ASSERTIONS
    #include <stdio.h>
    #define __stl_assert(expr) \
        if (!(expr)) { fprintf(stderr, "%s:%d STL assertion failure: %s\n", \
            __FILE__, __LINE__, # expr); abort(); }
#else
    #define __stl_assert(expr)
#endif

#endif // SGI_STL_CONFIG_H