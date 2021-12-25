#ifndef TEST_TEST_STACK
#define TEST_TEST_STACK

#include "../src/stack.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

int test_stack() {
    tstl::stack<int, tstl::vector<int>> s1;
    s1.push(1);
    s1.push(2);
    s1.pop();
    std::cout << s1.top() << std::endl;
    return 0;
}

#endif
