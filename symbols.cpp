#include "symbols.h"

std::ostream& operator<<(std::ostream& out, symbol const& sym)
{
    if (sym == WILD) {
        out << 'W';
    }
    else {
        out << sym + 1;
    }
    return out;
}
