#ifndef TEST_TEST_STACK
#define TEST_TEST_STACK

#include "../src/vector.hpp"
#include "../src/stack.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <class T>
class mystack : public tstl::stack<T, tstl::vector<int>> {
  public:
    using tstl::stack<T, tstl::vector<int>>::stack;
    void show() const {
        std::cout << "[stack] (" << this->size() << ") : ";
        std::cout << "[";
        for (auto i : this->c) {
            std::cout << i << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

int test_stack() {
    mystack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.show();
    s1.pop();
    s1.show();
    return 0;
}

#endif
