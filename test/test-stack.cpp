#ifndef TEST_TEST_STACK
#define TEST_TEST_STACK

#include "../src/vector.hpp"
#include "../src/deque.hpp"
#include "../src/stack.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class mystack : public tstl::stack<T, tstl::deque<T>> {
  public:
    using tstl::stack<T, tstl::deque<T>>::stack;
    friend std::ostream &operator<<(std::ostream &os, const mystack &s) {
        os << "[stack] (" << s.size() << ") : [";
        for (auto i : s.c) {
            os << i << ", ";
        }
        return os << "]";
    }
};

namespace test_stack_detail {

template <class T>
void show(const mystack<T> &v, const std::string &s) {
    std::cout << s << " | " << v << std::endl;
}

#define dbg(x) show(x, #x)
// #define dbg(x) 1

void t_construct() {
    std::cout << "====== stack construct ======" << std::endl;
    mystack<int> s1;
    dbg(s1);
    mystack<int> s2(tstl::deque<int>{1, 2, 3});
    dbg(s2);
    mystack<int> s3(s2);
    dbg(s3);
}

void t_operator_eq() {
    std::cout << "====== stack operator eq ======" << std::endl;
    mystack<int> s0({1, 2, 3});
    dbg(s0);
    mystack<int> s1 = s0;
    dbg(s1);
}

void t_top() {
    std::cout << "====== stack top ======" << std::endl;
    mystack<int> s1({1, 2, 3});
    dbg(s1);
    std::cout << "s1.top() = " << s1.top() << std::endl;
}

void t_empty_and_size() {
    std::cout << "====== stack empty & size ======" << std::endl;
    mystack<int> s1;
    dbg(s1);
    char c1 = s1.empty() ? 'Y' : 'N';
    std::cout << "s1.empty() = " << c1 << std::endl;
    std::cout << "s1.size() = " << s1.size() << std::endl;
    mystack<int> s2({1, 2, 3});
    dbg(s2);
    char c2 = s2.empty() ? 'Y' : 'N';
    std::cout << "s1.empty() = " << c2 << std::endl;
    std::cout << "s1.size() = " << s2.size() << std::endl;
}

void t_push_emplace_pop() {
    std::cout << "====== stack push & emplace & pop ======" << std::endl;
    mystack<int> s1;
    s1.push(1);
    s1.emplace(2);
    s1.emplace(3);
    s1.pop();
    dbg(s1);
}

void t_swap() {
    std::cout << "====== stack swap ======" << std::endl;
    mystack<int> s1({1, 2}), s2({4, 5, 6});
    tstl::swap(s1, s2);
    dbg(s1), dbg(s2);
}

} // namespace test_stack_detail

int test_stack() {
    std::cout << "||||||||| STACK TEST |||||||||" << std::endl;

    // 测试构造函数
    test_stack_detail::t_construct();

    // 测试赋值运算符
    test_stack_detail::t_operator_eq();

    // 测试 top
    test_stack_detail::t_top();

    // 测试 empty() & size()
    test_stack_detail::t_empty_and_size();

    // 测试 push() & emplace() & pop()
    test_stack_detail::t_push_emplace_pop();

    // 测试 swap()
    test_stack_detail::t_swap();

    return 0;
}

#endif
