#ifndef TSTL_SRC_DEQUE_HPP
#define TSTL_SRC_DEQUE_HPP

#include "iterator.hpp"
#include "memory/uninitialized.hpp"
#include "algorithm.hpp"
#include <limits>
#include <stdexcept>

#ifndef TSTL_DEQUE_BUF_SIZE
#define TSTL_DEQUE_BUF_SIZE 512
#endif

#ifndef TSTL_DEQUE_MAP_INIT_SIZE
#define TSTL_DEQUE_MAP_INIT_SIZE 8
#endif

namespace tstl {

template <class T>
struct _deque_buf_size {
    static constexpr size_t value =
        sizeof(T) < TSTL_DEQUE_BUF_SIZE ? TSTL_DEQUE_BUF_SIZE / sizeof(T) : 1;
};

template <class T, class Ref, class Ptr>
struct deque_iterator {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = Ref;
    using pointer = Ptr;
    using iterator_category = random_access_iterator_tag;

    using self = deque_iterator;
    using iterator = deque_iterator<T, T &, T *>;
    using const_iterator = deque_iterator<T, const T &, const T *>;
    using value_pointer = T *;
    using map_pointer = T **;

    static constexpr difference_type m_buffer_size = _deque_buf_size<T>::value;

    value_pointer m_cur = nullptr;
    value_pointer m_first = nullptr;
    value_pointer m_last = nullptr;
    map_pointer m_node = nullptr;

    deque_iterator() = default;

    deque_iterator(value_pointer off, map_pointer n)
        : m_cur(off), m_first(*n), m_last(*n + m_buffer_size), m_node(n) {
    }

    deque_iterator(const iterator &other)
        : m_cur(other.m_cur), m_first(other.m_first), m_last(other.m_last), m_node(other.m_node) {
    }

    deque_iterator(const const_iterator &other)
        : m_cur(other.m_cur), m_first(other.m_first), m_last(other.m_last), m_node(other.m_node) {
    }

    deque_iterator &operator=(const deque_iterator &other) = default;

    reference operator*() const {
        return *m_cur;
    }

    pointer operator->() const {
        return m_cur;
    }

    self &operator++() {
        ++m_cur;
        if (m_cur == m_last) {
            m_set_node(m_node + 1);
            m_cur = m_first;
        }
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self &operator--() {
        if (m_cur == m_first) {
            m_set_node(m_node - 1);
            m_cur = m_last;
        }
        --m_cur;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    self &operator+=(difference_type n) {
        const auto offset = n + (m_cur - m_first);
        if (offset >= 0 && offset < m_buffer_size) {
            m_cur += n;
        } else {
            difference_type node_offset = offset > 0
                                              ? offset / m_buffer_size
                                              : -(-offset - 1) / difference_type(m_buffer_size) - 1;
            m_set_node(m_node + node_offset);
            m_cur = m_first + (offset - node_offset * m_buffer_size);
        }
        return *this;
    }

    deque_iterator &operator-=(difference_type n) {
        return *this += -n;
    }

    deque_iterator operator+(difference_type n) const {
        deque_iterator tmp = *this;
        return tmp += n;
    }

    deque_iterator operator-(difference_type n) const {
        deque_iterator tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const self &other) const {
        difference_type diff = (m_cur - m_first) - (other.m_cur - other.m_first);
        return m_buffer_size * (m_node - other.m_node) + diff;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    friend bool operator==(const self &lhs, const self &rhs) {
        return lhs.m_cur == rhs.m_cur;
    }

    friend bool operator<(const self &lhs, const self &rhs) {
        if (lhs.m_node == rhs.m_node)
            return lhs.m_cur < rhs.m_cur;
        return lhs.m_node < rhs.m_node;
    }

    friend bool operator>(const self &lhs, const self &rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const self &lhs, const self &rhs) {
        return !(lhs > rhs);
    }

    friend bool operator>=(const self &lhs, const self &rhs) {
        return !(lhs < rhs);
    }

    friend bool operator!=(const self &lhs, const self &rhs) {
        return !(lhs == rhs);
    }

    void m_set_node(map_pointer new_node) {
        m_node = new_node;
        m_first = *new_node;
        m_last = m_first + m_buffer_size;
    }
};

template <class T, class Allocator = std::allocator<T>>
class deque {
  private:
    using alloc_traits = std::allocator_traits<Allocator>;

  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using iterator = deque_iterator<T, T &, T *>;
    using const_iterator = deque_iterator<T, const T &, const T *>;
    using reverse_iterator = tstl::reverse_iterator<iterator>;
    using const_reverse_iterator = tstl::reverse_iterator<const_iterator>;

  public:
    allocator_type get_allocator() const noexcept {
        return allocator_type(m_alloc);
    }

    deque() : m_map_alloc(m_alloc) {
        m_init_map(0);
    }

    explicit deque(const Allocator &alloc) : m_alloc(alloc), m_map_alloc(m_alloc) {
        m_init_map(0);
    }

    deque(size_type count, const T &value, const Allocator &alloc = Allocator())
        : m_alloc(alloc), m_map_alloc(m_alloc) {
        m_init_map(count);
        m_fill_init(value);
    }

    explicit deque(size_type count, const Allocator &alloc = Allocator())
        : m_alloc(alloc), m_map_alloc(m_alloc) {
        m_init_map(count);
        m_default_init();
    }

    template <class InputIt, typename = tstl::_RequireInputIter<InputIt>>
    deque(InputIt first, InputIt last, const Allocator &alloc = Allocator())
        : m_alloc(alloc), m_map_alloc(m_alloc) {
        m_range_init(first, last, tstl::_iterator_category(first));
    }

    deque(const deque &other)
        : deque(other, alloc_traits::select_on_container_copy_construction(other.get_allocator())) {
    }

    deque(const deque &other, const Allocator &alloc) : m_alloc(alloc), m_map_alloc(m_alloc) {
        m_init_map(other.size());
        tstl::_uninitialized_copy_a(other.begin(), other.end(), m_start, m_alloc);
    }

    deque(deque &&other) noexcept : m_alloc(std::move(other.m_alloc)), m_map_alloc(m_alloc) {
        m_init_map(0);
        m_swap_data(other);
    }

    deque(deque &&other, const Allocator &alloc) : deque(std::move(other), Allocator(alloc)) {
    }

    deque(std::initializer_list<T> init, const Allocator &alloc = Allocator())
        : m_alloc(alloc), m_map_alloc(m_alloc) {
        m_range_init(init.begin(), init.end(), tstl::random_access_iterator_tag());
    }

    deque &operator=(const deque &other) {
        if (&other == this)
            return *this;
        const size_type len = size();
        if (len >= other.size()) {
            iterator i = tstl::copy(other.begin(), other.end(), m_start);
            m_erase_at_end(i);
        } else {
            const_iterator mid = other.begin() + len;
            tstl::copy(other.begin(), mid, m_start);
            m_range_insert_aux(m_finish, mid, other.end(), tstl::random_access_iterator_tag());
        }
        return *this;
    }

    deque &operator=(deque &&other) noexcept {
        m_swap_data(other);
        return *this;
    }

    deque &operator=(std::initializer_list<T> ilist) {
        m_assign_aux(ilist.begin(), ilist.end(), tstl::random_access_iterator_tag());
        return *this;
    }

    void assign(size_type count, const T &value) {
        m_fill_assign(count, value);
    }

    template <class InputIt, typename = tstl::_RequireInputIter<InputIt>>
    void assign(InputIt first, InputIt last) {
        m_assign_aux(first, last, tstl::_iterator_category(first));
    }

    void assign(std::initializer_list<T> ilist) {
        m_assign_aux(ilist.begin(), ilist.end(), random_access_iterator_tag());
    }

    iterator begin() noexcept {
        return m_start;
    }

    const_iterator begin() const noexcept {
        return m_start;
    }

    const_iterator cbegin() const noexcept {
        return m_start;
    }
    iterator end() noexcept {
        return m_finish;
    }

    const_iterator end() const noexcept {
        return m_finish;
    }

    const_iterator cend() const noexcept {
        return m_finish;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(m_finish);
    }

    const_reverse_iterator rbegin() const noexcept {
        return reverse_iterator(m_finish);
    }

    const_reverse_iterator crbegin() const noexcept {
        return reverse_iterator(m_finish);
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(m_start);
    }

    const_reverse_iterator rend() const noexcept {
        return reverse_iterator(m_start);
    }

    const_reverse_iterator crend() const noexcept {
        return reverse_iterator(m_start);
    }

    reference operator[](size_type pos) {
        return m_start[pos];
    }

    const_reference operator[](size_type pos) const {
        return m_start[pos];
    }

    reference at(size_type pos) {
        m_range_check(pos);
        return (*this)[pos];
    }

    const_reference at(size_type pos) const {
        m_range_check(pos);
        return (*this)[pos];
    }

    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *begin();
    }

    reference back() {
        return *(end() - 1);
    }

    const_reference back() const {
        return *(end() - 1);
    }

    bool empty() const noexcept {
        return begin() == end();
    }

    size_type size() const noexcept {
        return tstl::distance(begin(), end());
    }

    size_type max_size() const {
        size_type diff_max = std::numeric_limits<difference_type>::max();
        size_type alloc_max = alloc_traits::max_size();
        return tstl::min(diff_max, alloc_max);
    }

    void shrink_to_fit() {
        // TODO
    }

    void clear() noexcept {
        m_erase_at_end(begin());
    }

    iterator insert(const_iterator pos, const T &value) {
        if (pos.m_cur == m_start.m_cur) {
            push_front(value);
            return m_start;
        } else if (pos.m_cur == m_finish.m_cur) {
            push_back(value);
            iterator tmp = m_finish;
            return --tmp;
        } else {
            m_insert_aux(pos, value);
        }
    }

    iterator insert(const_iterator pos, T &&value) {
        return emplace(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T &value) {
        difference_type offset = pos - cbegin();
        m_fill_insert(pos, count, value);
        return begin() + offset;
    }

    template <class InputIt, typename = tstl::_RequireInputIter<InputIt>>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type offset = pos - cbegin();
        m_range_insert_aux(pos, first, last, tstl::_iterator_category(first));
        return begin() + offset;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        size_type offset = pos - cbegin();
        m_range_insert_aux(pos, ilist.begin(), ilist.end(), tstl::random_access_iterator_tag());
        return begin() + offset;
    }

    template <class... Args>
    iterator emplace(const_iterator pos, Args &&...args) {
        if (pos.m_cur == m_start.m_cur) {
            emplace_front(std::forward<Args>(args)...);
            return m_start;
        } else if (pos.m_cur == m_finish.m_cur) {
            emplace_back(std::forward<Args>(args)...);
            iterator tmp = m_finish;
            return --tmp;
        } else {
            return m_insert_aux(pos, std::forward<Args>(args)...);
        }
    }

    iterator erase(const_iterator pos) {
        return m_erase(pos);
    }

    iterator erase(const_iterator first, const_iterator last) {
        return m_erase(first, last);
    }

    void push_back(const T &value) {
        if (m_finish.m_cur != m_finish.m_last - 1) {
            alloc_traits::construct(m_alloc, m_finish.m_cur, value);
            ++m_finish.m_cur;
        } else {
            m_push_back_aux(value);
        }
    }

    void push_back(T &&value) {
        emplace_back(std::move(value));
    }

    template <class... Args>
    void emplace_back(Args &&...args) {
        if (m_finish.m_cur != m_finish.m_last - 1) {
            alloc_traits::construct(m_alloc, m_finish.m_cur, std::forward<Args>(args)...);
            ++m_finish.m_cur;
        } else {
            m_push_back_aux(std::forward<Args>(args)...);
        }
    }

    void push_front(const T &value) {
        if (m_start.m_cur != m_start.m_first) {
            alloc_traits::construct(m_alloc, m_start.m_cur - 1, value);
            --m_start.m_cur;
        } else {
            m_push_front_aux(value);
        }
    }

    void push_front(T &&value) {
        emplace_front(std::move(value));
    }

    template <class... Args>
    void emplace_front(Args &&...args) {
        if (m_start.m_cur != m_start.m_first) {
            alloc_traits::construct(m_alloc, m_start.m_cur - 1, std::forward<Args>(args)...);
            --m_start.m_cur;
        } else {
            m_push_front_aux(std::forward<Args>(args)...);
        }
    }

    void pop_front() {
        if (m_start.m_cur != m_start.m_last - 1) {
            alloc_traits::destroy(m_alloc, m_start.m_cur);
            ++m_start.m_cur;
        } else {
            m_pop_front_aux();
        }
    }

    void pop_back() {
        if (m_finish.m_cur != m_finish.m_first) {
            --m_finish.m_cur;
            alloc_traits::destroy(m_alloc, m_finish.m_cur);
        } else {
            m_pop_back_aux();
        }
    }

    void resize(size_type count) {
        const size_type len = size();
        if (count > len) {
            m_default_append(count - len);
        } else {
            m_erase_at_end(m_start + count);
        }
    }

    void resize(size_type count, const value_type &value) {
        const size_type len = size();
        if (count > len) {
            m_fill_insert(m_finish, count - len, value);
        } else {
            m_erase_at_end(m_start + count);
        }
    }

    void swap(deque &other) {
        other.m_swap_data(*this);
    }

    friend bool operator==(const deque &lhs, const deque &rhs) {
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

  protected:
    using map_pointer = typename iterator::map_pointer;
    static const size_type m_buffer_size = _deque_buf_size<T>::value;

    using map_alloc_type = typename alloc_traits::template rebind_alloc<pointer>;
    using map_alloc_traits = std::allocator_traits<map_alloc_type>;

    Allocator m_alloc;
    map_alloc_type m_map_alloc;
    map_pointer m_map;
    size_type m_map_size;
    iterator m_start;
    iterator m_finish;

    void m_swap_data(deque &other) {
        tstl::swap(m_map, other.m_map);
        tstl::swap(m_map_size, other.m_map_size);
        tstl::swap(m_start, other.m_start);
        tstl::swap(m_finish, other.m_finish);
    }

    void m_default_init() {
        map_pointer cur;
        try {
            for (cur = m_start.m_node; cur < m_finish.m_node; ++cur) {
                tstl::_uninitialized_default_construct_a(*cur, *cur + m_buffer_size, m_alloc);
            }
            tstl::_uninitialized_default_construct_a(m_finish.m_first, m_finish.m_cur, m_alloc);
        } catch (...) {
            tstl::_destroy_a(m_start, iterator(*cur, cur), m_alloc);
            throw;
        }
    }

    void m_init_map(size_type elements_count) {
        size_type nodes_count = (elements_count / m_buffer_size) + 1;
        m_map_size = tstl::max<size_type>(TSTL_DEQUE_MAP_INIT_SIZE, nodes_count + 2);
        m_map = m_allocate_map(m_map_size);
        map_pointer n_start = m_map + (m_map_size - nodes_count) / 2;
        map_pointer n_finish = n_start + nodes_count;
        try {
            m_create_nodes(n_start, n_finish);
        } catch (...) {
            m_deallocate_map(m_map, m_map_size);
            m_map = nullptr;
            m_map_size = 0;
            throw;
        }
        m_start.m_set_node(n_start);
        m_finish.m_set_node(n_finish - 1);
        m_start.m_cur = m_start.m_first;
        m_finish.m_cur = m_finish.m_first + (elements_count % m_buffer_size);
    }

    void m_create_nodes(map_pointer n_start, map_pointer n_finish) {
        map_pointer cur;
        try {
            for (cur = n_start; cur < n_finish; ++cur) {
                *cur = m_allocate_node();
            }
        } catch (...) {
            m_destroy_nodes(n_start, cur);
            throw;
        }
    }

    void m_destroy_nodes(map_pointer n_start, map_pointer n_finish) {
        for (map_pointer n = n_start; n < n_finish; ++n) {
            m_deallocate_node(*n);
        }
    }

    map_pointer m_allocate_map(size_type count) {
        return map_alloc_traits::allocate(m_map_alloc, count);
    }

    pointer m_allocate_node() {
        return alloc_traits::allocate(m_alloc, m_buffer_size);
    }

    void m_deallocate_map(map_pointer p, size_type count) noexcept {
        map_alloc_traits::deallocate(m_map_alloc, p, count);
    }

    void m_deallocate_node(pointer p) {
        alloc_traits::deallocate(m_alloc, p, m_buffer_size);
    }

    void m_destroy(pointer p) {
        alloc_traits::destroy(m_alloc, p);
    }

    void m_destroy(pointer first, pointer last) {
        for (; first != last; ++first) {
            m_destroy(first);
        }
    }

    void m_destroy_data(iterator first, iterator last) {
        m_destroy_data_aux(first, last);
    }

    void m_destroy_data_aux(iterator first, iterator last) {
        for (map_pointer node = first.m_node + 1; node < last.m_node; ++node) {
            m_destroy(*node, *node + m_buffer_size);
        }
        if (first.m_node != last.m_node) {
            m_destroy(first.m_cur, first.m_last);
            m_destroy(last.m_first, last.m_cur);
        } else {
            m_destroy(first.m_cur, last.m_cur);
        }
    }

    void m_erase_at_begin(iterator pos) {
        m_destroy_data(begin(), pos);
        m_destroy_nodes(m_start.m_node, pos.m_node);
        m_start = pos;
    }

    void m_erase_at_end(iterator pos) {
        m_destroy_data(pos, end());
        m_destroy_nodes(pos.m_node + 1, m_finish.m_node + 1);
        m_finish = pos;
    }

    template <class InputIt>
    void m_range_init(InputIt first, InputIt last, tstl::input_iterator_tag) {
        m_init_map(0);
        try {
            for (; first != last; ++first) {
                emplace_back(*first);
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    template <class InputIt>
    void m_assign_aux(InputIt first, InputIt last, tstl::input_iterator_tag) {
        iterator current = begin();
        for (; first != last && current != end(); ++current, ++first) {
            *current = *first;
        }
        if (first == last) {
            m_erase_at_end(current);
        } else {
            m_range_insert_aux(end(), first, last, tstl::_iterator_category(first));
        }
    }

    template <class ForwardIt>
    void m_range_init(ForwardIt first, ForwardIt last, tstl::forward_iterator_tag) {
        const size_type n = tstl::distance(first, last);
        m_init_map(n);
        map_pointer current_node;
        try {
            for (current_node = m_start.m_node; current_node < m_finish.m_node; ++current_node) {
                ForwardIt mid = first;
                tstl::advance(mid, m_buffer_size);
                tstl::_uninitialized_copy_a(first, mid, *current_node, m_alloc);
                first = mid;
            }
            tstl::_uninitialized_copy_a(first, last, m_finish.m_first, m_alloc);
        } catch (...) {
            tstl::_destroy_a(m_start, iterator(*current_node, current_node), m_alloc);
            throw;
        }
    }

    iterator m_erase(iterator pos) {
        iterator next = pos;
        ++next;
        const difference_type id = pos - begin();
        if (size_type(id) < size() / 2) {
            if (pos != begin()) {
                tstl::move_backward(begin(), pos, next);
            }
            pop_front();
        } else {
            if (next != end()) {
                tstl::move(next, end(), pos);
            }
            pop_back();
        }
        return begin() + id;
    }

    iterator m_erase(iterator first, iterator last) {
        if (first == last) {
            return first;
        } else if (first == begin() && last == end()) {
            clear();
            return end();
        }
        const difference_type n = last - first;
        const difference_type elems_before = first - begin();
        if (size_type(elems_before) <= size() - n) {
            if (first != begin()) {
                tstl::move_backward(begin(), first, last);
            }
            m_erase_at_begin(begin() + n);
        } else {
            if (last != end()) {
                tstl::move(last, end(), first);
            }
            m_erase_at_end(end() - n);
        }
        return begin() + elems_before;
    }

    void m_default_append(size_type n) {
        if (n > 0) {
            iterator new_finish = m_reserve_elements_at_back(n);
            try {
                tstl::_uninitialized_default_construct_a(m_finish, new_finish, m_alloc);
                m_finish = new_finish;
            } catch (...) {
                m_destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                throw;
            }
        }
    }

    iterator m_reserve_elements_at_front(size_type n) {
        const size_type vacancies = m_start.m_cur - m_start.m_first;
        if (n > vacancies) {
            m_new_elements_at_front(n - vacancies);
        }
        return m_start - n;
    }

    iterator m_reserve_elements_at_back(size_type n) {
        const size_type vacancies = (m_finish.m_last - m_finish.m_cur) - 1;
        if (n > vacancies) {
            m_new_elements_at_back(n - vacancies);
        }
        return m_finish + n;
    }

    void m_new_elements_at_front(size_type new_elems) {
        const size_type new_nodes = (new_elems + m_buffer_size - 1) / m_buffer_size;
        m_reserve_map_at_front(new_nodes);
        size_type i;
        try {
            for (i = 1; i <= new_nodes; ++i) {
                *(m_start.m_node - i) = m_allocate_node();
            }
        } catch (...) {
            for (size_type j = 1; j < i; ++j) {
                m_deallocate_node(*(m_start.m_node + j));
            }
            throw;
        }
    }

    void m_new_elements_at_back(size_type new_elems) {
        const size_type new_nodes = (new_elems + m_buffer_size - 1) / m_buffer_size;
        m_reserve_map_at_back(new_nodes);
        size_type i;
        try {
            for (i = 1; i <= new_nodes; ++i) {
                *(m_finish.m_node + i) = m_allocate_node();
            }
        } catch (...) {
            for (size_type j = 1; j < i; ++j) {
                m_deallocate_node(*(m_finish.m_node + j));
            }
            throw;
        }
    }

    void m_reserve_map_at_back(size_type nodes_to_add = 1) {
        if (nodes_to_add + 1 > m_map_size - (m_finish.m_node - m_map)) {
            m_reallocate_map(nodes_to_add, false);
        }
    }

    void m_reserve_map_at_front(size_type nodes_to_add = 1) {
        if (nodes_to_add > size_type(m_start.m_node - m_map)) {
            m_reallocate_map(nodes_to_add, true);
        }
    }

    void m_reallocate_map(size_type nodes_to_add, bool add_at_front) {
        const size_type old_count_nodes = m_finish.m_node - m_start.m_node + 1;
        const size_type new_count_nodes = old_count_nodes + nodes_to_add;

        map_pointer new_nstart;
        if (m_map_size > 2 * new_count_nodes) {
            new_nstart = m_map + (m_map_size - new_count_nodes) / 2;
            if (add_at_front) {
                new_nstart += nodes_to_add;
            }
            if (new_nstart < m_start.m_node) {
                tstl::copy(m_start.m_node, m_finish.m_node + 1, new_nstart);
            } else {
                tstl::copy_backward(
                    m_start.m_node, m_finish.m_node + 1, new_nstart + old_count_nodes);
            }
        } else {
            size_type new_map_size = m_map_size + tstl::max(m_map_size, nodes_to_add) + 2;
            map_pointer new_map = m_allocate_map(new_map_size);
            new_nstart = new_map + (new_map_size - new_count_nodes) / 2;
            if (add_at_front) {
                new_nstart += nodes_to_add;
            }
            tstl::copy(m_start.m_node, m_finish.m_node + 1, new_nstart);
            m_deallocate_map(m_map, m_map_size);
            m_map = new_map;
            m_map_size = new_map_size;
        }
        m_start.m_set_node(new_nstart);
        m_finish.m_set_node(new_nstart + old_count_nodes - 1);
    }

    void m_pop_front_aux() {
        alloc_traits::destroy(m_alloc, m_start.m_cur);
        m_deallocate_node(m_start.m_first);
        m_start.m_set_node(m_start.m_node + 1);
        m_start.m_cur = m_start.m_first;
    }

    void m_pop_back_aux() {
        m_deallocate_node(m_finish.m_first);
        m_finish.m_set_node(m_finish.m_node - 1);
        m_finish.m_cur = m_finish.m_last - 1;
        alloc_traits::destroy(m_alloc, m_finish.m_first);
    }

    template <class ForwardIt>
    void
    m_range_insert_aux(iterator pos, ForwardIt first, ForwardIt last, tstl::forward_iterator_tag) {
        const size_type n = tstl::distance(first, last);
        if (pos.m_cur == m_start.m_cur) {
            iterator new_start = m_reserve_elements_at_front(n);
            try {
                tstl::_uninitialized_copy_a(first, last, new_start, m_alloc);
                m_start = new_start;
            } catch (...) {
                m_destroy_nodes(new_start.m_node, m_start.m_node);
                throw;
            }
        } else if (pos.m_cur == m_finish.m_cur) {
            iterator new_finish = m_reserve_elements_at_back(n);
            try {
                tstl::_uninitialized_copy_a(first, last, m_finish, m_alloc);
                m_finish = new_finish;
            } catch (...) {
                m_destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                throw;
            }
        } else {
            m_insert_aux(pos, first, last, n);
        }
    }

    template <class... Args>
    iterator m_insert_aux(iterator pos, Args &&...args) {
        T x_copy(std::forward<Args>(args)...);
        size_type id = pos - m_start;
        if (id < size() / 2) {
            push_front(std::move(front()));
            iterator front1 = m_start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            pos = m_start + id;
            iterator pos1 = pos;
            ++pos1;
            tstl::move(front2, pos1, front1);
        } else {
            push_back(std::move(back()));
            iterator back1 = m_finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = m_start + id;
            tstl::move_backward(pos, back2, back1);
        }
        *pos = std::move(x_copy);
        return pos;
    }

    void m_fill_assign(size_type count, const T &value) {
        if (count > size()) {
            tstl::fill(begin(), end(), value);
            m_fill_insert(end(), count - size(), value);
        } else {
            m_erase_at_end(begin() + count);
            tstl::fill(begin(), end(), value);
        }
    }

    void m_insert_aux(iterator pos, size_type n, const T &value) {
        const difference_type elems_before = pos - m_start;
        const size_type length = size();
        T x_copy = value;
        if (size_type(elems_before) < length / 2) {
            iterator new_start = m_reserve_elements_at_front(n);
            iterator old_start = m_start;
            pos = m_start + elems_before;
            try {
                if (size_type(elems_before) >= n) {
                    iterator start_n = m_start + n;
                    tstl::_uninitialized_move_a(m_start, start_n, new_start, m_alloc);
                    m_start = new_start;
                    tstl::move(start_n, pos, old_start);
                    tstl::fill(pos - difference_type(n), pos, x_copy);
                } else {
                    iterator mid = tstl::_uninitialized_move_a(m_start, pos, new_start, m_alloc);
                    tstl::_uninitialized_fill_a(mid, m_start, x_copy, m_alloc);
                    m_start = new_start;
                    tstl::fill(old_start, pos, x_copy);
                }
            } catch (...) {
                m_destroy_nodes(new_start.m_node, m_start.m_node);
                throw;
            }
        } else {
            iterator new_finish = m_reserve_elements_at_back(n);
            iterator old_finish = m_finish;
            const difference_type elems_after = difference_type(length) - elems_before;
            pos = m_finish - elems_after;
            try {
                if (size_type(elems_after) > n) {
                    iterator finish_n = m_finish - n;
                    tstl::_uninitialized_move_a(finish_n, m_finish, m_finish, m_alloc);
                    m_finish = new_finish;
                    tstl::move_backward(pos, finish_n, old_finish);
                    tstl::fill(pos, pos + n, x_copy);
                } else {
                    tstl::_uninitialized_fill_a(m_finish, pos + n, x_copy, m_alloc);
                    tstl::_uninitialized_move_a(pos, m_finish, pos + n, m_alloc);
                    m_finish = new_finish;
                    tstl::fill(pos, old_finish, x_copy);
                }
            } catch (...) {
                m_destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                throw;
            }
        }
    }

    template <class ForwardIt>
    void m_insert_aux(iterator pos, ForwardIt first, ForwardIt last, size_type n) {
        const difference_type elems_before = pos - m_start;
        const size_type length = size();
        if (size_type(elems_before) < length / 2) {
            iterator new_start = m_reserve_elements_at_front(n);
            iterator old_start = m_start;
            pos = m_start + elems_before;
            try {
                if (size_type(elems_before) >= n) {
                    iterator start_n = m_start + n;
                    tstl::_uninitialized_move_a(m_start, start_n, new_start, m_alloc);
                    m_start = new_start;
                    tstl::move(start_n, pos, old_start);
                    tstl::copy(first, last, pos - n);
                } else {
                    ForwardIt mid = first;
                    tstl::advance(mid, n - elems_before);
                    iterator mid2 = tstl::_uninitialized_move_a(m_start, pos, new_start, m_alloc);
                    tstl::_uninitialized_copy_a(first, mid, mid2, m_alloc);
                    m_start = new_start;
                    tstl::copy(mid, last, old_start);
                }
            } catch (...) {
                m_destroy_nodes(new_start.m_node, m_start.m_node);
                throw;
            }
        } else {
            iterator new_finish = m_reserve_elements_at_back(n);
            iterator old_finish = m_finish;
            const difference_type elems_after = length - elems_before;
            pos = m_finish - elems_after;
            try {
                if (size_type(elems_after) > n) {
                    iterator finish_n = m_finish - n;
                    _uninitialized_move_a(finish_n, m_finish, m_finish, m_alloc);
                    m_finish = new_finish;
                    tstl::move_backward(pos, finish_n, old_finish);
                    tstl::copy(first, last, pos);
                } else {
                    ForwardIt mid = first;
                    tstl::advance(mid, elems_after);
                    iterator mid2 = tstl::_uninitialized_move_a(mid, last, m_finish, m_alloc);
                    tstl::_uninitialized_copy_a(pos, m_finish, mid2, m_alloc);
                    m_finish = new_finish;
                    tstl::copy(first, mid, pos);
                }
            } catch (...) {
                m_destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                throw;
            }
        }
    }

    void m_fill_insert(iterator pos, size_type n, const T &value) {
        if (pos.m_cur == m_start.m_cur) {
            iterator new_start = m_reserve_elements_at_front(n);
            try {
                tstl::_uninitialized_fill_a(new_start, m_start, value, m_alloc);
                m_start = new_start;
            } catch (...) {
                m_destroy_nodes(new_start.m_node, m_start.m_node);
                throw;
            }
        } else if (pos.m_cur == m_finish.m_cur) {
            iterator new_finish = m_reserve_elements_at_back(n);
            try {
                tstl::_uninitialized_fill_a(m_finish, new_finish, value, m_alloc);
                m_finish = new_finish;
            } catch (...) {
                m_destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                throw;
            }
        } else {
            m_insert_aux(pos, n, value);
        }
    }

    void m_fill_init(const T &value) {
        map_pointer current;
        try {
            for (current = m_start.m_node; current < m_finish.m_node; ++current) {
                tstl::_uninitialized_fill_a(*current, *current + m_buffer_size, value, m_alloc);
            }
            tstl::_uninitialized_fill_a(m_finish.m_first, m_finish.m_cur, value, m_alloc);
        } catch (...) {
            tstl::_destroy_a(m_start, iterator(*current, current), m_alloc);
        }
    }

    template <class... Args>
    void m_push_back_aux(Args &&...args) {
        m_reserve_map_at_back();
        *(m_finish.m_node + 1) = m_allocate_node();
        try {
            alloc_traits::construct(m_alloc, m_finish.m_cur, std::forward<Args>(args)...);
            m_finish.m_set_node(m_finish.m_node + 1);
            m_finish.m_cur = m_finish.m_first;
        } catch (...) {
            m_deallocate_node(*(m_finish.m_node + 1));
            throw;
        }
    }

    template <class... Args>
    void m_push_front_aux(Args &&...args) {
        m_reserve_map_at_front();
        *(m_start.m_node - 1) = m_allocate_node();
        try {
            m_start.m_set_node(m_start.m_node - 1);
            m_start.m_cur = m_start.m_last - 1;
            alloc_traits::construct(m_alloc, m_start.m_cur, std::forward<Args>(args)...);
        } catch (...) {
            m_deallocate_node(*(m_start.m_node - 1));
            throw;
        }
    }

    void m_range_check(size_type pos) {
        if (pos >= size()) {
            throw std::out_of_range("deque::m_range_check: out of range");
        }
    }
};

/**
 * @brief 为 deque 特化 swap 算法。
 */
template <class T, class Alloc>
void swap(deque<T, Alloc> &lhs, deque<T, Alloc> &rhs) {
    lhs.swap(rhs);
}

} // namespace tstl

#endif
