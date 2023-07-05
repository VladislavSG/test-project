#include "line.h"
#include <string>
#include "parse_error.h"
#include <ranges>
#include <algorithm>

std::istream &operator>>(std::istream &in, line &l)
{
    using namespace std;
    array<array<bool, WINDOW_SIZE>, ROLLERS> data{};
    for (int i = 0; i < WINDOW_SIZE; ++i) {
        string str_line;
        while (str_line.empty()) {
            getline(in, str_line);
        }
        if (str_line.size() != ROLLERS) {
            throw parse_error("invalid count of rollers in line");
        }
        for (int j = 0; j < ROLLERS; ++j) {
            char const& c = str_line[j];
            if (c == 'o' || c == 'x') {
                data[j][i] = c == 'x';
            } else {
                throw parse_error("invalid character in line definition");
            }
        }
    }

    for (int i = 0; i < ROLLERS; ++i) {
        const auto& window = data[i];
        auto it = find(window.begin(), window.end(), true);
        if (it == window.end()) {
            throw parse_error("No 'x' in one window");
        }
        if (count(it + 1, window.end(), true) > 0) {
            throw parse_error("multiple 'x' in one window");
        }
        l.positions[i] = static_cast<int>(it - window.begin());
    }

    return in;
}
