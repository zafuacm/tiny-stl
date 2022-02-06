#ifndef TEST_TEST_DEQUE
#define TEST_TEST_DEQUE

#include "../src/deque.hpp"
#include "../src/algorithm.hpp"

template <class T>
using deque = tstl::deque<T, std::allocator<T>>;

TEST(DequeTest, Construct) {
    {
        deque<int> d;
        deque<int> expect = {};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d(3, 1);
        deque<int> expect = {1, 1, 1};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d(3, -1);
        d[2] = 2;
        deque<int> expect = {-1, -1, 2};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d1 = {1, 2, 3};
        deque<int> d2(d1.begin(), d1.end());
        deque<int> expect = {1, 2, 3};
        EXPECT_EQ(d2, expect);
    } {
        deque<int> d1 = {1, 2, 3};
        deque<int> d2(d1.rbegin(), d1.rend());
        deque<int> expect = {3, 2, 1};
        EXPECT_EQ(d2, expect);
    } {
        deque<int> d(deque<int>(3, 1));
        deque<int> expect = {1, 1, 1};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d(2048, -1);
        deque<int> expect = {};
        for (int i = 0; i < 2048; i++)
            expect.push_back(-1);
        EXPECT_EQ(d, expect);
    }
}

TEST(DequeTest, Assign) {
    {
        deque<int> d = {1, 2, 3};
        d.assign(3, -1);
        deque<int> expect = {-1, -1, -1};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d1 = {1, 2, 3};
        deque<int> d2 = {2, 3, 1};
        d1.assign(d2.begin(), d2.end());
        deque<int> expect = {2, 3, 1};
        EXPECT_EQ(d1, expect);
    } {
        deque<int> d = {1, 2, 3};
        d.assign({3, 2, 1});
        deque<int> expect = {3, 2, 1};
        EXPECT_EQ(d, expect);
    }
}

TEST(DequeTest, At) {
    deque<int> d = {1, 2, 3};
    EXPECT_EQ(2, d.at(1));
    EXPECT_THROW(d.at(5), std::out_of_range);
}

TEST(DequeTest, OperatorAt) {
    deque<int> d = {1, 2, 3};
    EXPECT_EQ(2, d[1]);
    // EXPECT_EQ(0, d[5]);
}

TEST(DequeTest, FrontAndBack) {
    deque<int> d = {1, 2, 3};
    EXPECT_EQ(d.front(), 1);
    EXPECT_EQ(d.back(), 3);
}

TEST(DequeTest, EmptyAndSize) {
    deque<int> d;
    EXPECT_EQ(d.empty(), true);
    d.push_back(1), d.push_front(2);
    EXPECT_EQ(d.empty(), false);
    EXPECT_EQ(d.size(), 2);
}

TEST(DequeTest, Clear) {
    deque<int> d = {1, 2, 3};
    d.clear();
    deque<int> expect = {};
    EXPECT_EQ(d, expect);
}

TEST(DequeTest, Insert) {
    {
        deque<int> d = {1, 2, 3};
        d.insert(d.begin() + 1, -10);
        deque<int> expect = {1, -10, 2, 3};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d = {1, 2, 3};
        d.insert(d.begin(), 2, -10);
        deque<int> expect = {-10, -10, 1, 2, 3};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d1 = {1, 2, 3};
        deque<int> d2 = {4, 5};
        d1.insert(d1.begin(), d2.rbegin(), d2.rend());
        deque<int> expect = {5, 4, 1, 2, 3};
        EXPECT_EQ(d1, expect);
    } {
        deque<int> d = {1, 2, 3};
        d.insert(d.begin(), {4, 5});
        deque<int> expect = {4, 5, 1, 2, 3};
        EXPECT_EQ(d, expect);
    }
}

TEST(DequeTest, PushPopEmplace) {
    {
        deque<int> d = {1, 2, 3};
        d.pop_back();
        d.pop_front();
        d.push_back(-1);
        d.push_front(-2);
        deque<int> expect = {-2, 2, -1};
        EXPECT_EQ(d, expect);
    } {
        deque<std::pair<int, int>> d = {{1, 1}, {2, 2}};
        d.pop_back();
        d.pop_front();
        d.emplace_back(3, 3);
        d.emplace_front(-1, -1);
        deque<std::pair<int, int>> expect = {{-1, -1}, {3, 3}};
        EXPECT_EQ(d, expect);
    }
}

TEST(DequeTest, Erase) {
    {
        deque<int> d = {1, 2, 3};
        d.erase(d.begin() + 1);
        deque<int> expect = {1, 3};
        EXPECT_EQ(d, expect);
    } {
        deque<int> d = {1, 2, 3, 4, 5};
        d.erase(d.begin() + 1, d.end() - 1);
        deque<int> expect = {1, 5};
        EXPECT_EQ(d, expect);
    }
}

TEST(DequeTest, Resize) {
    deque<int> d = {1, 2, 3, 4, 5};
    d.resize(1);
    deque<int> expect_1 = {1};
    EXPECT_EQ(d, expect_1);
    d.resize(5, -1);
    deque<int> expect_2 = {1, -1, -1, -1, -1};
    EXPECT_EQ(d, expect_2);
}

TEST(DequeTest, Swap) {
    deque<int> d1 = {1, 2, 3};
    deque<int> d2 = {4, 5};
    tstl::swap(d1, d2);
    deque<int> expect_1 = {4, 5};
    deque<int> expect_2 = {1, 2, 3};
    EXPECT_EQ(d1, expect_1);
    EXPECT_EQ(d2, expect_2);
}

#endif
