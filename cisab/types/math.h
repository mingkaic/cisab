
#ifndef CISAB_TYPES_MATH_H
#define CISAB_TYPES_MATH_H

#include <complex>
#include <type_traits>

namespace cisab
{

namespace types
{

template<typename T>
struct is_complex : public std::false_type {};

template<typename T>
struct is_complex<std::complex<T>> : public std::true_type {};

}

}

#endif // CISAB_TYPES_MATH_H
