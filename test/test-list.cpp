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

// 针对各种构造函数的测试（还未测试右值类型）
void constructedTest() {
    std::cout << "构造函数测试：" << std::endl;
    tstl::list<int> intlist1(5, 3);
    tstl::list<int> intlist2(7, 6);
    tstl::list<int>::iterator tmp = intlist2.begin();
    tstl::advance(tmp, 2);
    tstl::list<int> intlist3(tmp, --intlist2.end());
    tstl::list<int> intlist4(intlist3);
    std::cout << intlist1 << std::endl;
    std::cout << intlist2 << std::endl;
    std::cout << intlist3 << std::endl;
    std::cout << intlist4 << std::endl;
    intlist4 = intlist2;
    std::cout << intlist4 << std::endl;
    intlist4.swap(intlist1);
    std::cout << intlist1 << std::endl;
    std::cout << intlist4 << std::endl;
}
// 与链表存取操作有关的方法的测试
void modifyListTest() {
    std::cout << "存取修改函数测试：" << std::endl;
    tstl::list<double> ilist(2, 1.5);
    std::cout << ilist << std::endl;

    ilist.push_back(2.5);
    ilist.push_back(5.1);
    ilist.push_front(3.5);
    ilist.push_front(4.1);
    std::cout << ilist << std::endl;

    ilist.pop_back();
    ilist.pop_fornt();
    std::cout << ilist << std::endl;

    if(ilist.empty())
        std::cout << "List is empty" << '\n';
    else
        std::cout << "List is not empty" << '\n';
    
    tstl::list<double> ilist2;
    ilist2.assign(ilist.begin(),ilist.end());
    ilist.assign(6, 2.2);
    std::cout << ilist2 << std::endl;
    std::cout << ilist << std::endl;
    ilist.resize(8);
    std::cout << ilist << std::endl;
    tstl::list<double>::iterator tmp = ilist.begin();
    tstl::advance(tmp, 3);
    ilist.insert(tmp, 4.6);
    std::cout << ilist << std::endl;

    ilist.erase(--tmp);
    std::cout << ilist << std::endl;
    tmp = ilist.begin();
    tstl::advance(tmp, 2);
    // ilist.splice(tmp, ilist2);
    // ilist.splice(tmp, ilist2, --ilist2.end());
    ilist.splice(tmp, ilist2, ++ilist2.begin(), --ilist2.end());
    std::cout << ilist << std::endl;
    
    // ilist.unique();
    // std::cout << ilist);
    // std::cout << "size = " << ilist.size() << '\n';
    // ilist.erase(ilist.begin(),ilist.end());
    // std::cout << ilist);
    // std::cout << "size = " << ilist.size() << '\n';
    // ilist.clear();
    // std::cout << ilist);
    // std::cout << "size = " << ilist.size() << '\n';
}

// list 元素移动类函数测试
void move_element_Test() {
    std::cout << "移动分割函数测试：" << std::endl;
    tstl::list<int> list1(2, 3);
    tstl::list<int> list2(2, 2);
    std::cout << list1 << std::endl;
    list2.push_back(3);
    list2.push_back(4);
    std::cout << list2 << std::endl;
    list1.merge(list2);
    std::cout << list1 << std::endl;

    list1.remove(2);
    std::cout << list1 << std::endl;
}
// 针对 list.sort（）的测试
void sortTest() {
    std::cout << "随机生成长度为 10 的链表排序" << std::endl;
    tstl::list<int> list;
    for(int i = 0 ; i < 10; i ++)
        list.push_back(rand() % 10 + 1);

    std::cout << "初始 ： "<< std::endl;
    std::cout << list << std::endl;
    list.sort();
    std::cout << "从小到大（默认） ： "<< std::endl;
    std::cout << list << std::endl;
    std::cout << "从大到小（cmp函数调用） ： "<< std::endl;
    list.sort([&](int a, int b){
        return a > b;
    });
    std::cout << list << std::endl;
}

int main() {
    constructedTest();
    modifyListTest();
    move_element_Test();
    sortTest();
    return 0;
}