#include "cylinder.h"
#include <random>
#include <string>
#include <ranges>
#include "parse_error.h"

static std::mt19937 gen{314214};

bool cylinder::is_valid() const
{
    size_t size = lenta.size();
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < i + WINDOW_SIZE; ++j) {
            if (lenta[i] == lenta[j % size]) {
                return false;
            }
        }
    }
    return true;
}

cylinder cylinder::random()
{
    using namespace std;
    uniform_int_distribution<int> rnd(S1, WILD);
    cylinder res;
    res.lenta.resize(12);
    do {
        generate(res.lenta.begin(), res.lenta.end(), [&]() { return static_cast<symbol>(rnd(gen)); });
    } while (!res.is_valid());
    return res;
}

std::array<symbol, WINDOW_SIZE> cylinder::window(int start) const
{
    std::array<symbol, WINDOW_SIZE> res{};
    for (int i = 0; i < res.size(); ++i) {
        int pos = (start + i) % lenta.size();
        res[i] = lenta[pos];
    }
    return res;
}

std::array<symbol, WINDOW_SIZE> cylinder::random_window() const
{
    using namespace std;
    uniform_int_distribution<int> rnd(0, static_cast<int>(lenta.size()));
    int start = rnd(gen);
    return window(start);
}

size_t cylinder::size() const noexcept
{
    return lenta.size();
}

std::istream& operator>>(std::istream &in, cylinder &cyl)
{
    std::string line;
    std::getline(in, line);
    cyl.lenta.resize(line.size());
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == 'W') {
            cyl.lenta[i] = WILD;
        }
        else {
            int s = line[i] - '1';
            if (s < S1 || s >= WILD) {
                throw parse_error("invalid character in lenta");
            }
            cyl.lenta[i] = static_cast<symbol>(s);
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, cylinder const& cyl)
{
    for (symbol sym : cyl.lenta) {
        out << sym;
    }
    return out << std::endl;
}
