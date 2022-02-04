#ifndef TEST_TEST_MULTIMAP
#define TEST_TEST_MULTIMAP

#include "../src/multimap.hpp"

template <class K, class V>
using multimap = tstl::multimap<K, V, std::less<K>>;

TEST(MultimapTest, All) {
    multimap<int, int> mp;
    mp.insert({1, 2});
    mp.insert({2, 3});
    mp.insert({3, 4});
    mp.insert({3, 4});
    mp.insert({1, 3});
    multimap<int, int> expect = {{1, 2}, {1, 3}, {2, 3}, {3, 4}, {3, 4}};
    EXPECT_EQ(mp, expect);
}

#endif
