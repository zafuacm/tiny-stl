#ifndef TSTL_SRC_VECTOR_CPP
#define TSTL_SRC_VECTOR_CPP

#include <iterator>
#include <memory>

namespace tstl {

template <typename T, class Allocator = std::allocator<T>>
class vector {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = typename Allocator::pointer;
    using const_pointer = typename Allocator::const_pointer;
    using iterator = typename __gnu_cxx::__normal_iterator<pointer, vector>;
    using const_iterator =
        typename __gnu_cxx::__normal_iterator<const_pointer, vector>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() {
        vector(0);
    }

    vector(size_type count,
           const T &value = T(),
           const Allocator &allocator = Allocator()) {
        alloc = allocator;
        m_data = alloc.allocate(count);
        m_size = count;
        m_start = m_data;
        m_end = m_finish = m_data + count;
    }

    ~vector() {
        alloc.destroy(m_data);
        alloc.deallocate(m_data, m_size);
    }

    vector(const vector &other);

    vector(vector &&other);

    vector(std::initializer_list<T> init, const Allocator &alloc = Allocator());

    vector &operator=(const vector &other);
    vector &operator=(vector &&other);
    vector &operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T &value);

    void assign(std::initializer_list<T> ilist);

    allocator_type get_allocator() const {
        return Allocator();
    }

    reference at(size_type pos);

    const_reference at(size_type pos) const;

    reference operator[](size_type pos) {
        return m_data[pos];
    }

    const_reference operator[](size_type pos) const {
        return m_data[pos];
    }

    reference back();

    const_reference back() const;

    T *data();

    const T *data() const;

    iterator begin() {
        return iterator(m_start);
    }

    const_iterator begin() const {
        return const_iterator(m_start);
    }

    const_iterator cbegin() const {
        return iterator(m_start);
    }

    iterator end() {
        return iterator(m_end);
    }

    const_iterator end() const {
        return const_iterator(m_end);
    }

    const_iterator cend() const {
        return const_iterator(m_end);
    }

    reverse_iterator rbegin() {
        reverse_iterator(end());
    }

    const_iterator rbegin() const {
        const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(m_end);
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }

    bool empty() const;

    size_type size() const {
        return m_size;
    }

    size_type max_size() const;

    void reserve(size_type new_cap);

    size_type capacity() const;

    void shrink_to_fit();

    void clear();

    iterator insert(iterator pos, const T &value);

    iterator insert(const_iterator pos, const T &value);

    void insert(iterator pos, size_type count, const T &value);

    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args);

    iterator erase(iterator pos);

    iterator erase(const_iterator pos);

    iterator erase(iterator first, iterator last);

    iterator erase(const_iterator first, const_iterator last);

    void push_back(const T &value);

    template <class... Args>
    void emplace_back(Args &&...args);

    template <class... Args>
    reference emplace_back(Args &&...args);

    void pop_back();

    friend bool operator==(const vector &lhs, const vector &rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        auto lhs_i = lhs.begin(), rhs_i = rhs.begin();
        while (lhs_i != lhs.end() && rhs_i != rhs.end()) {
            if (*lhs_i != *rhs_i) {
                return false;
            }
            lhs_i++, rhs_i++;
        }
        return true;
    }

    void resize(size_type count);

    void resize(size_type count, T value = T());

    void resize(size_type count, const value_type &value);

    void swap(vector &other);

    friend void swap(vector &lhs, vector &rhs);

  private:
    pointer m_data;
    size_type m_size;
    pointer m_start;
    pointer m_finish;
    pointer m_end;
    Allocator alloc;
};

} // namespace tstl

#endif
