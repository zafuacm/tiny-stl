#ifndef TEST_TEST
#define TEST_TEST

#include "test-vector.cpp"
#include "test-stack.cpp"
#include "test-deque.cpp"
#include "test-list.cpp"
#include "test-multimap.cpp"

int main() {
    test_vector();
    std::cout << "\n" << std::endl;
    test_deque();
    std::cout << "\n" << std::endl;
    test_stack();
    std::cout << "\n" << std::endl;
    test_list();
    std::cout << "\n" << std::endl;
    test_multimap();
    return 0;
}

#endif
