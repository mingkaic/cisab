
#ifndef CISAB_FMTS_DEPRECATED_HPP
#define CISAB_FMTS_DEPRECATED_HPP

#include "cisab/types/iterator.h"

#include "cisab/fmts/readable.h"

namespace fmts
{

template <typename IT>
using ContainerT = std::vector<types::IterValT<IT>>;

template <typename IT>
[[deprecated("Use Humanize instead")]]
void to_stream (std::ostream& os, IT b, IT e)
{
	os << Humanize<ContainerT<IT>>(ContainerT<IT>(b, e));
}

template <typename T>
[[deprecated("Use Humanize instead")]]
void to_stream (std::ostream& os, const T& e)
{
	os << Humanize<T>(e);
}

template <typename IT>
[[deprecated("Use readable instead")]]
std::string to_string (IT b, IT e)
{
	return Humanize<ContainerT<IT>>(ContainerT<IT>(b, e));
}

template <typename T>
[[deprecated("Use readable instead")]]
std::string to_string (const T& e)
{
	return Humanize<T>(e);
}

}

#endif // CISAB_FMTS_DEPRECATED_HPP
