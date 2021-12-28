#ifndef TEST_TEST_LIST
#define TEST_TEST_LIST

#include <iostream>
#include "../src/list.hpp"

struct Test {
    int a;
    char ch;
    Test(int _a, char _c) : a(_a), ch(_c) {
        std::cout << "construct" << std::endl;
    }
    friend std::istream &operator>>(std::istream &input, Test &t) {
        input >> t.a >> t.ch;
        return input;
    }
    friend std::ostream &operator<<(std::ostream &output, Test &t) {
        output << "int = " << t.a << ", ch = " << t.ch;
        return output;
    }
};

template <class T>
class mylist : public tstl::list<T> {
  public:
    using tstl::list<T>::list;
    friend std::ostream &operator<<(std::ostream &os, const mylist &l) {
        os << "[list] (" << l.size() << ") : [";
        for (auto i : l) {
            os << i << ", ";
        }
        return os << "]";
    }
};

namespace test_list_detail {

template <class T>
void show(const mylist<T> &v, const std::string &s) {
    std::cout << s << " | " << v << std::endl;
}

#define dbg(x) show(x, #x)
// #define dbg(x) 1

void constructedTest() {
    std::cout << "====== list construct ======" << std::endl;
    mylist<int> l1(5, 3);
    mylist<int> l2(7, 6);
    mylist<int>::iterator tmp = l2.begin();
    tstl::advance(tmp, 2);
    mylist<int> l3(tmp, --l2.end());
    mylist<int> l4(l3);
    dbg(l1);
    dbg(l2);
    dbg(l3);
    dbg(l4);
    l4 = l2;
    dbg(l4);
    l4.swap(l1);
    dbg(l1);
    dbg(l4);
}

void modifyListTest() {
    std::cout << "====== list modify ======" << std::endl;
    mylist<double> l(2, 1.5);
    dbg(l);

    l.push_back(2.5);
    l.push_back(5.1);
    l.push_front(3.5);
    l.push_front(4.1);
    dbg(l);

    l.pop_back();
    l.pop_fornt();

    dbg(l);

    mylist<double> l2;
    l2.assign(l.begin(), l.end());
    l.assign(6, 2.2);
    dbg(l2);

    dbg(l);
    l.resize(8);

    dbg(l);
    mylist<double>::iterator tmp = l.begin();
    tstl::advance(tmp, 3);
    l.insert(tmp, 4.6);

    dbg(l);

    l.erase(--tmp);

    dbg(l);
    tmp = l.begin();
    tstl::advance(tmp, 2);
    // l.splice(tmp, l2);
    // l.splice(tmp, l2, --l2.end());
    l.splice(tmp, l2, ++l2.begin(), --l2.end());

    dbg(l);

    // l.unique();
    // std::cout << l);
    // std::cout << "size = " << l.size() << '\n';
    // l.erase(l.begin(),l.end());
    // std::cout << l);
    // std::cout << "size = " << l.size() << '\n';
    // l.clear();
    // std::cout << l);
    // std::cout << "size = " << l.size() << '\n';
}

void move_element_Test() {
    std::cout << "====== list move ======" << std::endl;
    mylist<int> list1(2, 3);
    mylist<int> list2(2, 2);
    dbg(list1);
    list2.push_back(3);
    list2.push_back(4);
    dbg(list2);
    list1.merge(list2);
    dbg(list1);

    list1.remove(2);
    dbg(list1);
}

void sortTest() {
    std::cout << "====== list sort ======" << std::endl;
    mylist<int> list;
    for (int i = 0; i < 10; i++)
        list.push_back(rand() % 10 + 1);

    dbg(list);
    list.sort();
    dbg(list);
    list.sort([&](int a, int b) { return a > b; });
    dbg(list);
}

} // namespace test_list_detail

int test_list() {
    std::cout << "||||||||| LIST TEST |||||||||" << std::endl;
    test_list_detail::constructedTest();
    test_list_detail::modifyListTest();
    test_list_detail::move_element_Test();
    test_list_detail::sortTest();
    return 0;
}

#endif
