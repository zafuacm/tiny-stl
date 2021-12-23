#ifndef TSTL_SRC_TYPE_TRAITS_HPP
#define TSTL_SRC_TYPE_TRAITS_HPP

#include <cstddef>

namespace tstl {

template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    operator value_type() const {
        return value;
    }
    value_type operator()() const {
        return value;
    }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <typename...>
using _void_t = void;

} // namespace tstl

#endif
