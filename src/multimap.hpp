#ifndef TSTL_SRC_MULTIMAP_HPP
#define TSTL_SRC_MULTIMAP_HPP

// #include "rb_tree/rbTree.hpp"
#include "rbtree.hpp"
#include <ostream>

namespace tstl {

template <class Key, class T, class Compare = std::less<Key>>
class multimap {
  public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<const Key, T> value_type;
    typedef Compare key_compare;

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
    // 底层为红黑树
    typedef tstl::rb_tree<value_type, key_compare> base_type;
    base_type tree_;

  public:
    // 红黑树的一些基本定义
    typedef typename base_type::node_type node_type;
    typedef typename base_type::pointer pointer;
    typedef typename base_type::const_pointer const_pointer;
    typedef typename base_type::reference reference;
    typedef typename base_type::const_reference const_reference;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::const_iterator const_iterator;
    typedef typename base_type::reverse_iterator reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::size_type size_type;
    typedef typename base_type::difference_type difference_type;
    typedef typename base_type::allocator_type allocator_type;

  public:
    // 构造、复制、移动函数
    multimap() = default;

    template <class InputIterator>
    multimap(InputIterator first, InputIterator last) : tree_() {
        tree_.insert_multi(first, last);
    }
    multimap(std::initializer_list<value_type> ilist) : tree_() {
        tree_.insert_multi(ilist.begin(), ilist.end());
    }

    multimap(const multimap &rhs) : tree_(rhs.tree_) {
    }
    multimap(multimap &&rhs) noexcept : tree_(std::move(rhs.tree_)) {
    }

    multimap &operator=(const multimap &rhs) {
        tree_ = rhs.tree_;
        return *this;
    }
    multimap &operator=(multimap &&rhs) {
        tree_ = std::move(rhs.tree_);
        return *this;
    }

    multimap &operator=(std::initializer_list<value_type> ilist) {
        tree_.clear();
        tree_.insert_multi(ilist.begin(), ilist.end());
        return *this;
    }

    // 接口
    key_compare key_comp() const {
        return tree_.key_comp();
    }
    value_compare value_comp() const {
        return value_compare(tree_.key_comp());
    }
    allocator_type get_allocator() const {
        return tree_.get_allocator();
    }

    // 迭代器相关方法
    iterator begin() noexcept {
        return tree_.begin();
    }
    const_iterator begin() const noexcept {
        return tree_.begin();
    }
    iterator end() noexcept {
        return tree_.end();
    }
    const_iterator end() const noexcept {
        return tree_.end();
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

    // size相关
    bool empty() const noexcept {
        return tree_.empty();
    }
    size_type size() const noexcept {
        return tree_.size();
    }
    size_type max_size() const noexcept {
        return tree_.max_size();
    }

    // 插入删除
    template <class... Args>
    iterator emplace(Args &&...args) {
        return tree_.emplace_multi(std::forward<Args>(args)...);
    }
    template <class... Args>
    iterator emplace_hint(iterator hint, Args &&...args) {
        return tree_.emplace_multi_use_hint(hint, std::forward<Args>(args)...);
    }
    iterator insert(const value_type &value) {
        return tree_.insert_multi(value);
    }
    iterator insert(value_type &&value) {
        return tree_.insert_multi(std::move(value));
    }

    iterator insert(iterator hint, const value_type &value) {
        return tree_.insert_multi(hint, value);
    }
    iterator insert(iterator hint, value_type &&value) {
        return tree_.insert_multi(hint, std::move(value));
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        tree_.insert_multi(first, last);
    }
    void erase(iterator position) {
        tree_.erase(position);
    }
    size_type erase(const key_type &key) {
        return tree_.erase_multi(key);
    }
    void erase(iterator first, iterator last) {
        tree_.erase(first, last);
    }
    void clear() {
        tree_.clear();
    }

    // 查找操作
    iterator find(const key_type &key) {
        return tree_.find(key);
    }
    const_iterator find(const key_type &key) const {
        return tree_.find(key);
    }

    size_type count(const key_type &key) const {
        return tree_.count_multi(key);
    }

    iterator lower_bound(const key_type &key) {
        return tree_.lower_bound(key);
    }
    const_iterator lower_bound(const key_type &key) const {
        return tree_.lower_bound(key);
    }

    iterator upper_bound(const key_type &key) {
        return tree_.upper_bound(key);
    }
    const_iterator upper_bound(const key_type &key) const {
        return tree_.upper_bound(key);
    }

    pair<iterator, iterator> equal_range(const key_type &key) {
        return tree_.equal_range_multi(key);
    }

    pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
        return tree_.equal_range_multi(key);
    }

    void swap(multimap &rhs) noexcept {
        tree_.swap(rhs.tree_);
    }

  public:
    friend bool operator==(const multimap &lhs, const multimap &rhs) {
        return lhs.tree_ == rhs.tree_;
    }
    friend bool operator<(const multimap &lhs, const multimap &rhs) {
        return lhs.tree_ < rhs.tree_;
    }
    friend std::ostream &operator<<(std::ostream &output, const multimap &mp) {
        output << '[';
        for (auto i : mp) {
            if (i != *mp.begin())
                output << ',';
            output << '{' << i.first << " , " << i.second << '}';
        }
        output << ']';
        return output;
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