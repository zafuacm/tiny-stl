#ifndef TSTL_SRC_ALGORITHM_HPP
#define TSTL_SRC_ALGORITHM_HPP

#include "iterator.hpp"
#include <memory>

namespace tstl {

template <class T>
void swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <class ForwardIt1, class ForwardIt2>
void iter_swap(ForwardIt1 a, ForwardIt2 b) {
    swap(*a, *b);
}

template <class BidirIt>
void reverse(BidirIt first, BidirIt last, bidirectional_iterator_tag) {
    while ((first != last) && (first != --last)) {
        iter_swap(first++, last);
    }
}

template <typename RandomAccessIter>
void reverse(RandomAccessIter first, RandomAccessIter last, random_access_iterator_tag) {
    while (first < last) {
        tstl::iter_swap(first++, --last);
    }
}

template <class BidirIter>
void reverse(BidirIter first, BidirIter last) {
    reverse(first, last, tstl::_iterator_category(first));
}

template <class BidirIt1, class BidirIt2>
BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (first != last) {
        *(--d_last) = std::move(*(--last));
    }
    return d_last;
}

template <class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}

template <class InputIt, class OutputIt, class UnaryPredicate>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred) {
    while (first != last) {
        if (pred(*first))
            *d_first++ = *first;
        ++first;
    }
    return d_first;
}

template <class InputIt, class OutputIt>
OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
    while (first != last) {
        *d_first++ = std::move(*first++);
    }
    return d_first;
}

template <class T>
const T &max(const T &a, const T &b) {
    return (a < b) ? b : a;
}

template <class T, class Compare>
const T &max(const T &a, const T &b, Compare comp) {
    return (comp(a, b)) ? b : a;
}

template <class ForwardIt>
ForwardIt max_element(ForwardIt first, ForwardIt last) {
    if (first == last) {
        return last;
    }
    ForwardIt largest = first;
    ++first;
    for (; first != last; ++first) {
        if (*largest < *first) {
            largest = first;
        }
    }
    return largest;
}

template <class ForwardIt, class Compare>
ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) {
        return last;
    }
    ForwardIt largest = first;
    ++first;
    for (; first != last; ++first) {
        if (comp(*largest, *first)) {
            largest = first;
        }
    }
    return largest;
}

template <class T>
T max(std::initializer_list<T> ilist) {
    return *tstl::max_element(ilist.begin(), ilist.end());
}

template <class T, class Compare>
T max(std::initializer_list<T> ilist, Compare comp) {
    return *tstl::max_element(ilist.begin(), ilist.end(), comp);
}

template <class T>
const T &min(const T &a, const T &b) {
    return (b < a) ? b : a;
}

template <class T, class Compare>
const T &min(const T &a, const T &b, Compare comp) {
    return (comp(b, a)) ? b : a;
}

template <class ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last) {
    if (first == last) {
        return last;
    }
    ForwardIt smallest = first;
    ++first;
    for (; first != last; ++first) {
        if (*first < *smallest) {
            smallest = first;
        }
    }
    return smallest;
}

template <class ForwardIt, class Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) {
        return last;
    }
    ForwardIt smallest = first;
    ++first;
    for (; first != last; ++first) {
        if (comp(*first, *smallest)) {
            smallest = first;
        }
    }
    return smallest;
}

template <class T>
T min(std::initializer_list<T> ilist) {
    return *tstl::min_element(ilist.begin(), ilist.end());
}

template <class T, class Compare>
T min(std::initializer_list<T> ilist, Compare comp) {
    return *tstl::min_element(ilist.begin(), ilist.end(), comp);
}

template <class ForwardIt, class T>
void fill(ForwardIt first, ForwardIt last, const T &value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

template <class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count, const T &value) {
    for (Size i = 0; i < count; ++i) {
        *first++ = value;
    }
    return first;
}

} // namespace tstl

#endif
