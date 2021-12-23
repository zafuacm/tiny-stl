#include "../src/vector.hpp"
#include "../src/algorithm.hpp"
#include <iostream>

template <typename T>
std::ostream &operator<<(std::ostream &os, tstl::vector<T> &v) {
    os << "[vector] : [";
    for (auto i : v) {
        os << i << ", ";
    }
    os << "]";
    return os;
}

int main() {
    tstl::vector<int> v1 = {1, 2, 3, 4};
    v1 = {4, 3, 2, 1};
    tstl::vector<int> v2 = v1;
    v2 = tstl::vector<int>(5, -5);
    v1.insert(v1.begin() + 1, -1);
    tstl::vector<int> v3 = {v1.rbegin(), v1.rend()};
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
