#ifndef TEST_TEST_VECTOR
#define TEST_TEST_VECTOR

#include "../src/vector.hpp"
#include "../src/algorithm.hpp"
#include <iostream>
#include "vector"

template <class T>
class myvector : public tstl::vector<T> {
  public:
    using tstl::vector<T>::vector;
    friend std::ostream &operator<<(std::ostream &os, const myvector &v) {
        os << "[vector] (" << v.size() << ") : [";
        for (auto i : v) {
            os << i << ", ";
        }
        os << "]";
        return os;
    }
};

namespace test_vector_detail {

template <class T>
void show(const myvector<T> &v, const std::string &s) {
    std::cout << s << " | " << v << std::endl;
}

#define dbg(x) show(x, #x)
// #define dbg(x) 1

void t_construct() {
    std::cout << "====== vector construct ======" << std::endl;
    myvector<int> v1;
    dbg(v1);
    myvector<int> v2(3, 0);
    dbg(v2);
    myvector<int> v3(3);
    v3[2] = 1;
    dbg(v3);
    myvector<int> v4_1(v3.begin(), v3.end());
    dbg(v4_1);
    myvector<int> v4_2(v3.rbegin(), v3.rend());
    dbg(v4_2);
    myvector<int> v5(v3);
    dbg(v5);
    myvector<int> v6(myvector<int>(3, 1));
    dbg(v6);
    myvector<int> v7{1, 2, 3};
    dbg(v7);
}

void t_operator_eq() {
    std::cout << "====== vector operator eq ======" << std::endl;
    myvector<int> v1, v2, v3, v4;
    v1 = {1, 2};
    dbg(v1);
    v2 = v1;
    dbg(v2);
    v3 = v2 = v1;
    dbg(v3);
    v4 = myvector<int>(4, -1);
    dbg(v4);
}

void t_assign() {
    std::cout << "====== vector assign ======" << std::endl;
    myvector<int> v1 = {1, 2, 3};
    v1.assign(5, -1);
    dbg(v1);
    myvector<int> v2 = {1, 2, 3};
    v2.assign(v1.begin(), v1.end());
    dbg(v2);
    myvector<int> v3 = v2;
    v3.assign({2, 3, 4});
    dbg(v3);
}

void t_at() {
    std::cout << "====== vector at ======" << std::endl;
    myvector<int> v1 = {1, 2, 3};
    dbg(v1);
    std::cout << "v1.at(1) = " << v1.at(1) << std::endl;
    //    std::cout << "v1.at(5) = " << v1.at(5) << std::endl;
}

void t_operator_at() {
    std::cout << "====== vector operator[] ======" << std::endl;
    myvector<int> v1 = {1, 2, 3};
    dbg(v1);
    std::cout << "v1[1] = " << v1[1] << std::endl;
    //    std::cout << "v1[5] = " << v1[5] << std::endl;
}

void t_front_and_back() {
    std::cout << "====== vector front & back ======" << std::endl;
    myvector<int> v1 = {1, 2, 3};
    dbg(v1);
    std::cout << "v1.front() = " << v1.front() << std::endl;
    std::cout << "v1.back() = " << v1.back() << std::endl;
}

void t_empty_size_capacity() {
    std::cout << "====== vector empty & size & capacity ======" << std::endl;
    myvector<int> v1;
    dbg(v1);
    char c1 = v1.empty() ? 'Y' : 'N';
    std::cout << "v1.empty() = " << c1 << std::endl;
    std::cout << "v1.size() = " << v1.size() << std::endl;
    std::cout << "v1.capacity() = " << v1.capacity() << std::endl;
    myvector<int> v2 = {1, 2, 3};
    v2.push_back(4);
    dbg(v2);
    char c2 = v2.empty() ? 'Y' : 'N';
    std::cout << "v2.empty() = " << c2 << std::endl;
    std::cout << "v2.size() = " << v2.size() << std::endl;
    std::cout << "v2.capacity() = " << v2.capacity() << std::endl;
}

void t_clear() {
    std::cout << "====== vector clear ======" << std::endl;
    myvector<int> v1 = {1, 2, 3};
    dbg(v1);
    v1.clear();
    dbg(v1);
}

void t_insert() {
    std::cout << "====== vector insert ======" << std::endl;
    myvector<int> v0 = {1, 2, 3};
    dbg(v0);
    auto v1 = v0;
    v1.insert(v1.begin() + 1, -10);
    dbg(v1);
    auto v2 = v0;
    v2.insert(v2.begin() + 1, 5, -10);
    dbg(v2);
    auto v3 = v0;
    v3.insert(v3.begin() + 1, v0.rbegin(), v0.rend());
    dbg(v3);
    auto v4 = v0;
    v4.insert(v4.begin() + 1, {-5, -4, -3});
    dbg(v4);
}

void t_push_pop_emplace() {
    std::cout << "====== vector push_back & pop_back & emplace_back ======" << std::endl;
    myvector<int> v0 = {1, 2, 3};
    dbg(v0);
    auto v1 = v0;
    v1.push_back(-1);
    dbg(v1);
    auto v2 = v0;
    v2.emplace_back(-2);
    dbg(v2);
    auto v3 = v0;
    v3.pop_back();
    dbg(v3);
}

void t_erase() {
    std::cout << "====== vector erase ======" << std::endl;
    myvector<int> v0 = {1, 2, 3, 4, 5};
    dbg(v0);
    auto v1 = v0;
    v1.erase(v1.begin() + 1);
    dbg(v1);
    auto v2 = v0;
    v2.erase(v2.begin() + 1, v2.end() - 1);
    dbg(v2);
}

void t_resize() {
    std::cout << "====== vector resize ======" << std::endl;
    myvector<int> v0 = {1, 2, 3, 4, 5};
    dbg(v0);
    auto v1 = v0;
    v1.resize(1);
    dbg(v1);
    auto v2 = v0;
    v2.resize(8);
    dbg(v2);
}

void t_swap() {
    std::cout << "====== vector swap ======" << std::endl;
    myvector<int> v1 = {1, 2, 3};
    myvector<int> v2 = {3, 2, 1};
    tstl::swap(v1, v2);
    dbg(v1), dbg(v2);
}

} // namespace test_vector_detail

int test_vector() {
    std::cout << "||||||||| VECTOR TEST |||||||||" << std::endl;

    // 测试构造函数
    test_vector_detail::t_construct();

    // 测试赋值运算符
    test_vector_detail::t_operator_eq();

    // 测试 assign
    test_vector_detail::t_assign();

    // 测试 at
    test_vector_detail::t_at();

    // 测试 operator[]
    test_vector_detail::t_operator_at();

    // 测试 front 和 back
    test_vector_detail::t_front_and_back();

    // 测试 empty
    test_vector_detail::t_empty_size_capacity();

    // 测试 clear
    test_vector_detail::t_clear();

    // 测试 insert
    test_vector_detail::t_insert();

    // 测试 push_back | pop_back | emplace_back
    test_vector_detail::t_push_pop_emplace();

    // 测试 erase
    test_vector_detail::t_erase();

    // 测试 resize
    test_vector_detail::t_resize();

    // 测试 swap
    test_vector_detail::t_swap();

    /**
     * 不必测试的函数如下。部分是因为不具有可观察性，另外部分是其他测试中已经包含。
     * - begin() & end() 系列
     * - data()
     * - reserve()
     */

    return 0;
}

#endif
