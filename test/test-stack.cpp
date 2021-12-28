#ifndef TEST_TEST_STACK
#define TEST_TEST_STACK

#include "../src/vector.hpp"
#include "../src/deque.hpp"
#include "../src/stack.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class mystack : public tstl::stack<T, tstl::deque<T>> {
  public:
    using tstl::stack<T, tstl::deque<T>>::stack;
    friend std::ostream &operator<<(std::ostream &os, mystack &s) {
        os << "[stack] (" << s.size() << ") : [";
        for (auto i : s.c) {
            os << i << ", ";
        }
        return os << "]";
    }
};

int test_stack() {
    std::cout << "||||||||| STACK TEST |||||||||" << std::endl;

    mystack<double> s1;
    s1.push(1.5);
    s1.push(2);
    std::cout << s1 << std::endl;
    s1.pop();
    std::cout << s1 << std::endl;
    return 0;
}

#endif
