#ifndef TEST_TEST
#define TEST_TEST

#include <iostream>
#include <gtest/gtest.h>

#include "test-vector.cpp"
#include "test-stack.cpp"
#include "test-deque.cpp"
#include "test-list.cpp"
#include "test-multimap.cpp"

int main(int argc, char **argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif
