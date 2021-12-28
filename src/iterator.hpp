#ifndef TSTL_SRC_ITERATOR_HPP
#define TSTL_SRC_ITERATOR_HPP

#include <memory>
#include "type_traits.hpp"

namespace tstl {

struct input_iterator_tag {};

struct output_iterator_tag {};

struct forward_iterator_tag : public input_iterator_tag {};

struct bidirectional_iterator_tag : public forward_iterator_tag {};

struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category,
          class T,
          class Distance = std::ptrdiff_t,
          class Pointer = T *,
          class Reference = T &>
struct iterator {
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using refence = Reference;
};

template <typename Iterator, typename = tstl::_void_t<>>
struct _iterator_traits {};

template <class Iter>
struct _iterator_traits<Iter,
                        tstl::_void_t<typename Iter::iterator_category,
                                      typename Iter::value_type,
                                      typename Iter::difference_type,
                                      typename Iter::pointer,
                                      typename Iter::reference>> {
    using difference_type = typename Iter::difference_type;
    using value_type = typename Iter::value_type;
    using pointer = typename Iter::pointer;
    using reference = typename Iter::reference;
    using iterator_category = typename Iter::iterator_category;
};

template <typename Iterator>
struct iterator_traits : public _iterator_traits<Iterator> {};

template <class T>
struct iterator_traits<T *> {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = random_access_iterator_tag;
};

template <class T>
struct iterator_traits<const T *> {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;
    using iterator_category = random_access_iterator_tag;
};

#include "iterator/reverse_iterator.hpp"

#include "iterator/normal_iterator.hpp"

namespace detail {

template <class Iter>
typename iterator_traits<Iter>::difference_type do_distance(Iter first, Iter last, input_iterator_tag) {
    typename iterator_traits<Iter>::difference_type result = 0;
    while (first != last) {
        ++first;
        ++result;
    }
    return result;
}

template <class Iter>
typename iterator_traits<Iter>::difference_type
do_distance(Iter first, Iter last, random_access_iterator_tag) {
    return last - first;
}

template <class Iter, class Distance>
void do_advance(Iter &it, Distance n, input_iterator_tag) {
    while (n > 0) {
        --n;
        ++it;
    }
}

template <class Iter, class Distance>
void do_advance(Iter &it, Distance n, bidirectional_iterator_tag) {
    while (n > 0) {
        --n;
        ++it;
    }
    while (n < 0) {
        ++n;
        --it;
    }
}

template <class Iter, class Distance>
void do_advance(Iter &it, Distance n, random_access_iterator_tag) {
    it += n;
}

} // namespace detail

template <class Iter, class Distance>
void advance(Iter &it, Distance n) {
    typename iterator_traits<Iter>::difference_type d = n;
    detail::do_advance(it,
                       typename iterator_traits<Iter>::difference_type(n),
                       typename iterator_traits<Iter>::iterator_category());
}

template <typename Iter>
using _iterator_category_t = typename iterator_traits<Iter>::iterator_category;

template <class Iter>
typename iterator_traits<Iter>::iterator_category _iterator_category(const Iter &) {
    return _iterator_category_t<Iter>();
}

template <class Iter>
typename iterator_traits<Iter>::difference_type distance(Iter first, Iter last) {
    return detail::do_distance(first, last, _iterator_category_t<Iter>());
}

template <typename InputIter>
using _RequireInputIter = std::enable_if_t<
    std::is_convertible<_iterator_category_t<InputIter>, input_iterator_tag>::value>;

} // namespace tstl

#endif
