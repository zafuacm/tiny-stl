#ifndef TEST_TEST_LIST
#define TEST_TEST_LIST

#include "../src/list.hpp"

struct Test {
    int a;
    char ch;
    Test(int _a, char _c) : a(_a), ch(_c) {
        std::cout << "construct" << std::endl;
    }
    friend std::istream &operator>>(std::istream &input, Test &t) {
        input >> t.a >> t.ch;
        return input;
    }
    friend std::ostream &operator<<(std::ostream &output, Test &t) {
        output << "int = " << t.a << ", ch = " << t.ch;
        return output;
    }
};

template <class T>
using list = tstl::list<T, std::allocator<T>>;

TEST(ListTest, Construct) {
    {
        list<int> l(5, 3);
        list<int> expect = {3, 3, 3, 3, 3};
        EXPECT_EQ(l, expect);
    } {
        list<int> l1 = {1, 2, 3};
        list<int> l2(l1.rbegin(), l1.rend());
        list<int> expect = {3, 2, 1};
        EXPECT_EQ(l2, expect);
    } {
        list<int> l1 = {1, 2, 3};
        list<int> l2(l1);
        list<int> expect = {1, 2, 3};
        EXPECT_EQ(l2, expect);
    } {
        list<int> l1(list<int>{1, 2, 3});
        list<int> expect = {1, 2, 3};
        EXPECT_EQ(l1, expect);
    }
}


TEST(ListTest, Modify) {
    {
        list<int> l = {1, 2};
        l.pop_back();
        l.pop_front();
        l.push_back(3);
        l.push_front(4);
        list<int> expect = {4, 3};
        EXPECT_EQ(l, expect);
    } {
        list<int> l1 = {1, 2, 3};
        list<int> l2(l1.rbegin(), l1.rend());
        list<int> expect = {3, 2, 1};
        EXPECT_EQ(l2, expect);
    } {
        list<int> l1 = {1, 2, 3};
        list<int> l2 = {};
        l2.assign(l1.begin(), l1.end());
        list<int> expect = {1, 2, 3};
        EXPECT_EQ(l2, expect);
    } {
        list<int> l = {1, 2, 3};
        l.resize(1);
        list<int> expect = {1};
        EXPECT_EQ(l, expect);
    } {
        list<int> l = {1, 1, 2, 2};
        l.unique();
        list<int> expect = {1, 2};
        EXPECT_EQ(l, expect);
    } {
        list<int> l1 = {1, 2};
        list<int> l2 = {3, 4};
        l2.splice(l2.end(), l1);
        list<int> expect = {3, 4, 1, 2};
        EXPECT_EQ(l2, expect);
    }
}

TEST(ListTest, Sort) {
    {
        list<int> l = {2, 4, 8, 5, 7, 9, 1, 2};
        l.sort();
        list<int> expect_1 = {1, 2, 2, 4, 5, 7, 8, 9};
        EXPECT_EQ(l, expect_1);
        l.sort([](int a, int b) { return a > b; });
        list<int> expect_2 = {9, 8, 7, 5, 4, 2, 2, 1};
        EXPECT_EQ(l, expect_2);
    } {
        list<int> l1 = {1, 3, 5};
        list<int> l2 = {2, 4, 6};
        l2.merge(l1);
        list<int> expect = {1, 2, 3, 4, 5, 6};
        EXPECT_EQ(l2, expect);
    }
}

#endif
