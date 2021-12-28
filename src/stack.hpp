#ifndef TSTL_SRC_STACK_HPP
#define TSTL_SRC_STACK_HPP

#include "iterator.hpp"
#include "algorithm.hpp"
#include <limits>
#include <deque>

namespace tstl {

template <class T, class Container = std::deque<T>>
class stack {

  public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    template <class Alloc>
    struct uses_allocator : std::uses_allocator<Container, Alloc>::type {};

  protected:
    Container c;

    template <typename Alloc>
    using _Uses = typename std::enable_if<uses_allocator<Alloc>::value>::type;

  public:
    stack() : stack(Container()) {
    }

    explicit stack(const Container &cont) : c(cont) {
    }

    explicit stack(Container &&cont) : c(cont) {
    }

    stack(const stack &other) = default;

    stack(stack &&other) noexcept = default;

    template <class Alloc, _Uses<Alloc>>
    explicit stack(const Alloc &alloc) : c(alloc) {
    }

    template <class Alloc, _Uses<Alloc>>
    stack(const Container &cont, const Alloc &alloc) : c(cont, alloc) {
    }

    template <class Alloc, _Uses<Alloc>>
    stack(Container &&cont, const Alloc &alloc) : c(cont, alloc) {
    }

    template <class Alloc, _Uses<Alloc>>
    stack(const stack &other, const Alloc &alloc) : c(other.c, alloc) {
    }

    template <class Alloc, _Uses<Alloc>>
    stack(stack &&other, const Alloc &alloc) : c(other.c, alloc) {
    }

    ~stack() = default;

    stack &operator=(const stack &other) = default;

    stack &operator=(stack &&other) = default;

    reference top() {
        return c.back();
    }

    const_reference top() const {
        return c.back();
    }

    bool empty() const {
        return c.empty();
    }

    size_type size() const {
        return c.size();
    }

    void push(const value_type &value) {
        c.push_back(value);
    }

    void push(value_type &&value) {
        c.push_back(std::move(value));
    }

    template <class... Args>
    void emplace(Args &&...args) {
        c.emplace_back(std::forward<Args>(args)...);
    }

    void pop() {
        c.pop_back();
    }

    void swap(stack &other) noexcept {
        tstl::swap(c, other.c);
    }

    friend bool operator==(const stack &lhs, const stack &rhs) {
        return lhs.c == rhs.c;
    }

    friend bool operator!=(const stack &lhs, const stack &rhs) {
        return lhs.c != rhs.c;
    }

    friend bool operator<(const stack &lhs, const stack &rhs) {
        return lhs.c < rhs.c;
    }

    friend bool operator>(const stack &lhs, const stack &rhs) {
        return lhs.c > rhs.c;
    }

    friend bool operator<=(const stack &lhs, const stack &rhs) {
        return lhs.c < rhs.c;
    }

    friend bool operator>=(const stack &lhs, const stack &rhs) {
        return lhs.c > rhs.c;
    }

    friend void swap(stack &lhs, stack &rhs) {
        lhs.swap(rhs);
    }
};

/**
 * @brief 为 stack 特化 swap 算法。
 */
template <class T, class Alloc>
void swap(stack<T, Alloc> &lhs, stack<T, Alloc> &rhs) {
    lhs.swap(rhs);
}

} // namespace tstl

#endif
