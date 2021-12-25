#ifndef TSTL_SRC_CONSTRUCT_HPP
#define TSTL_SRC_CONSTRUCT_HPP
#include <new>
#include <climits>
#include <iostream>

// 这个文件的内容是实现一个分配器（allocator）

namespace tstl {

template <class T>
// 分配空间 （大小 + 类型）
inline T *_allocatr(ptrdiff_t size, T *) {
    std::set_new_handler(0); // new 操作失败时的处理函数
    // static_cast<T> 用于一种显示的良性类型转换
    T *tmp = static_cast<T *>(::operator new((size_t)(size * sizeof(T))));
    if (tmp == NULL) { // 内存分配失败
        std::cerr << "out of memory" << std::endl;
        exit(-1);
    }
    return tmp;
}
// 释放空间（只需要指针，因为分配空间的cookie中有记录）
template <class T>
inline void _deallocate(T *buffer) {
    ::operator delete(buffer);
}

template <class T1, class T2>
inline void _construct(T1 *p, const T2 &value) {
    new (p) T1(value);
}

template <class T>
inline void _destory(T *ptr) {
    ptr->~T();
}
// 分配器模板类
template <class T>
class allocator {
  public:
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

    pointer allocate(size_type n, void *hint = nullptr) {
        return _allocatr(static_cast<difference_type>(n), static_cast<pointer>(nullptr));
    }

    void deallocate(pointer p, size_type n) {
        // 第二个参数其实根本没有用到，因为内存分配所带的cookie里面包含了这些信息
        _deallocate(p);
    }

    void construct(pointer p, const T &value) {
        _construct(p, value);
    }

    void destory(pointer p) {
        _destory(p);
    }

    pointer address(reference x) {
        return static_cast<pointer>(&x);
    }

    const_pointer const_address(const_reference x) {
        return static_cast<const_pointer>(&x);
    }

    size_type max_size() const {
        return size_type(UINT_MAX / sizeof(T));
    }
};

} // namespace tstl

#endif