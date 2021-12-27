#ifndef TEST_TEST_DEQUE
#define TEST_TEST_DEQUE

#include "../src/deque.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class mydeque : public tstl::deque<T> {
  public:
    using tstl::deque<T>::deque;
    void show() const {
        std::cout << "[deque] (" << this->size() << ") : ";
        std::cout << "[";
        for (auto i : *this) {
            std::cout << i << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

int test_deque() {
    mydeque<int> d1 = {1, 2, 3, 4};
    d1.show();
    d1 = {4, 3, 2, 1};
    mydeque<int> d2 = d1;
    d2 = mydeque<int>(5, -5);
    d1.insert(d1.begin() + 1, -1);
    d1.show();
    d2.show();
    bool ret = d1 == d2;
    printf("d1 == d2 : %c\n", ret ? 'Y' : 'N');
    return 0;
}

#endif
