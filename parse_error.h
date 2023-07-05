#pragma once

#include <stdexcept>

class parse_error : std::runtime_error {
    using base = std::runtime_error;
    using base::base;
};
