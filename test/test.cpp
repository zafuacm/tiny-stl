#ifndef TEST_TEST
#define TEST_TEST

#include "test-vector.cpp"
#include "test-stack.cpp"
#include "test-deque.cpp"

int main() {
    test_vector();
    std::cout << "\n" << std::endl;
    test_deque();
    std::cout << "\n" << std::endl;
    test_stack();
    return 0;
}

#endif
