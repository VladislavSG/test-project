#pragma once

#include "game_properties.h"
#include <istream>
#include <array>

struct line {
    std::array<int, ROLLERS> positions;

    friend std::istream& operator>>(std::istream &in, line &l);
};
