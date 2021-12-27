#ifndef TEST_TEST_VECTOR
#define TEST_TEST_VECTOR

#include "../src/vector.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class myvector : public tstl::vector<T> {
  public:
    using tstl::vector<T>::vector;
    void show() const {
        std::cout << "[vector] (" << this->size() << ") : ";
        std::cout << "[";
        for (auto i : *this) {
            std::cout << i << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

int test_vector() {
    myvector<int> v1 = {1, 2, 3, 4};
    v1 = {4, 3, 2, 1};
    myvector<int> v2 = v1;
    v2 = myvector<int>(5, -5);
    v1.insert(v1.begin() + 1, -1);
    myvector<int> v3 = {v1.rbegin(), v1.rend()};
    v1.show();
    v2.show();
    v3.show();
    tstl::reverse(v3.begin(), v3.end());
    v3.insert(v3.end(), 2);
    v3.show();
    bool ret = v1 == v2;
    printf("v1 == v2 : %c\n", ret ? 'Y' : 'N');
    return 0;
}

#endif
