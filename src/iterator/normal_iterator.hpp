#ifndef TSTL_SRC_ITERATOR_NORMAL_ITERATOR_HPP
#define TSTL_SRC_ITERATOR_NORMAL_ITERATOR_HPP

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
        return current[n];
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

#endif
