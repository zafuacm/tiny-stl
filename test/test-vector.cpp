#ifndef TEST_TEST_VECTOR
#define TEST_TEST_VECTOR

#include "../src/vector.hpp"
#include "../src/algorithm.hpp"

template <class T>
using vec = tstl::vector<T, std::allocator<T>>;

TEST(VectorTest, Construct) {
    {
        vec<int> v;
        vec<int> expect = {};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v(3, -1);
        vec<int> expect = {-1, -1, -1};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v(3, -1);
        v[2] = 1;
        vec<int> expect = {-1, -1, 1};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v1 = {1, 2, 3};
        vec<int> v2(v1.begin(), v1.end());
        vec<int> expect = {1, 2, 3};
        EXPECT_EQ(v2, expect);
    }
    {
        vec<int> v1 = {1, 2, 3};
        vec<int> v2(v1.rbegin(), v1.rend());
        vec<int> expect = {3, 2, 1};
        EXPECT_EQ(v2, expect);
    }
    {
        vec<int> v(vec<int>(vec<int>(3, 1)));
        vec<int> expect = {1, 1, 1};
        EXPECT_EQ(v, expect);
    }
}

TEST(VectorTest, Assign) {
    {
        vec<int> v = {1, 2, 3};
        v.assign(5, -1);
        vec<int> expect = {-1, -1, -1, -1, -1};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v1 = {1, 2, 3};
        vec<int> v2 = {2, 3, 1};
        v2.assign(v1.begin(), v1.end());
        vec<int> expect = {1, 2, 3};
        EXPECT_EQ(v2, expect);
    }
    {
        vec<int> v = {1, 2, 3};
        v.assign({3, 2, 1});
        vec<int> expect = {3, 2, 1};
        EXPECT_EQ(v, expect);
    }
}

TEST(VectorTest, At) {
    vec<int> v = {1, 2, 3};
    EXPECT_EQ(2, v.at(1));
    EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST(VectorTest, OperatorAt) {
    vec<int> v = {1, 2, 3};
    EXPECT_EQ(2, v[1]);
    //    EXPECT_EQ(0, v[5]);
}

TEST(VectorTest, FrontAndBack) {
    vec<int> v = {1, 2, 3};
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
}

TEST(VectorTest, EmptySizeCapacity) {
    vec<int> v;
    EXPECT_EQ(v.empty(), true);
    v = {1, 2, 3};
    v.push_back(4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v.empty(), false);
    EXPECT_EQ(v.capacity(), 6);
}

TEST(VectorTest, Clear) {
    vec<int> v = {1, 2, 3};
    v.clear();
    vec<int> expect = {};
    EXPECT_EQ(v, expect);
}

TEST(VectorTest, Insert) {
    {
        vec<int> v = {1, 2, 3};
        v.insert(v.begin() + 1, -10);
        vec<int> expect = {1, -10, 2, 3};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v = {1, 2, 3};
        v.insert(v.begin() + 1, 2, -10);
        vec<int> expect = {1, -10, -10, 2, 3};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v1 = {1, 2, 3};
        vec<int> v2 = v1;
        v2.insert(v2.begin(), v1.rbegin(), v1.rend());
        vec<int> expect = {3, 2, 1, 1, 2, 3};
        EXPECT_EQ(v2, expect);
    }
    {
        vec<int> v = {1, 2, 3};
        v.insert(v.begin(), {-3, -2, -1});
        vec<int> expect = {-3, -2, -1, 1, 2, 3};
        EXPECT_EQ(v, expect);
    }
}

TEST(VectorTest, PushPopEmplace) {
    {
        vec<int> v = {1, 2, 3};
        v.push_back(-1);
        vec<int> expect = {1, 2, 3, -1};
        EXPECT_EQ(v, expect);
    }
    {
        vec<std::pair<int, int>> v = {{1, 1}, {2, 2}};
        v.emplace_back(-1, -1);
        vec<std::pair<int, int>> expect = {{1, 1}, {2, 2}, {-1, -1}};
        EXPECT_EQ(v, expect);
    }
}

TEST(VectorTest, Erase) {
    {
        vec<int> v = {1, 2, 3};
        v.erase(v.begin() + 1);
        vec<int> expect = {1, 3};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v = {1, 2, 3, 4, 5};
        v.erase(v.begin() + 1, v.end() - 1);
        vec<int> expect = {1, 5};
        EXPECT_EQ(v, expect);
    }
}

TEST(VectorTest, Resize) {
    {
        vec<int> v = {1, 2, 3};
        v.resize(1);
        vec<int> expect = {1};
        EXPECT_EQ(v, expect);
    }
    {
        vec<int> v = {1, 2, 3};
        v.resize(5);
        vec<int> expect = {1, 2, 3, 0, 0};
        EXPECT_EQ(v, expect);
    }
}

TEST(VectorTest, Swap) {
    vec<int> v1 = {1, 2, 3};
    vec<int> v2 = {4, 5};
    tstl::swap(v1, v2);
    vec<int> expect_1 = {4, 5};
    vec<int> expect_2 = {1, 2, 3};
    EXPECT_EQ(v1, expect_1);
    EXPECT_EQ(v2, expect_2);
}

#endif
