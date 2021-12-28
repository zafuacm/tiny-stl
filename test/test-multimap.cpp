#ifndef TEST_TEST_MULTIMAP
#define TEST_TEST_MULTIMAP

#include <iostream>
#include "../src/multimap.hpp"

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
    return os << "{" << p.first << ", " << p.second << "}";
}

template <class K, class V>
class mymultimap : public tstl::multimap<K, V> {
  public:
    using tstl::multimap<K, V>::multimap;
    friend std::ostream &operator<<(std::ostream &os, const mymultimap &m) {
        os << "[multimap] (" << m.size() << ") : [";
        for (auto i : m) {
            os << i << ", ";
        }
        return os << "]";
    }
};

namespace test_mutimap_detail {

template <class K, class V>
void show(const mymultimap<K, V> &m, const std::string &s) {
    std::cout << s << " | " << m << std::endl;
}

#define dbg(x) show(x, #x)
// #define dbg(x) 1

void test_all() {
    mymultimap<int, int> mp;
    dbg(mp);
    mp.insert({1, 2});
    mp.insert({2, 3});
    mp.insert({3, 4});
    mp.insert({3, 4});
    mp.insert({1, 3});
    dbg(mp);
    std::cout << "*mp.begin() = " << *mp.begin() << std::endl;
    dbg(mp);
    auto tmp = mp.find(1);
    std::cout << "*mp.find(1) = " << *tmp << std::endl;
    mp.erase(tmp);
    dbg(mp);
    mp.clear();
    dbg(mp);
}

} // namespace test_mutimap_detail

int test_multimap() {
    std::cout << "||||||||| MULTIMAP TEST |||||||||" << std::endl;
    test_mutimap_detail::test_all();
    return 0;
}

#endif
