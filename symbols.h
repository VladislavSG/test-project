#pragma once

#include <ostream>

enum symbol {
	S1, S2, S3, S4, S5, S6, S7, S8, WILD
};

std::ostream& operator<<(std::ostream&, symbol const&);
