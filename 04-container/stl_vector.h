#ifndef SGI_STL_VECTOR_H
#define SGI_STL_VECTOR_H

#include "02-allocator/stl_alloc.h"

// 虽然STL规定, 欲使用vector者必须先包含<vetor>, 但SGI STL将vector实现于更底层
// <stl_vector.h>

// alloc 是SGI STL的空间配置, 见第二章
template <class T, class Alloc = alloc>
class vector {
public:
    //vector的嵌套定义
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type*     iterator;
    typedef value_type&     reference;
    typedef size_t*         size_type;
    typedef ptrfdiff_t      difference_type;

protected:
    //以下simple_alloc是SGI STL的空间配置器
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator start;             //表示目前使用空间的头
    iterator finish;            //表示目前使用空间的尾
    iterator end_of_storage;    //表示目前可用空间的尾

    void insert_aux(iterator position, const T& x);
    void deallocate() { 
        if (start) 
            data_allocator::deallocate(start, end_of_storage - start); 
    }
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }
public:
    iterator begin() { return start; }
    iterator end() { return finish; }
    size_type size() { return size_type(end() - begin()); }
    size_type capacity() const { return size_type(end_of_storage - begin()); }
    bool empty() const { return begin() == end(); }
    reference operator[](size_type n) { return (begin() + n); }

    vector(): start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T& value): { fill_initialize(n, value); }
    vector(int n, const T& value) { fill_initialize(n, value); }
    vector(long n, const T& value) { fill_initialize(n ,value); }
    explicit vactor(size_type n) { fill_initialize(n, T()); }

    ~vector() { 
        destory(start, finish); // 全局函数,第二章(stl_construt.h)
        deallocate(); // 这是vector的一个成员函数
    }
    reference front() { return *begin(); } // 第一个元素
    reference back() { return *(end() - 1); } // 最后一个元素
    void push_back(const T& x) { // 将元素插入至最尾端
        if (finish != end_of_storage) {
            construct(finish, x);        // 全局函数
            ++finish;
        } else {
            insert_aux(end(), x);       // 这是vector的一个成员函数
        }
    }
    void pop_back() { // 将最尾元素取出
        --finish;
        destorry(finish);           // 全局函数,第二章(stl_construt.h)
    }
    void insert(iterator position, size_type n, const T& x);

    // 清除[first, last]中的所有元素
    iterator erase(iterator first, iterator last) {
        iterator i = copy(last, finish, first); // copy全局函数, 第6章
        destory(i, finish); // destory 全局函数, 第2章
        finish = finish - (last - first);
        return first;
    }
    iterator erase(iterator position) { // 清除某位置上的元素
        if (position + 1 != end())
            copy(position + 1, finish, position); // 后续元素往前移动, 全局函数,第六章
        --finish;
        destory(finish); // 全局函数,第二章(stl_construt.h)
        return position;
    }
    void resize(size_type new_szie, onst T& x) {
        if (new_size < size)
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void resize(size_type new_size) { resize(new_size, T()); }
    void clear() { erase(begin(), end()); }
protected:
    // 配置空间并填满内容
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x) // 全局函数,第二章(stl_uninitialized.h)
        return result;
    }
};

template <class T, class Alloc>
inline void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
    if (finish != end_of_storage) { // 还有备用空间
        // 在备用空间起始处构造一个元素, 并以vector最后一个元素为其初值
        construct(finish, *(finish - 1));
        // 调整水位
        ++finish;
        T x_cpoy = x;
        copy_backward(position, finish - 2, finish - 1);
        *positon = x_copy;
    } else { // 已无备用空间
        const size_type old_size = size();
        const size_type len = old_size != 0 ? 2 * old_size : 1;
        // 以上配置原则: 如果大小为0, 则配置1个元素
        // 如果原大小不为0, 则配置原大小的两倍
        // 前半段用来放置原数据, 后半段准备用来配置新数据
        iterator new_start = data_allocator::allocate(len); // 实际配置
        iterator new_finish = new_start;
        try {
            // 将原vector的内容拷贝到新vector
            new_finish = uninitialized_copy(start, position, new_start);
            // 为新元素设定初值x
            construct(new_finish, x);
            // 调整水位
            ++new_finish;
            // 将原vector备用空间的内容也拷过来(侯捷: 啥用途?)
            new_finish = uninitialized_copy(position, finish, new_finish);
        } catch (...) {
            // "commit or rollback" semantics
            destory(new_start, new_finish);
            data_allocate::deallocate(new_start, len);
            throw;
        }
        
        // 析构并释放原vector
        destory(begin(), end());
        deallocate();

        // 调整迭代器, 指向vector
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start+ len;
    }
}

// 从position开始, 插入n个元素, 元素值为x
template <class T, class Alloc>
inline void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
    if (n != 0) { // 当 n != 0 才进行以下所有操作
        if (size_type(end_of_storage - finish) >= n) {
            // 备用空间大于等于"新增元素个数"
            T x_copy  x;
            // 以下计算插入点之后的现有元素个数
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            if (elems_after > n) { // "插入点之后的现有元素个数"大于"新增元素大小"
                uninitialized_copy(finish - n, finish, finish);
                finish += n; // 将vector尾端后移
                copy_backward(position, old_finish - n, old_finish);
                fill(position, position + n, x_copy); // 从插入点开始填入新值
            } else { // "插入点之后的现有元素个数"小于等于"新增元素大小"
                uninitialized_fill_n(finish, n - elems_after, x_copy);
                finish += n - elems_after;
                uninitialzied_copy(position, old_finish, finish);
                finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        } else {
            // 备用空间小于"新增元素个数"（那就必须配置额外的内存）
            // 首先决定新长度: 旧长度的两倍, 或旧长度+新增元素个数
            const size_type old_szie = size();
            const size_type len = old_size + max(old_size, n);
            // 以下配置新的vector空间
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            __STL_TRY {
                // 以下首先将旧的vector的插入点之前的元素复制到新空间
                new_finish = uninitialized_copy(start, position, new_start);
                // 以下再将新增元素(初值皆为n)填入新空间
                new_finish = uninitialized_fill_n(new_finish, n, x);
                // 以下再将旧vector的插入点之后的元素复制到新空间
                new_finish = uninitialized_copy(position, finish, new_finish);
            }

            #ifdef __STL_USE_EXCEPTIONS
            catch (...) {
                // 如有异常发生, 实现"commit or rollback" semantics
                destory(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            #endif // __STL_USE_EXCEPTIONS

            // 以下清除并释放旧的vector
            destory(start, finish);
            deallocate();
            // 以下调整水位标记
            start = new_start;
            finish = new_finish;
            end_of_storage = new _start + len;
        }
    }
}

#endif // SGI_STL_VECTOR_H