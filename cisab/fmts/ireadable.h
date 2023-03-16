///
/// ireadable.hpp
/// fmts
///
/// Purpose:
/// Define readable interface and override streaming and string conversion
///

#ifndef CISAB_FMTS_IREADABLE_H
#define CISAB_FMTS_IREADABLE_H

#include <string>
#include <ostream>
#include <memory>

namespace cisab
{

namespace fmts
{

// iReadable interface defines objects that can be read into a stream or represented as a string.
struct iReadable
{
	virtual ~iReadable (void) = default;

	// Read the string representation of this object to outstream
	virtual void read (std::ostream& out) const = 0;

	// Return the string representation of this object
	virtual std::string to_string (void) const = 0;

	// convert to string
	operator std::string()
	{
		return this->to_string();
	}
};

std::ostream& operator << (std::ostream& os, const iReadable& reader);

std::ostream& operator << (std::ostream& os, const iReadable* reader);

std::ostream& operator << (std::ostream& os, std::shared_ptr<iReadable> reader);

}

}

#if __cplusplus > 201703L

#include <format>

namespace cisab
{

namespace fmts
{

template <>
struct std::formatter<iReadable, std::string>
{
	template <typename FormatContext>
	auto format (const iReadable& readable, FormatContext& fc)
	{
		return readable.to_string();
	}
};

}

}

#endif

#endif // CISAB_FMTS_IREADABLE_H
