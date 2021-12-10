#include "../src/container/vector.cpp"
#include <iostream>

template <typename T>
std::ostream &operator<<(std::ostream &os, tstl::vector<T> &v) {
    os << "[vector] : [";
    for (size_t i = 0; i < v.size(); i++) {
        os << v[i] << ", ";
    }
    os << "]";
    return os;
}

int main() {
    tstl::vector<int> v1(5);
    tstl::vector<int> v2(5);
    v1[0] = 1, v2[0] = 1;
    v1[1] = 2, v2[1] = 2;
    v1[2] = 3, v2[2] = 4;
    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    bool ret = v1 == v2;
    printf("v1 == v2 : %c\n", ret ? 'Y' : 'N');
    return 0;
}
