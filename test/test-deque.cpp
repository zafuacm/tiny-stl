#ifndef TEST_TEST_DEQUE
#define TEST_TEST_DEQUE

#include "../src/deque.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class mydeque : public tstl::deque<T> {
  public:
    using tstl::deque<T>::deque;
    friend std::ostream &operator<<(std::ostream &os, mydeque &d) {
        os << "[deque] (" << d.size() << ") : [";
        for (auto i : d) {
            os << i << ", ";
        }
        os << "]";
        return os;
    }
};

int test_deque() {
    mydeque<int> d1 = {1, 2, 3, 4};
    std::cout << d1 << std::endl;
    d1 = {4, 3, 2, 1};
    mydeque<int> d2 = d1;
    d2 = mydeque<int>(5, -5);
    d1.insert(d1.begin() + 1, -1);
    std::cout << d1 << std::endl;
    std::cout << d2 << std::endl;
    bool ret = d1 == d2;
    printf("d1 == d2 : %c\n", ret ? 'Y' : 'N');
    return 0;
}

#endif
