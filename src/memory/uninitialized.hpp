#ifndef TSTL_SRC_UNINITIALIZED_HPP
#define TSTL_SRC_UNINITIALIZED_HPP

#include <cstddef>
#include "construct.hpp"
#include "../iterator.hpp"
#include <memory>

namespace tstl {

template <class ForwardIt>
ForwardIt uninitialized_default_construct(ForwardIt first, ForwardIt last) {
    ForwardIt cur = first;
    try {
        for (; cur != last; ++cur) {
            tstl::construct_at(std::addressof(*cur));
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class ForwardIt, class Size>
ForwardIt uninitialized_default_construct_n(ForwardIt first, Size n) {
    ForwardIt cur = first;
    try {
        for (; n > 0; ++cur, --n) {
            tstl::construct_at(std::addressof(*cur));
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class ForwardIt, class T>
ForwardIt uninitialized_fill(ForwardIt first, ForwardIt last, const T &value) {
    ForwardIt cur = first;
    try {
        for (; cur != last; ++cur) {
            tstl::construct_at(std::addressof(*cur), value);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class ForwardIt, class Size, class T>
ForwardIt uninitialized_fill_n(ForwardIt first, Size n, const T &value) {
    ForwardIt cur = first;
    try {
        for (; n > 0; ++cur, --n) {
            tstl::construct_at(std::addressof(*cur), value);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first) {
    ForwardIt cur = first, d_cur = d_first;
    try {
        for (; cur != last; ++cur, ++d_cur) {
            tstl::construct_at(std::addressof(*d_cur), *cur);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class InputIt, class Size, class ForwardIt>
ForwardIt uninitialized_copy_n(InputIt first, Size n, ForwardIt d_first) {
    ForwardIt cur = first, d_cur = d_first;
    try {
        for (; n > 0; ++cur, ++d_cur, --n) {
            tstl::construct_at(std::addressof(*d_cur), *cur);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class InputIt, class ForwardIt>
ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt d_first) {
    ForwardIt cur = first, d_cur = d_first;
    try {
        for (; cur != last; ++cur, ++d_cur) {
            tstl::construct_at(std::addressof(*d_cur), std::move(*cur));
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class ForwardIt, class Allocator>
ForwardIt _uninitialized_default_construct_a(ForwardIt first, ForwardIt last, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    ForwardIt cur = first;
    try {
        for (; cur != last; ++cur) {
            alloc_traits::construct(alloc, std::addressof(*cur));
        }
    } catch (...) {
        tstl::_destroy_a(first, cur, alloc);
        throw;
    }
    return cur;
}

template <class ForwardIt, class Size, class Allocator>
ForwardIt _uninitialized_default_construct_n_a(ForwardIt first, Size n, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    ForwardIt cur = first;
    try {
        for (; n > 0; ++cur, --n) {
            alloc_traits::construct(alloc, std::addressof(*cur));
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class ForwardIt, class T, class Allocator>
ForwardIt _uninitialized_fill_a(ForwardIt first, ForwardIt last, const T &value, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    ForwardIt cur = first;
    try {
        for (; cur != last; ++cur) {
            alloc_traits::construct(alloc, std::addressof(*cur), value);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class ForwardIt, class Size, class T, class Allocator>
ForwardIt _uninitialized_fill_n_a(ForwardIt first, Size n, const T &value, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    ForwardIt cur = first;
    try {
        for (; n > 0; ++cur, --n) {
            alloc_traits::construct(alloc, std::addressof(*cur), value);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return cur;
}

template <class InputIt, class ForwardIt, class Allocator>
ForwardIt _uninitialized_copy_a(InputIt first, InputIt last, ForwardIt d_first, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    InputIt cur = first;
    ForwardIt d_cur = d_first;
    try {
        for (; cur != last; ++cur, ++d_cur) {
            alloc_traits::construct(alloc, std::addressof(*d_cur), *cur);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return d_cur;
}

template <class InputIt, class Size, class ForwardIt, class Allocator>
ForwardIt _uninitialized_copy_n_a(InputIt first, Size n, ForwardIt d_first, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    ForwardIt cur = first, d_cur = d_first;
    try {
        for (; n > 0; ++cur, ++d_cur, --n) {
            alloc_traits::construct(alloc, std::addressof(*d_cur), *cur);
        }
    } catch (...) {
        tstl::destroy(first, cur);
        throw;
    }
    return d_cur;
}

template <class InputIt, class ForwardIt, class Allocator>
ForwardIt _uninitialized_move_a(InputIt first, InputIt last, ForwardIt d_first, Allocator &alloc) {
    using alloc_traits = std::allocator_traits<Allocator>;
    InputIt cur = first;
    ForwardIt d_cur = d_first;
    try {
        for (; cur != last; ++cur, ++d_cur) {
            alloc_traits::construct(alloc, std::addressof(*d_cur), std::move(*cur));
        }
        return d_cur;
    } catch (...) {
        tstl::destroy(d_first, d_cur);
        throw;
    }
}

} // namespace tstl

#endif
