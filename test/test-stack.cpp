#ifndef TEST_TEST_STACK
#define TEST_TEST_STACK

#include "../src/vector.hpp"
#include "../src/deque.hpp"
#include "../src/stack.hpp"
#include "../src/list.hpp"
#include "../src/algorithm.hpp"

template <class T>
using container = tstl::vector<T, std::allocator<T>>;

template <class T>
using stack = tstl::stack<T, container<T>>;

template <class T>
container<T> dump(stack<T> s) {
    container<T> c;
    while (!s.empty()) {
        c.push_back(s.top());
        s.pop();
    }
    tstl::reverse(c.begin(), c.end());
    return c;
}

TEST(StackTest, Construct) {
    {
        stack<int> s;
        container<int> expect = {};
        EXPECT_EQ(dump(s), expect);
    } {
        stack<int> s({1, 2, 3});
        container<int> expect = {1, 2, 3};
        EXPECT_EQ(dump(s), expect);
    } {
        stack<int> s(container<int>{1, 2, 3});
        container<int> expect = {1, 2, 3};
        EXPECT_EQ(dump(s), expect);
    }
}

TEST(StackTest, Top) {
    stack<int> s1({1, 2, 3});
    EXPECT_EQ(s1.top(), 3);
}

TEST(StackTest, EmptyAndSize) {
    stack<int> s;
    EXPECT_EQ(s.empty(), true);
    s.push(1), s.push(2);
    EXPECT_EQ(s.empty(), false);
    EXPECT_EQ(s.size(), 2);
}

TEST(StackTest, PushEmplacePop) {
    {
        stack<int> s({1, 2, 3});
        s.push(-1);
        container<int> expect = {1, 2, 3, -1};
        EXPECT_EQ(dump(s), expect);
    } {
        stack<std::pair<int, int>> s({{1, 1}, {2, 2}});
        s.emplace(-1, -1);
        container<std::pair<int, int>> expect = {{1, 1}, {2, 2}, {-1, -1}};
        EXPECT_EQ(dump(s), expect);
    }
}

TEST(StackTest, Swap) {
    stack<int> s1({1, 2, 3});
    stack<int> s2({4, 5});
    tstl::swap(s1, s2);
    container<int> c1 = {4, 5};
    container<int> c2 = {1, 2, 3};
    EXPECT_EQ(dump(s1), c1);
    EXPECT_EQ(dump(s2), c2);
}

#endif
