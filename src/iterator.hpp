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

template <class Iter>
class reverse_iterator : public iterator<typename iterator_traits<Iter>::iterator_category,
                                         typename iterator_traits<Iter>::value_type,
                                         typename iterator_traits<Iter>::difference_type,
                                         typename iterator_traits<Iter>::pointer,
                                         typename iterator_traits<Iter>::reference> {
  private:
    Iter current;
    using traits_type = iterator_traits<Iter>;

  public:
    using iterator_type = Iter;
    using difference_type = typename traits_type::difference_type;
    using pointer = typename traits_type::pointer;
    using reference = typename traits_type::reference;

    reverse_iterator() : current() {
    }

    explicit reverse_iterator(iterator_type x) : current(x) {
    }

    template <class U>
    reverse_iterator(const reverse_iterator<U> &other) : current(other.current) {
    }

    iterator_type base() const {
        return current;
    }

    reference operator*() const {
        Iter tmp = current;
        return *--tmp;
    }

    pointer operator->() const {
        return std::addressof(operator*());
    }

    reverse_iterator &operator++() {
        --current;
        return *this;
    }

    reverse_iterator &operator--() {
        ++current;
        return *this;
    }

    reverse_iterator operator++(int) {
        Iter tmp = *this;
        --current;
        return tmp;
    }

    reverse_iterator operator--(int) {
        Iter tmp = *this;
        ++current;
        return tmp;
    }

    reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(base() - n);
    }

    reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(base() + n);
    }

    reverse_iterator &operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    reverse_iterator &operator-=(difference_type n) {
        current += n;
        return *this;
    }

    reverse_iterator &operator=(const reverse_iterator &) = default;

    reverse_iterator make_reverse_iterator(Iter i) {
        return reverse_iterator(i);
    }

    reverse_iterator &operator[](difference_type n) const {
        return base()[-n - 1];
    }

    friend bool operator==(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return lhs.base() == rhs.base();
    }

    friend bool operator!=(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return lhs.base() != rhs.base();
    }

    friend bool operator<(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return lhs.base() > rhs.base();
    }

    friend bool operator<=(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return lhs.base() >= rhs.base();
    }

    friend bool operator>(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return lhs.base() < rhs.base();
    }

    friend bool operator>=(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return lhs.base() <= rhs.base();
    }

    friend reverse_iterator operator+(difference_type n, const reverse_iterator &it) {
        return reverse_iterator(it.base() - n);
    }

    friend difference_type operator-(const reverse_iterator &lhs, const reverse_iterator &rhs) {
        return rhs.base() - lhs.base();
    }
};

template <class Iter, class Container>
class _normal_iterator {
  private:
    Iter current;
    using traits_type = iterator_traits<Iter>;

  public:
    using iterator_type = Iter;
    using difference_type = typename traits_type::difference_type;
    using value_type = typename traits_type::value_type;
    using iterator_category = typename traits_type::iterator_category;
    using pointer = typename traits_type::pointer;
    using reference = typename traits_type::reference;

    _normal_iterator() : current() {
    }

    explicit _normal_iterator(iterator_type x) : current(x) {
    }

    _normal_iterator(const _normal_iterator<typename Container::pointer, Container> &i)
        : current(i.base()) {
    }

    _normal_iterator(const _normal_iterator<typename Container::const_pointer, Container> &i)
        : current(i.base()) {
    }

    iterator_type base() const {
        return current;
    }

    reference operator*() const {
        return *current;
    }

    pointer operator->() const {
        return current;
    }

    _normal_iterator &operator++() {
        ++current;
        return *this;
    }

    _normal_iterator &operator--() {
        --current;
        return *this;
    }

    _normal_iterator operator++(int) {
        return _normal_iterator(current++);
    }

    _normal_iterator operator--(int) {
        return _normal_iterator(current--);
    }

    _normal_iterator operator+(difference_type n) const {
        return _normal_iterator(current + n);
    }

    _normal_iterator operator-(difference_type n) const {
        return _normal_iterator(current - n);
    }

    _normal_iterator &operator+=(difference_type n) {
        current += n;
        return *this;
    }

    _normal_iterator &operator-=(difference_type n) {
        current -= n;
        return *this;
    }

    _normal_iterator &operator=(const _normal_iterator &) = default;

    _normal_iterator &operator[](difference_type n) const {
        return base()[-n - 1];
    }

    friend bool operator==(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() == rhs.base();
    }

    friend bool operator!=(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() != rhs.base();
    }

    friend bool operator<(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() < rhs.base();
    }

    friend bool operator<=(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() <= rhs.base();
    }

    friend bool operator>(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() > rhs.base();
    }

    friend bool operator>=(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() >= rhs.base();
    }

    friend _normal_iterator operator+(difference_type n, const _normal_iterator &it) {
        return _normal_iterator(it.base() - n);
    }

    friend difference_type operator-(const _normal_iterator &lhs, const _normal_iterator &rhs) {
        return lhs.base() - rhs.base();
    }
};

namespace detail {

template <class It>
typename iterator_traits<It>::difference_type do_distance(It first, It last, input_iterator_tag) {
    typename iterator_traits<It>::difference_type result = 0;
    while (first != last) {
        ++first;
        ++result;
    }
    return result;
}

template <class It>
typename iterator_traits<It>::difference_type
do_distance(It first, It last, random_access_iterator_tag) {
    return last - first;
}

template <class It, class Distance>
void do_advance(It &it, Distance n, input_iterator_tag) {
    while (n > 0) {
        --n;
        ++it;
    }
}

template <class It, class Distance>
void do_advance(It &it, Distance n, bidirectional_iterator_tag) {
    while (n > 0) {
        --n;
        ++it;
    }
    while (n < 0) {
        ++n;
        --it;
    }
}

template <class It, class Distance>
void do_advance(It &it, Distance n, random_access_iterator_tag) {
    it += n;
}

} // namespace detail

template <class It, class Distance>
void advance(It &it, Distance n) {
    typename iterator_traits<It>::difference_type d = n;
    detail::do_advance(it,
                       typename iterator_traits<It>::difference_type(n),
                       typename iterator_traits<It>::iterator_category());
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
using _RequireInputIter = std::__enable_if_t<
    std::is_convertible<_iterator_category_t<InputIter>, tstl::input_iterator_tag>::value>;

} // namespace tstl

#endif
