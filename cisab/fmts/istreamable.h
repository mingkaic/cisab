///
/// istreamable.hpp
/// fmts
///
/// Purpose:
/// Implement iStreamable convenience abstract class taking strings from stream
///

#ifndef CISAB_FMTS_ISTREAMABLE_H
#define CISAB_FMTS_ISTREAMABLE_H

#include <string>
#include <sstream>

#include "cisab/fmts/ireadable.h"

namespace cisab
{

namespace fmts
{

// iStreamable defines objects that can be read into a stream. Its string representation is the
// stream output.
struct iStreamable : public iReadable
{
	virtual ~iStreamable (void) = default;

	std::string to_string () const override
	{
		std::stringstream ss;
		this->read(ss);
		return ss.str();
	}
};

}

}

#endif // CISAB_FMTS_ISTREAMABLE_H
