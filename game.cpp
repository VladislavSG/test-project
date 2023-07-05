#include "game.h"
#include <fstream>
#include <ranges>
#include <stdexcept>
#include <iostream>
#include <random>

static std::mt19937 gen(44425);

std::ostream& operator<<(std::ostream &out, game::roll_result const& res)
{
	out << "GAME ROLL RESULT:\n";
	for (int i = 0; i < WINDOW_SIZE; ++i) {
		for (int j = 0; j < ROLLERS; ++j) {
            out << res.data[j][i];
		}
		out << '\n';
	}
	return out;
}

bool operator==(game_state_iterator const& l, game_state_iterator const& r)
{
    return l.positions == r.positions;
}

bool operator!=(game_state_iterator const& l, game_state_iterator const& r)
{
    return !(l == r);
}

void game::init_lines(std::string const& lines_file)
{
    using namespace std;
    ifstream in(lines_file);
    int n;
    in >> n;
    lines.resize(n);
    for (line& l : lines) {
        in >> l;
    }
}

void game::init_cylinders(std::string const& lentas_file)
{
    using namespace std;
    ifstream in(lentas_file);
    for (cylinder& c : cylinders) {
        in >> c;
    }
}

void game::init_win_table(std::string const& win_table_file)
{
    using namespace std;
    ifstream in(win_table_file);
    
    for (auto &win_line : win_table | views::reverse) {
        for (auto &win_value : win_line) {
            in >> win_value;
        }
    }
}

game::game(std::string const& lines_file,
           std::string const& lentas_file,
           std::string const& win_table_file)
{
    init_lines(lines_file);
    init_cylinders(lentas_file);
    init_win_table(win_table_file);
}



void game::random_lines()
{
    std::bernoulli_distribution dist(0.5);
    for (auto& c : cylinders) {
        if (dist(gen)) {
            c = cylinder::random();
        }
    }
}

void game::save_lines()
{
    std::ofstream out("input_files/lenta.txt");
    for (auto const& c : cylinders) {
        out << c;
        std::cout << c;
    }

}

int game::bet() const
{
    return static_cast<int>(lines.size());
}

game::roll_result game::roll()
{
    roll_result res(*this);
	for (int i = 0; i < ROLLERS; ++i) {
		res.data[i] = cylinders[i].random_window();
	}
	return res;
}

game_state_iterator game::begin() const
{
    return {*this};
}

game_state_iterator game::end() const
{
    game_state_iterator res(*this);
    res.positions.back() = static_cast<int>(cylinders.back().size());
    return res;
}

constexpr int game::min_win_combination_size() const
{
    return ROLLERS - static_cast<int>(win_table.size()) + 1;
}

int game::win_by_combination(int size, symbol sym) const
{
    if (size < min_win_combination_size()) {
        return 0;
    }
    if (size > ROLLERS) {
        throw std::invalid_argument("size bigger than rollers count");
    }
    int pos =  size - min_win_combination_size();
    return win_table[pos][sym];
}

game::roll_result::roll_result(game const& g) : g(g) {}

int game::roll_result::win()
{
    int result = 0;
    for (auto const& l : g.lines) {
        int count = 0;
        while (count < ROLLERS && line_n(l, count) == WILD) {
            ++count;
        }
        symbol comb_sym = count < ROLLERS ? line_n(l, count) : S8;
        while (count < ROLLERS 
              && (line_n(l, count) == comb_sym
              || line_n(l, count) == WILD))
        {
            ++count;
        }
        result += g.win_by_combination(count, comb_sym);
    }
    return result;
}

double game::roll_result::return_percent()
{
    return static_cast<double>(win()) / g.bet();
}

symbol game::roll_result::line_n(line const& l, int n)
{
    return data[n][l.positions[n]];
}

game_state_iterator::game_state_iterator(game const& g) : g(g) {
    positions.fill(0);
}

game::roll_result game_state_iterator::operator*()
{
    game::roll_result res(g);
    for (int i = 0; i < ROLLERS; ++i) {
        res.data[i] = g.cylinders[i].window(positions[i]);
    }
    return res;
}

game_state_iterator& game_state_iterator::operator++()
{
    for (int i = 0; i < ROLLERS; ++i) {
        if (++positions[i] < g.cylinders[i].size())
        {
            break;
        }
        else if (i + 1 < ROLLERS)
        {
            positions[i] = 0;
        }
    }

    return *this;
}
