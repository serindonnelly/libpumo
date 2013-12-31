#pragma once
#include <ostream>
#include "teebuf.h"
class teestream : public std::ostream
{
public:
	// Construct an ostream which tees output to the supplied
	// ostreams.
	teestream(std::ostream & o1, std::ostream & o2);
private:
	teebuf tbuf;
};