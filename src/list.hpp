#ifndef TSTL_SRC_LIST_HPP
#define TSTL_SRC_LIST_HPP

#include "memory/construct.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include <iostream>
#include <ostream>
#include <utility>

// 文件内容主要是容器 —— List 以及 其配件
namespace tstl {

template <class T>
struct list_node_base;
template <class T>
struct list_node;

// 结点萃取
template <class T>
struct node_traits {
    typedef list_node_base<T> *base_ptr; // 指向基本结点的指针
    typedef list_node<T> *node_ptr;      // 指向含数据域结点的指针
};

// list 的基本结点
template <class T>
struct list_node_base {

    using base_ptr = typename node_traits<T>::base_ptr;
    using node_ptr = typename node_traits<T>::node_ptr;

    // 只有指针
    base_ptr prev;
    base_ptr next;

    // 构造函数
    list_node_base() = default;
    // 指向自己的指针
    base_ptr self() {
        return static_cast<base_ptr>(&*this);
    }
    // 将自己强转为 list_node*
    node_ptr as_node() {
        return static_cast<node_ptr>(self());
    }
    // 没有指向
    void unlink() {
        prev = next = self();
    }
};

// list 的结点
template <class T>
struct list_node : public list_node_base<T> {
    using base_ptr = typename node_traits<T>::base_ptr;
    using node_ptr = typename node_traits<T>::node_ptr;

    T data; // 数据域

    // ListNode 的构造函数
    list_node() = default;

    list_node(const T &_data) : data(_data) {
    }

    list_node(T &&_data) : data(std::move(_data)) {
    } // 右值引用

    // 指针
    node_ptr self() {
        return static_cast<node_ptr>(&*this);
    }

    base_ptr as_base() {
        return static_cast<base_ptr>(self());
    }
};

// list 的迭代器
template <class T>
struct list_iterator : public tstl::iterator<tstl::bidirectional_iterator_tag, T> {
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_iterator<T> self;

    base_ptr m_node; // 指向当前结点的指针

    // 构造函数
    list_iterator() = default;

    list_iterator(base_ptr p) : m_node(p) {
    }

    list_iterator(node_ptr p) : m_node(p->as_base()) {
    }

    list_iterator(const list_iterator &iter) : m_node(iter.m_node) {
    }

    // 重载操作符
    reference operator*() const {
        return m_node->as_node()->data;
    }

    pointer operator->() const {
        return &(operator*());
    }

    bool operator==(const self &iter) const {
        return m_node == iter.m_node;
    }

    bool operator!=(const self &iter) const {
        return m_node != iter.m_node;
    }

    self &operator++() {
        m_node = m_node->next;
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self &operator--() {
        m_node = m_node->prev;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

// list 的常迭代器
template <class T>
struct list_const_iterator : public tstl::iterator<tstl::bidirectional_iterator_tag, T> {
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_const_iterator<T> self;

    base_ptr m_node; // 指向当前结点的指针

    // 构造函数
    list_const_iterator() = default;

    list_const_iterator(base_ptr p) : m_node(p) {
    }

    list_const_iterator(node_ptr p) : m_node(p->as_base()) {
    }

    list_const_iterator(const list_const_iterator &iter) : m_node(iter.m_node) {
    }

    list_const_iterator(const list_iterator<T> &other) : m_node(other.m_node) {
    }

    // 重载操作符
    reference operator*() const {
        return m_node->as_node()->data;
    }

    pointer operator->() const {
        return &(operator*());
    }

    bool operator==(const self &iter) const {
        return m_node == iter.m_node;
    }

    bool operator!=(const self &iter) const {
        return m_node != iter.m_node;
    }

    self &operator++() {
        m_node = m_node->next;
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self &operator--() {
        m_node = m_node->prev;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

// list 容器
template <class T, class Allocator = std::allocator<T>>
class list {
  public:
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using iterator = list_iterator<T>;
    using const_iterator = list_const_iterator<T>;
    using reverse_iterator = tstl::reverse_iterator<iterator>;
    using const_reverse_iterator = tstl::reverse_iterator<const_iterator>;

    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_node<T> Lnode;
    typedef list_node_base<T> Lbnode;
    typedef std::allocator<Lnode> list_node_alloc;
    typedef std::allocator<Lbnode> list_bnode_alloc;

  private:
    base_ptr node;              // 指向末尾的结点 = end()
    size_type m_size;           // 容器大小
    list_bnode_alloc basealloc; // 构造器
    list_node_alloc alloc;      // 构造器

  public:
    // 构造函数
    list() {
        empty_initialized();
    }

    explicit list(size_type n, const value_type &value = value_type()) {
        empty_initialized();
        fill_insert(begin(), n, value);
    }

    template <class InputIt, typename = tstl::_RequireInputIter<InputIt>>
    list(InputIt first, InputIt last) {
        empty_initialized();
        list_insert(begin(), first, last);
    }

    list(const list &l) {
        empty_initialized();
        list_insert(begin(), l.begin(), l.end());
    }

    list &operator=(const list &l) noexcept {
        list temp(l);
        swap(temp);
        return *this;
    }

    list(list &&rhs) noexcept { // 右值引用
        empty_initialized();
        swap(rhs);
        tstl::distance(begin(), end());
    }

    list &operator=(list &&rhs) noexcept {
        clear();
        swap(rhs);
        m_size = tstl::distance(begin(), end());
        return *this;
    }

    ~list() {
        clear();
        put_node(node);
    }

  public:
    // 常用方法
    void push_back(const T &value) {
        insert(end(), value);
    }

    void push_front(const T &value) {
        insert(begin(), value);
    }

    void pop_fornt() {
        erase(begin());
    }

    void pop_back() {
        iterator temp = end();
        erase(--temp);
    }

    template <class... Args> // 右值引用，暂时没写好 forward，先用一下std的
    void emplace_back(Args &&...args) {
        insert(end(), std::forward<Args>(args)...);
    }

    template <class... Args>
    void emplace_front(Args &&...args) {
        insert(begin(), std::forward<Args>(args)...);
    }

    size_type size() const noexcept {
        return m_size;
    }

    bool empty() const noexcept {
        return begin() == end();
    }

    void swap(list &rhs) noexcept {
        tstl::swap(node, rhs.node);
        tstl::swap(m_size, rhs.m_size);
    }

    void assign(size_type n, const value_type &val = value_type()) {
        fill_assign(n, val);
        m_size = n;
    }

    void assign(iterator first, iterator last) {
        assign_dispatch(first, last);
    }

    void resize(size_type n) {
        if (m_size >= n) {
            iterator cur = begin();
            for (; cur != end() && n; ++cur, --n) {
            }
            erase(cur, end());
        } else {
            fill_insert(end(), n - m_size, value_type());
        }
    }

    // 在 position 迭代器之前插入一个结点
    iterator insert(iterator position) {
        return insert(position, value_type());
    }

    iterator insert(iterator position, const T &x) {
        ++m_size;
        node_ptr tmp = create_node(x);
        tmp->next = position.m_node;
        tmp->prev = position.m_node->prev;
        position.m_node->prev->next = tmp;
        position.m_node->prev = tmp;
        return tmp;
    }

    // 删除迭代器指向的结点
    iterator erase(iterator position) {
        --m_size;
        base_ptr next_node = position.m_node->next;
        base_ptr prev_node = position.m_node->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.m_node);
        return iterator(next_node);
    }

    // 删除 [first, last)
    iterator erase(iterator first, iterator last) {
        iterator res;
        while (first != last) {
            res = erase(first++);
        }
        return res;
    }

    // 清空链表
    void clear() {
        base_ptr now = node->next;
        while (now != node) {
            base_ptr temp = now;
            now = now->next;
            destroy_node(temp);
        }
        node->unlink();
        m_size = 0;
    }

    // 链表去重
    void unique() {
        iterator first = begin();
        iterator last = end();
        if (first == last) {
            return;
        }
        iterator next = first;
        while (++next != last) {
            if (*first == *next) {
                erase(next);
                next = first;
            } else {
                first = next;
            }
        }
    }

    // 再 pos 之前插入一段链表
    void splice(iterator pos, list &rhs) {
        if (!rhs.empty()) {
            transfer(pos, rhs.begin(), rhs.end());
            m_size += rhs.m_size;
            rhs.m_size = 0;
        }
    }

    void splice(iterator position, list &l, iterator last) {
        iterator tmp = last;
        ++tmp;
        // i==pos 自身无法插于自身之前
        // j==pos 已处于pos之前
        if (position == last || position == tmp) {
            return;
        }
        m_size += distance(l.begin(), last); // 更新大小（复杂度高，待优化）
        transfer(position, last, tmp);
    }

    void splice(iterator pos, list &, iterator first, iterator last) {
        if (first != last) {
            m_size += distance(first, last);
            transfer(pos, first, last);
        }
    }

    // 链表合并
    void merge(list &l) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = l.begin();
        iterator last2 = l.end();

        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if (first2 != last2) {
            transfer(last1, first2, last2);
        }
        m_size += l.m_size;
        l.m_size = 0;
    }

    // 链表合并（支持仿函数）
    template <class Compare>
    void merge(list &l, Compare cmp) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = l.begin();
        iterator last2 = l.end();

        while (first1 != last1 && first2 != last2) {
            if (cmp(*first2, *first1)) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if (first2 != last2) {
            transfer(last1, first2, last2);
        }
        m_size += l.m_size;
        l.m_size = 0;
    }

    // 链表翻转
    void reverse() {
        // empty || size()==1
        if (node->next == node || node->next->next == node)
            return;
        iterator first = begin();
        ++first; // begin自身并不需要移动，它将作为指示末元素的哨兵（确切地说，最终begin.node->next
                 // == end.node)
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }

    // 归并排序
    void sort() {
        if (node->next == node || node->next->next == node)
            return;
        // 数据缓存区
        // _tmp[n]中最多存放2^(n+1)个元素，若大于则与_tmp[n+1]作归并
        list carry;
        list counter[64];
        int fill = 0, size = m_size;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) {
                ++fill;
            }
        }
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1]);
        }
        swap(counter[fill - 1]);
        m_size = size;
    }

    // 支持仿函数的排序
    template <class Compare>
    void sort(Compare cmp) {
        if (node->next == node || node->next->next == node)
            return;
        // 数据缓存区
        // _tmp[n]中最多存放2^(n+1)个元素，若大于则与_tmp[n+1]作归并
        list carry;
        list counter[64];
        int fill = 0, size = m_size;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry, cmp);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) {
                ++fill;
            }
        }
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1], cmp);
        }
        swap(counter[fill - 1]);
        m_size = size;
    }

    // 从 list 种移除 value 元素
    void remove(const T &value) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator next = first;
            ++next;
            if (*first == value) {
                erase(first);
            }
            first = next;
        }
    }

  public:
    // 迭代器相关
    // setter
    iterator begin() noexcept {
        return iterator(node->next);
    }

    iterator end() noexcept {
        return iterator(node);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    reference front() noexcept {
        return *begin();
    }

    reference back() noexcept {
        return *(--end());
    }

    // getter
    const_iterator begin() const noexcept {
        return const_iterator(node->next);
    }

    const_iterator end() const noexcept {
        return const_iterator(node);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(node->next);
    }

    const_iterator cend() const noexcept {
        return const_iterator(node);
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    const_reference front() const noexcept {
        return *begin();
    }

    const_reference back() const noexcept {
        return *(--end());
    }

  private:
    // 生成一个结点
    base_ptr create_base_node() {
        return alloc.allocate(1);
    }
    node_ptr create_node(const value_type &value) {
        node_ptr p = alloc.allocate(1);
        try {
            alloc.construct(p, value);
        } catch (...) {
            put_node(p);
            throw;
        }
        return p;
    }

    // 销毁一个结点
    void put_node(base_ptr p) {
        basealloc.deallocate(p, 1);
    }

    void put_node(node_ptr p) {
        alloc.deallocate(p, 1);
    }

    void destroy_node(base_ptr p) {
        put_node(p);
    }

    void destroy_node(node_ptr p) {
        alloc.destory(&p->data);
        put_node(p);
    }

    // 链表为空的初始化
    void empty_initialized() {
        node = create_base_node(); // 链表的末尾结点
        node->unlink();            // 指针自连（初始化）
        m_size = 0;
    }

    // 在 position 之前插入 [first， last)
    template <class InputIt>
    void list_insert(iterator position, InputIt first, InputIt last) {
        size_type size = 0;
        for (; first != last; ++first, ++size) {
            insert(position, *first);
        }
    }

    // 在 pos 的位置之前，填充 n 个值为 val 的结点
    void fill_insert(iterator position, size_type n, const value_type &val) {
        for (size_type i = n; i != 0; --i) {
            position = insert(position, val);
        }
    }

    // 对链表只填充 n 个位置
    void fill_assign(size_type n, const value_type &val = value_type()) {
        iterator i = begin();
        for (; i != end(); ++i, --n) {
            *i = val;
        }
        if (n > 0) {
            fill_insert(end(), n, val);
        } else {
            erase(i, end());
        }
    }

    // 将链表变为 [first, last)
    size_type assign_dispatch(iterator first, iterator last) {
        size_type size = 0;
        iterator start = begin();
        iterator finish = end();
        for (; start != finish && first != last; ++start, ++first, ++size) {
            *start = *first;
        }
        if (first == last) {
            erase(start, finish);
        } else {
            list_insert(finish, first, last);
        }
        return size;
    }

    // 在position 前面插入 [first, last)
    void transfer(iterator position, iterator first, iterator last) {
        if (position != last) {
            last.m_node->prev->next = position.m_node;
            first.m_node->prev->next = last.m_node;
            position.m_node->prev->next = first.m_node;
            base_ptr temp = position.m_node->prev;
            position.m_node->prev = last.m_node->prev;
            last.m_node->prev = first.m_node->prev;
            first.m_node->prev = temp;
        }
    }

    friend std::ostream &operator<<(std::ostream &output, const list &l) {
        output << "[list] (" << l.size() << ") : [";
        for (auto i : l) {
            output << i << ", ";
        }
        output << "]";
        return output;
    }
};

template <class T>
inline bool operator==(const tstl::list<int>::iterator &lhs, const tstl::list<int>::iterator &rhs) {
    return lhs == rhs;
}

template <class T>
inline bool operator!=(const tstl::list<int>::iterator &lhs, const tstl::list<int>::iterator &rhs) {
    return lhs != rhs;
}

template <class T>
bool operator==(const list<T> &lhs, const list<T> &rhs) {
    auto it1 = lhs.begin(), it2 = rhs.begin(); // same as cbegin()
    for (; it1 != lhs.end() && it2 != rhs.end(); ++it1, ++it2)
        if (*it1 != *it2)
            return false;
    return it1 == lhs.end() && it2 == rhs.end();
}

template <class T>
inline bool operator!=(const list<T> &lhs, const list<T> &rhs) {
    return !(lhs == rhs);
}

} // namespace tstl

#endif