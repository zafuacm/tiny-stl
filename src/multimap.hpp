#ifndef TSTL_SRC_MULTIMAP_HPP
#define TSTL_SRC_MULTIMAP_HPP

// #include "rb_tree/rbTree.hpp"
#include "rbtree.hpp"

namespace tstl {

template <class Key, class T, class Compare = std::less<Key>>
class multimap {
  public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using key_compare = Compare;

    // 定义一个仿函数
    class value_compare : public std::binary_function<value_type, value_type, bool> {
        friend class multimap<Key, T, Compare>;

      private:
        Compare comp;
        value_compare(Compare c) : comp(c) {
        }

      public:
        bool operator()(const value_type &lhs, const value_type &rhs) const {
            return comp(lhs.first, rhs.first);
        }
    };

  private:
    using base_type = tstl::rb_tree<value_type, key_compare>;
    base_type m_tree;

  public:
    // 红黑树的一些基本定义
    using node_type = typename base_type::node_type;
    using pointer = typename base_type::pointer;
    using const_pointer = typename base_type::const_pointer;
    using reference = typename base_type::reference;
    using const_reference = typename base_type::const_reference;
    using iterator = typename base_type::iterator;
    using const_iterator = typename base_type::const_iterator;
    using reverse_iterator = typename base_type::reverse_iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;
    using size_type = typename base_type::size_type;
    using difference_type = typename base_type::difference_type;
    using allocator_type = typename base_type::allocator_type;

  public:
    // 构造、复制、移动函数
    multimap() = default;

    template <class InputIt>
    multimap(InputIt first, InputIt last) : m_tree() {
        m_tree.insert_multi(first, last);
    }

    multimap(std::initializer_list<T> ilist) : m_tree() {
        m_tree.insert_multi(ilist.begin(), ilist.end());
    }

    multimap(const multimap &other) : m_tree(other.m_tree) {
    }

    multimap(multimap &&other) noexcept : m_tree(std::move(other.m_tree)) {
    }

    multimap &operator=(const multimap &rhs) {
        m_tree = rhs.m_tree;
        return *this;
    }

    multimap &operator=(multimap &&rhs) noexcept {
        m_tree = std::move(rhs.m_tree);
        return *this;
    }

    multimap &operator=(std::initializer_list<T> ilist) {
        m_tree.clear();
        m_tree.insert_multi(ilist.begin(), ilist.end());
        return *this;
    }

    // 接口
    key_compare key_comp() const {
        return m_tree.key_comp();
    }

    value_compare value_comp() const {
        return value_compare(m_tree.key_comp());
    }

    allocator_type get_allocator() const {
        return m_tree.get_allocator();
    }

    iterator begin() noexcept {
        return m_tree.begin();
    }

    const_iterator begin() const noexcept {
        return m_tree.begin();
    }

    iterator end() noexcept {
        return m_tree.end();
    }

    const_iterator end() const noexcept {
        return m_tree.end();
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    bool empty() const noexcept {
        return m_tree.empty();
    }

    size_type size() const noexcept {
        return m_tree.size();
    }

    size_type max_size() const noexcept {
        return m_tree.max_size();
    }

    template <class... Args>
    iterator emplace(Args &&...args) {
        return m_tree.emplace_multi(std::forward<Args>(args)...);
    }

    template <class... Args>
    iterator emplace_hint(iterator hint, Args &&...args) {
        return m_tree.emplace_multi_use_hint(hint, std::forward<Args>(args)...);
    }

    iterator insert(const value_type &value) {
        return m_tree.insert_multi(value);
    }

    iterator insert(value_type &&value) {
        return m_tree.insert_multi(std::move(value));
    }

    iterator insert(iterator hint, const value_type &value) {
        return m_tree.insert_multi(hint, value);
    }

    iterator insert(iterator hint, value_type &&value) {
        return m_tree.insert_multi(hint, std::move(value));
    }

    template <class InputIt>
    void insert(InputIt first, InputIt last) {
        m_tree.insert_multi(first, last);
    }

    void erase(iterator position) {
        m_tree.erase(position);
    }

    size_type erase(const key_type &key) {
        return m_tree.erase_multi(key);
    }

    void erase(iterator first, iterator last) {
        m_tree.erase(first, last);
    }

    void clear() {
        m_tree.clear();
    }

    iterator find(const key_type &key) {
        return m_tree.find(key);
    }

    const_iterator find(const key_type &key) const {
        return m_tree.find(key);
    }

    size_type count(const key_type &key) const {
        return m_tree.count_multi(key);
    }

    iterator lower_bound(const key_type &key) {
        return m_tree.lower_bound(key);
    }

    const_iterator lower_bound(const key_type &key) const {
        return m_tree.lower_bound(key);
    }

    iterator upper_bound(const key_type &key) {
        return m_tree.upper_bound(key);
    }

    const_iterator upper_bound(const key_type &key) const {
        return m_tree.upper_bound(key);
    }

    std::pair<iterator, iterator> equal_range(const key_type &key) {
        return m_tree.equal_range_multi(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
        return m_tree.equal_range_multi(key);
    }

    void swap(multimap &other) noexcept {
        m_tree.swap(other.m_tree);
    }

    friend bool operator==(const multimap &lhs, const multimap &rhs) {
        return lhs.m_tree == rhs.m_tree;
    }

    friend bool operator<(const multimap &lhs, const multimap &rhs) {
        return lhs.m_tree < rhs.m_tree;
    }
};

// 重载比较操作符
template <class Key, class T, class Compare>
bool operator==(const multimap<Key, T, Compare> &lhs, const multimap<Key, T, Compare> &rhs) {
    return lhs == rhs;
}

template <class Key, class T, class Compare>
bool operator<(const multimap<Key, T, Compare> &lhs, const multimap<Key, T, Compare> &rhs) {
    return lhs < rhs;
}

template <class Key, class T, class Compare>
bool operator!=(const multimap<Key, T, Compare> &lhs, const multimap<Key, T, Compare> &rhs) {
    return !(lhs == rhs);
}

template <class Key, class T, class Compare>
bool operator>(const multimap<Key, T, Compare> &lhs, const multimap<Key, T, Compare> &rhs) {
    return rhs < lhs;
}

template <class Key, class T, class Compare>
bool operator<=(const multimap<Key, T, Compare> &lhs, const multimap<Key, T, Compare> &rhs) {
    return !(rhs < lhs);
}

template <class Key, class T, class Compare>
bool operator>=(const multimap<Key, T, Compare> &lhs, const multimap<Key, T, Compare> &rhs) {
    return !(lhs < rhs);
}

} // namespace tstl

#endif