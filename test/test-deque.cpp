#ifndef TEST_TEST_DEQUE
#define TEST_TEST_DEQUE

#include "../src/deque.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class mydeque : public tstl::deque<T> {
  public:
    using tstl::deque<T>::deque;
    friend std::ostream &operator<<(std::ostream &os, const mydeque &d) {
        os << "[deque] (" << d.size() << ") : [";
        for (auto i : d) {
            os << i << ", ";
        }
        return os << "]";
    }
};

namespace test_deque_detail {

template <class T>
void show(const mydeque<T> &v, const std::string &s) {
    std::cout << s << " | " << v << std::endl;
}

#define dbg(x) show(x, #x)
// #define dbg(x) 1

void t_construct() {
    std::cout << "====== deque construct ======" << std::endl;
    mydeque<int> d1;
    dbg(d1);
    mydeque<int> d2(3, 0);
    dbg(d2);
    mydeque<int> d3(3);
    d3[2] = 1;
    dbg(d3);
    mydeque<int> d4_1(d3.begin(), d3.end());
    dbg(d4_1);
    mydeque<int> d4_2(d3.rbegin(), d3.rend());
    dbg(d4_2);
    mydeque<int> d5(d3);
    dbg(d5);
    mydeque<int> d6(mydeque<int>(3, 1));
    dbg(d6);
    mydeque<int> d7{1, 2, 3};
    dbg(d7);
}

void t_operator_eq() {
    std::cout << "====== deque operator eq ======" << std::endl;
    mydeque<int> d1, d2, d3, d4;
    d1 = {1, 2};
    dbg(d1);
    d2 = d1;
    dbg(d2);
    d3 = d2 = d1;
    dbg(d3);
    d4 = mydeque<int>(4, -1);
    dbg(d4);
}

void t_assign() {
    std::cout << "====== deque assign ======" << std::endl;
    mydeque<int> d1 = {1, 2, 3};
    d1.assign(5, -1);
    dbg(d1);
    mydeque<int> d2 = {1, 2, 3};
    d2.assign(d1.begin(), d1.end());
    dbg(d2);
    mydeque<int> d3 = d2;
    d3.assign({2, 3, 4});
    dbg(d3);
}

void t_at() {
    std::cout << "====== deque at ======" << std::endl;
    mydeque<int> d1 = {1, 2, 3};
    dbg(d1);
    std::cout << "d1.at(1) = " << d1.at(1) << std::endl;
    //    std::cout << "d1.at(5) = " << d1.at(5) << std::endl;
}

void t_operator_at() {
    std::cout << "====== deque operator[] ======" << std::endl;
    mydeque<int> d1 = {1, 2, 3};
    dbg(d1);
    std::cout << "d1[1] = " << d1[1] << std::endl;
    //    std::cout << "d1[5] = " << d1[5] << std::endl;
}

void t_front_and_back() {
    std::cout << "====== deque front & back ======" << std::endl;
    mydeque<int> d1 = {1, 2, 3};
    dbg(d1);
    std::cout << "d1.front() = " << d1.front() << std::endl;
    std::cout << "d1.back() = " << d1.back() << std::endl;
}

void t_empty_size() {
    std::cout << "====== deque empty & size ======" << std::endl;
    mydeque<int> d1;
    dbg(d1);
    char c1 = d1.empty() ? 'Y' : 'N';
    std::cout << "d1.empty() = " << c1 << std::endl;
    std::cout << "d1.size() = " << d1.size() << std::endl;
    mydeque<int> d2 = {1, 2, 3};
    d2.push_back(4);
    dbg(d2);
    char c2 = d2.empty() ? 'Y' : 'N';
    std::cout << "d2.empty() = " << c2 << std::endl;
    std::cout << "d2.size() = " << d2.size() << std::endl;
}

void t_clear() {
    std::cout << "====== deque clear ======" << std::endl;
    mydeque<int> d1 = {1, 2, 3};
    dbg(d1);
    d1.clear();
    dbg(d1);
}

void t_insert() {
    std::cout << "====== deque insert ======" << std::endl;
    mydeque<int> d0 = {1, 2, 3};
    dbg(d0);
    auto d1 = d0;
    d1.insert(d1.begin() + 1, -10);
    dbg(d1);
    auto d2 = d0;
    d2.insert(d2.begin() + 1, 5, -10);
    dbg(d2);
    auto d3 = d0;
    d3.insert(d3.begin() + 1, d0.rbegin(), d0.rend());
    dbg(d3);
    auto d4 = d0;
    d4.insert(d4.begin() + 1, {-5, -4, -3});
    dbg(d4);
}

void t_push_pop_emplace() {
    std::cout << "====== deque push & pop & emplace ======" << std::endl;
    mydeque<int> d0 = {1, 2, 3};
    dbg(d0);
    auto d1 = d0;
    d1.push_back(-1);
    d1.push_front(-1);
    dbg(d1);
    auto d2 = d0;
    d2.emplace_front(-2);
    d2.emplace_back(-2);
    dbg(d2);
    auto d3 = d0;
    d3.pop_front();
    d3.pop_back();
    dbg(d3);
}

void t_erase() {
    std::cout << "====== deque erase ======" << std::endl;
    mydeque<int> d0 = {1, 2, 3, 4, 5};
    dbg(d0);
    auto d1 = d0;
    d1.erase(d1.begin() + 1);
    dbg(d1);
    auto d2 = d0;
    d2.erase(d2.begin() + 1, d2.end() - 1);
    dbg(d2);
}

void t_resize() {
    std::cout << "====== deque resize ======" << std::endl;
    mydeque<int> d0 = {1, 2, 3, 4, 5};
    dbg(d0);
    auto d1 = d0;
    d1.resize(1);
    dbg(d1);
    auto d2 = d0;
    d2.resize(8);
    dbg(d2);
}

void t_swap() {
    std::cout << "====== deque swap ======" << std::endl;
    mydeque<int> d1 = {1, 2, 3};
    mydeque<int> d2 = {3, 2, 1};
    tstl::swap(d1, d2);
    dbg(d1), dbg(d2);
}

} // namespace test_deque_detail

int test_deque() {
    std::cout << "||||||||| DEQUE TEST |||||||||" << std::endl;

    // 测试构造函数
    test_deque_detail::t_construct();

    // 测试赋值运算符
    test_deque_detail::t_operator_eq();

    // 测试 assign
    test_deque_detail::t_assign();

    // 测试 at
    test_deque_detail::t_at();

    // 测试 operator[]
    test_deque_detail::t_operator_at();

    // 测试 front 和 back
    test_deque_detail::t_front_and_back();

    // 测试 empty
    test_deque_detail::t_empty_size();

    // 测试 clear
    test_deque_detail::t_clear();

    // 测试 insert
    test_deque_detail::t_insert();

    // 测试 push | pop | emplace
    test_deque_detail::t_push_pop_emplace();

    // 测试 erase
    test_deque_detail::t_erase();

    // 测试 resize
    test_deque_detail::t_resize();

    // 测试 swap
    test_deque_detail::t_swap();

    return 0;
}

#endif
