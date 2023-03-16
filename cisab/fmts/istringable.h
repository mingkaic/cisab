///
/// istringable.hpp
/// fmts
///
/// Purpose:
/// Implement iStringable convenience abstract class writing stream from string
///

#ifndef CISAB_FMTS_ISTRINGABLE_H
#define CISAB_FMTS_ISTRINGABLE_H

#include <ostream>

#include "cisab/fmts/ireadable.h"

namespace cisab
{

namespace fmts
{

// iStringable defines objects that can be represented as a string. Its string representation can be
// read into a stream.
struct iStringable : public iReadable
{
	virtual ~iStringable (void) = default;

	void read (std::ostream& out) const override
	{
		out << this->to_string();
	}
};

}

}

#endif // CISAB_FMTS_ISTRINGABLE_H
