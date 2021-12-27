#ifndef TEST_TEST_VECTOR
#define TEST_TEST_VECTOR

#include "../src/vector.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class myvector : public tstl::vector<T> {
  public:
    using tstl::vector<T>::vector;
    friend std::ostream &operator<<(std::ostream &os, myvector &v) {
        os << "[vector] (" << v.size() << ") : [";
        for (auto i : v) {
            os << i << ", ";
        }
        os << "]";
        return os;
    }
};

int test_vector() {
    myvector<int> v1 = {1, 2, 3, 4};
    v1 = {4, 3, 2, 1};
    myvector<int> v2 = v1;
    v2 = myvector<int>(5, -5);
    v1.insert(v1.begin() + 1, -1);
    myvector<int> v3 = {v1.rbegin(), v1.rend()};
    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << v3 << std::endl;
    tstl::reverse(v3.begin(), v3.end());
    v3.insert(v3.end(), 2);
    std::cout << v3 << std::endl;
    bool ret = v1 == v2;
    printf("v1 == v2 : %c\n", ret ? 'Y' : 'N');
    return 0;
}

#endif
