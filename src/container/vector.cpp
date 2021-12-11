#ifndef TSTL_SRC_VECTOR_CPP
#define TSTL_SRC_VECTOR_CPP

#include <iterator>
#include <memory>
#include <limits>

namespace tstl {

template <class T, class Allocator = std::allocator<T>>
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
    using const_iterator = typename __gnu_cxx::__normal_iterator<const_pointer, vector>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
     * @brief 构造拥有个 count 默认插入的 T 实例的容器。不进行复制。
     */
    explicit vector(size_type count, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
        m_start = m_alloc.allocate(count);
        m_finish = std::__uninitialized_default_n_a(m_start, count, alloc);
        m_end_of_storage = m_start + count;
    }

    /**
     * @brief 构造拥有 count 个有值 value 的元素的容器。
     */
    vector(size_type count, const T &value, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
        m_start = m_alloc.allocate(count);
        m_finish = std::__uninitialized_fill_n_a(m_start, count, value, alloc);
        m_end_of_storage = m_start + count;
    }

    template <class InputIt, typename = std::_RequireInputIter<InputIt>>
    vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());

    vector(const vector &other, const Allocator &alloc);

    vector(vector &&other, const Allocator &alloc);

    vector(const vector &other);

    vector(vector &&other);

    vector(std::initializer_list<T> init, const Allocator &alloc = Allocator());

    ~vector() {
        m_alloc.destroy(m_start);
        m_alloc.deallocate(m_start, m_end_of_storage - m_start);
    }

    vector &operator=(const vector &other);
    vector &operator=(vector &&other);
    vector &operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T &value);

    void assign(std::initializer_list<T> ilist);

    /**
     * @brief 返回与容器关联的分配器。
     */
    allocator_type get_allocator() const {
        return Allocator();
    }

    reference at(size_type pos);

    const_reference at(size_type pos) const;

    /**
     * @brief 返回位于指定位置 pos 的元素的引用。不进行边界检查。
     */
    reference operator[](size_type pos) {
        return m_start[pos];
    }

    const_reference operator[](size_type pos) const {
        return m_start[pos];
    }

    reference back();

    const_reference back() const;

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
        return std::distance(begin(), end());
    }

    /**
     * @brief 返回根据系统或库实现限制的容器可保有的元素最大数量。
     */
    size_type max_size() const {
        size_type diff_max = std::numeric_limits<difference_type>::max();
        size_type alloc_max = std::allocator_traits<Allocator>::max_size();
        return std::min(diff_max, alloc_max);
    }

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
    pointer m_start;
    pointer m_finish;
    pointer m_end_of_storage;
    Allocator m_alloc;
};

} // namespace tstl

#endif
