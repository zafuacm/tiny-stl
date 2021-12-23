#ifndef TSTL_SRC_VECTOR_HPP
#define TSTL_SRC_VECTOR_HPP

#include "iterator.hpp"
#include "algorithm.hpp"
#include <limits>

namespace tstl {

template <class T, class Allocator = std::allocator<T>>
class vector {
  private:
    using alloc_traits = std::allocator_traits<Allocator>;

  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = typename Allocator::pointer;
    using const_pointer = typename Allocator::const_pointer;
    using iterator = tstl::_normal_iterator<pointer, vector>;
    using const_iterator = tstl::_normal_iterator<const_pointer, vector>;
    using reverse_iterator = tstl::reverse_iterator<iterator>;
    using const_reverse_iterator = tstl::reverse_iterator<const_iterator>;

    /**
     * @brief 默认构造函数。构造拥有默认构造的分配器的空容器。
     */
    vector() = default;

    /**
     * @brief 构造拥有给定分配器 alloc 的空容器。
     */
    explicit vector(const Allocator &alloc) : m_alloc(alloc) {
    }

    /**
     * @brief 构造拥有 count 个有值 value 的元素的容器。
     */
    vector(size_type count, const T &value, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
        m_create_storage(count);
        m_fill_init(count, value);
    }

    /**
     * @brief 构造拥有个 count 默认插入的 T 实例的容器。不进行复制。
     */
    explicit vector(size_type count, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
        m_create_storage(count);
        m_default_init(count);
    }

    template <class InputIt, typename = tstl::_RequireInputIter<InputIt>>
    vector(InputIt first, InputIt last, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
        m_range_init(first, last, tstl::_iterator_category(first));
    }

    /**
     * @brief 复制构造函数。构造拥有 other 内容的容器。
     */
    vector(const vector &other)
        : vector(other,
                 alloc_traits::select_on_container_copy_construction(other.get_allocator())) {
    }

    /**
     * @brief 构造拥有 other 内容的容器，以 alloc 为分配器。
     */
    vector(const vector &other, const Allocator &alloc) : m_alloc(alloc) {
        size_type count = other.size();
        m_create_storage(count);
        m_finish = m_uninitialized_copy(other.begin(), other.end(), m_start);
    }

    /**
     * @brief 移动构造函数。用移动语义构造拥有 other 内容的容器。分配器通过属于 other
     * 的分配器移动构造获得。
     */
    vector(vector &&other) = default;

    vector(vector &&other, const Allocator &alloc) {
        size_type count = other.size();
        m_create_storage(count);
        m_finish = m_uninitialized_copy(other.begin(), other.end(), m_start);
    }

    vector(std::initializer_list<T> init, const Allocator &alloc = Allocator())
        : vector(init.begin(), init.end(), alloc) {
        m_range_init(init.begin(), init.end(), tstl::random_access_iterator_tag());
    }

    ~vector() {
        m_destroy(m_start);
        m_deallocate(m_start, capacity());
    }

    vector &operator=(const vector &other) {
        if (&other == this)
            return *this;
        const size_type len = other.size();
        if (len > capacity()) {
            pointer tmp = m_allocate_and_copy(len, other.begin(), other.end());
            m_destroy(m_start, m_end_of_storage);
            m_deallocate(m_start, m_end_of_storage - m_start);
            m_start = tmp;
            m_end_of_storage = m_start + len;
        } else if (size() >= len) {
            iterator i = tstl::copy(other.begin(), other.end(), begin());
            m_destroy(i.base(), m_finish);
        }
    }

    vector &operator=(vector &&other) {
        m_move_assign(std::move(other));
        return *this;
    }
    vector &operator=(std::initializer_list<T> ilist) {
        m_assign_aux(ilist.begin(), ilist.end(), random_access_iterator_tag());
        return *this;
    }

    void assign(size_type count, const T &value) {
        m_fill_assign(count, value);
    }

    template <class InputIt, typename = tstl::_RequireInputIter<InputIt>>
    void assign(InputIt first, InputIt last) {
        _M_assign_dispatch(first, last, tstl::false_type());
    }

    void assign(std::initializer_list<T> ilist) {
        m_assign_aux(ilist.begin(), ilist.end(), random_access_iterator_tag());
    }

    /**
     * @brief 返回与容器关联的分配器。
     */
    allocator_type get_allocator() const {
        return allocator_type(m_alloc);
    }

    /**
     * @brief 返回位于指定位置 pos 的元素的引用，有边界检查。
     * 若 pos 不在容器范围内，则抛出 std::out_of_range 类型的异常。
     */
    reference at(size_type pos) {
        m_range_check(pos);
        return m_start[pos];
    }

    const_reference at(size_type pos) const {
        m_range_check(pos);
        return m_start[pos];
    }

    /**
     * @brief 返回位于指定位置 pos 的元素的引用。不进行边界检查。
     */
    reference operator[](size_type pos) {
        return m_start[pos];
    }

    const_reference operator[](size_type pos) const {
        return m_start[pos];
    }

    /**
     * @brief 返回到容器首元素的引用。
     */
    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *begin();
    }

    /**
     * @brief 返回到容器中最后一个元素的引用。
     */
    reference back() {
        return *(end() - 1);
    }

    const_reference back() const {
        return *(end() - 1);
    }

    /**
     * @brief 返回指向作为元素存储工作的底层数组的指针。
     */
    T *data() {
        return m_start;
    }

    const T *data() const {
        return m_start;
    }

    /**
     * @brief 返回指向 vector 首元素的迭代器。
     */
    iterator begin() {
        return iterator(m_start);
    }
    const_iterator begin() const {
        return const_iterator(m_start);
    }

    const_iterator cbegin() const {
        return iterator(m_start);
    }

    /**
     * @brief 返回指向 vector 末元素后一元素的迭代器。
     */
    iterator end() {
        return iterator(m_finish);
    }

    const_iterator end() const {
        return const_iterator(m_finish);
    }

    const_iterator cend() const {
        return const_iterator(m_finish);
    }

    /**
     * @brief 返回指向逆向 vector 首元素的逆向迭代器。
     */
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(m_end_of_storage);
    }

    const_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * @brief 返回指向逆向 vector 末元素后一元素的逆向迭代器。
     */
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief 检查容器是否无元素。
     */
    bool empty() const {
        return begin() == end();
    }

    /**
     * @brief 返回容器中的元素数。
     */
    size_type size() const {
        return tstl::distance(m_start, m_finish);
    }

    /**
     * @brief 返回根据系统或库实现限制的容器可保有的元素最大数量。
     */
    size_type max_size() const {
        size_type diff_max = std::numeric_limits<difference_type>::max();
        size_type alloc_max = alloc_traits::max_size();
        return tstl::min(diff_max, alloc_max);
    }

    void reserve(size_type new_cap) {
        if (capacity() >= new_cap)
            return;
        const size_type old_size = size();
        iterator tmp = m_allocate_and_copy(new_cap, m_start, m_finish);
        m_destroy(m_start, m_finish);
        m_deallocate(m_start, m_end_of_storage - m_start);
        m_start = tmp;
        m_finish = tmp + old_size;
        m_end_of_storage = m_start + new_cap;
    }

    size_type capacity() const {
        return tstl::distance(m_start, m_end_of_storage);
    }

    void shrink_to_fit() {
        // TODO
    }

    void clear() {
        erase(begin(), end());
    }

    iterator insert(const_iterator pos, const T &value) {
        const size_type len = pos - cbegin();
        if (m_finish != m_end_of_storage) {
            if (pos == end()) {
                alloc_traits::construct(m_alloc, m_finish, value);
                ++m_finish;
            } else {
                m_insert_aux(begin(), value);
            }
        } else {
            m_realloc_insert(begin() + len, value);
        }
        return iterator(m_start + len);
    }

    iterator insert(const_iterator pos, T &&value) {
        return m_insert_rval(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T &value) {
        m_fill_insert(pos, count, value);
        return pos;
    }

    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args) {
        return m_emplace_aux(pos, std::forward<Args>(args)...);
    }

    iterator erase(const_iterator pos) {
        if (pos + 1 != end()) {
            std::move(pos + 1, m_finish, pos);
        }
        --m_finish;
        m_destroy(m_finish);
        return pos;
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first != last) {
            if (last != end()) {
                std::move(last, end(), first);
            }
        }
    }

    void push_back(const T &value) {
        emplace_back(value);
    }

    void push_back(const T &&value) {
        emplace_back(value);
    }

    template <class... Args>
    void emplace_back(Args &&...args) {
        if (m_finish != m_end_of_storage) {
            alloc_traits::construct(m_alloc, m_finish, std::forward<Args>(args)...);
            m_finish++;
        } else {
            m_realloc_insert(end(), std::forward<Args>(args)...);
        }
    }

    void pop_back() {
        --m_finish;
        m_destroy(m_finish);
    }

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

    void resize(size_type count) {
        resize(count, T());
    }

    void resize(size_type count, const value_type &value) {
        if (count > size()) {
            insert(end(), count - size(), value);
        } else {
            m_erase_at_end(begin() + count);
        }
    }

    void swap(vector &other) {
        tstl::swap(m_start, other.m_start);
        tstl::swap(m_finish, other.finish);
        tstl::swap(m_end_of_storage, other.m_end_of_storage);
    }

    friend void swap(vector &lhs, vector &rhs) {
        lhs.swap(rhs);
    }

  private:
    pointer m_start = pointer();
    pointer m_finish = pointer();
    pointer m_end_of_storage = pointer();
    Allocator m_alloc;

    pointer m_allocate(size_type count) {
        if (count != 0)
            return alloc_traits::allocate(m_alloc, count);
        return pointer();
    }

    void m_deallocate(pointer p, size_type count) {
        if (p != nullptr)
            alloc_traits::deallocate(m_alloc, p, count);
    }

    void m_destroy(pointer p) {
        alloc_traits::destroy(m_alloc, p);
    }

    void m_destroy(pointer first, pointer last) {
        for (; first != last; ++first) {
            m_destroy(first);
        }
    }

    void m_construct(pointer p, const value_type &value) {
        alloc_traits::construct(m_alloc, p, value);
    }

    void m_create_storage(size_type count) {
        m_finish = m_start = m_allocate(count);
        m_end_of_storage = m_start + count;
    }

    void m_default_init(size_type count) {
        m_finish = std::__uninitialized_default_n_a(m_start, count, m_alloc);
    }

    void m_fill_init(size_type count, const value_type &value) {
        m_finish = std::__uninitialized_fill_n_a(m_start, count, value, m_alloc);
    }

    template <typename InputIter, typename ForwardIter>
    pointer m_uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
        return std::__uninitialized_copy_a(first, last, result, m_alloc);
    }

    template <class Arg>
    void m_insert_aux(iterator pos, Arg &&arg) {
        alloc_traits::construct(m_alloc, m_finish, std::move(*(m_finish - 1)));
        ++m_finish;
        tstl::move_backward(pos.base(), m_finish - 2, m_finish - 1);
        *pos = std::forward<Arg>(arg);
    }

    void m_erase_at_end(pointer pos) {
        if (m_finish != pos) {
            m_destroy(pos, m_finish);
            m_finish = pos;
        }
    }

    iterator m_insert_rval(const_iterator pos, T &&value) {
        int len = pos - cbegin();
        if (m_finish != m_end_of_storage) {
            if (pos == cend()) {
                alloc_traits::construct(m_alloc, m_finish, std::move(value));
                ++m_finish;
            } else {
                m_insert_aux(begin() + len, std::move(value));
            }
        } else {
            m_realloc_insert(begin() + len, std::move(value));
        }
        return iterator(m_start + len);
    }

    template <typename InputIt>
    void m_assign_aux(InputIt first, InputIt last, input_iterator_tag) {
        pointer cur = m_start;
        for (; first != last && cur != m_finish; ++cur, (void)++first) {
            *cur = *first;
        }
        if (first == last) {
            m_erase_at_end(cur);
        } else {
            _M_range_insert(end(), first, last, tstl::_iterator_category(first));
        }
    }

    template <typename ForwardIt>
    void m_assign_aux(ForwardIt first, ForwardIt last, forward_iterator_tag) {
        const size_type len = tstl::distance(first, last);
        if (len > capacity()) {
            pointer tmp = m_allocate_and_copy(len, first, last);
            m_destroy(m_start, m_finish);
            m_deallocate(m_start, m_end_of_storage - m_start);
            m_start = tmp;
            m_end_of_storage = m_finish = m_start + len;
        } else if (size() >= len) {
            pointer i = std::copy(first, last, m_start);
            m_erase_at_end(i);
        } else {
            ForwardIt mid = first;
            tstl::advance(mid, size());
            std::copy(first, mid, m_start);
            m_finish = m_uninitialized_copy(mid, last, m_finish);
        }
    }

    void m_fill_assign(size_type count, const value_type &value) {
        if (count > capacity()) {
            vector tmp(count, value, m_alloc);
            tmp.m_swap_data(*this);
        } else if (count > size()) {
            tstl::fill(begin(), end(), value);
            const size_type add = count - size();
            m_finish = std::__uninitialized_fill_n_a(m_finish, add, value, m_alloc);
        } else {
            iterator i = tstl::fill_n(m_start, count, value);
            m_erase_at_end(i);
        }
    }

    void m_swap_data(vector &other) {
        tstl::swap(m_start, other.m_start);
        tstl::swap(m_finish, other.m_finish);
        tstl::swap(m_end_of_storage, other.m_end_of_storage);
    }

    void m_move_assign(vector &&other) {
        vector tmp(get_allocator());
        m_swap_data(other);
        tmp.m_swap_data(other);
        std::__alloc_on_move(m_alloc, other.m_alloc);
    }

    iterator m_emplace_aux(const_iterator pos, T &&value) {
        return m_insert_rval(pos, std::move(value));
    }

    template <class... Args>
    void m_realloc_insert(iterator pos, Args &&...args) {
        const size_type len = size() * 2;
        pointer old_start = m_start;
        pointer old_finish = m_finish;
        const size_type elems_before = pos - begin();
        pointer new_start = m_allocate(len);
        pointer new_finish = new_start;
        try {
            alloc_traits::construct(m_alloc, new_start + elems_before, std::forward<Args>(args)...);
            new_finish = pointer();
            new_finish = m_uninitialized_copy(old_start, pos.base(), new_start);
            ++new_finish;
            new_finish = m_uninitialized_copy(pos.base(), old_finish, new_finish);
        } catch (...) {
            if (new_finish == nullptr) {
                m_destroy(new_start, new_start + elems_before);
            } else {
                m_destroy(new_start, new_finish);
            }
            m_deallocate(new_start, len);
            throw;
        }
        m_destroy(old_start, old_finish);
        m_deallocate(old_start, m_end_of_storage - old_start);
        m_start = new_start;
        m_finish = new_finish;
        m_end_of_storage = new_start + len;
    }

    template <class InputIt>
    void m_range_init(InputIt first, InputIt last, tstl::input_iterator_tag) {
        try {
            for (; first != last; first++) {
                emplace_back(*first);
            }
        } catch (...) {
            clear();
        }
    }

    template <class ForwardIt>
    void m_range_init(ForwardIt first, ForwardIt last, tstl::forward_iterator_tag) {
        size_type count = tstl::distance(first, last);
        m_start = m_allocate(count);
        m_end_of_storage = m_start + count;
        m_finish = m_uninitialized_copy(first, last, m_start);
    }

    template <class ForwardIt>
    pointer m_allocate_and_copy(size_type n, ForwardIt first, ForwardIt last) {
        pointer result = m_allocate(n);
        try {
            m_uninitialized_copy(first, last, result);
            return result;
        } catch (...) {
            m_deallocate(result, n);
            throw;
        }
    }

    /**
     * @brief 越界检查，仅被 at() 使用。
     */
    void m_range_check(size_type pos) {
        if (pos >= size()) {
            std::__throw_out_of_range_fmt(
                "vector::m_range_check: pos (which is %zu) >= size() (which is %zu)", pos, size());
        }
    }
};

} // namespace tstl

#endif
