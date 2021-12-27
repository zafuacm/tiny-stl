#ifndef TSTL_SRC_CONSTRUCT_HPP
#define TSTL_SRC_CONSTRUCT_HPP

#include <cstddef>
#include "../iterator.hpp"
#include <memory>

namespace tstl {

template <class T, class... Args>
T *construct_at(T *p, Args &&...args) {
    return ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
}

template <class T>
void destroy_at(T *p) {
    p->~T();
}

template <class ForwardIt>
void destroy(ForwardIt first, ForwardIt last) {
    for (; first != last; ++first) {
        tstl::destroy_at(std::addressof(*first));
    }
}

template <class ForwardIt, class Allocator>
void _destroy_a(ForwardIt first, ForwardIt last, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    for (; first != last; ++first) {
        alloc_traits::destroy(alloc, std::addressof(*first));
    }
}

} // namespace tstl

#endif
