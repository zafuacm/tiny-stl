#ifndef TSTL_SRC_ITERATOR_REVERSE_ITERATOR_HPP
#define TSTL_SRC_ITERATOR_REVERSE_ITERATOR_HPP

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
        reverse_iterator tmp = *this;
        --current;
        return tmp;
    }

    reverse_iterator operator--(int) {
        reverse_iterator tmp = *this;
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

#endif
