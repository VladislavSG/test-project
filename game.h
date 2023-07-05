#pragma once

#include <array>
#include <vector>
#include <ostream>
#include <string>
#include "cylinder.h"
#include "line.h"
#include "game_properties.h"

class game {
    std::vector<line> lines;
    std::array<std::array<int, WILD>, 3> win_table;

    void init_lines(std::string const&);
    void init_cylinders(std::string const&);
    void init_win_table(std::string const&);

    constexpr int min_win_combination_size() const;
    int win_by_combination(int size, symbol s) const;

    friend class game_state_iterator;

public:
    std::array<cylinder, ROLLERS> cylinders;

    game(std::string const&, std::string const&, std::string const&);

    void random_lines();
    void save_lines();

    int bet() const;

    class roll_result {
        game const& g;
        symbol line_n(line const&, int);

    public:
        std::array<std::array<symbol, WINDOW_SIZE>, ROLLERS> data;

        roll_result(game const&);

        int win();
        double return_percent();

        friend std::ostream& operator<<(std::ostream&, roll_result const&);
    };

    roll_result roll();

    game_state_iterator begin() const;
    game_state_iterator end() const;
};

class game_state_iterator {
    std::array<int, ROLLERS> positions;
    game const& g;

    friend class game;

public:
    game_state_iterator(game const& g);

    game::roll_result operator*();
    game_state_iterator& operator++();
    friend bool operator==(game_state_iterator const&, game_state_iterator const&);
    friend bool operator!=(game_state_iterator const&, game_state_iterator const&);
};
