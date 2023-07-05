#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>
#include <numeric>
#include "game.h"

template<typename It>
double mean(It begin, It end) {
    long long sum = accumulate(begin, end, 0ll);
    return double(sum) / (end - begin);
}

template<typename It>
double hit(It begin, It end) {
    int win_count = count_if(begin, end, [](int w) {return w > 0; });
    return (double)win_count / (end - begin);
}

template<typename It>
double std_dev(It begin, It end) {
    double m = mean(begin, end);
    double sum = 0;
    for (auto it = begin; it != end; ++it) {
        double d = *it - m;
        sum += d * d;
    }
    sum /= (end - begin);
    return sqrt(sum);
}

struct game_stat {
    double payout;
    double hit;
    double stddev;

    double metric() {
        double dp = payout - 0.92;
        double dh = hit - 0.2;
        return 10 * dp * dp + dh * dh + stddev / 400;
    }

    friend std::ostream& operator<<(std::ostream& out, game_stat const& stat) {
        return out << "Payout = " << 100 * stat.payout << "%\n"
                   << "Hit = "    << 100 * stat.hit    << "%\n"
                   << "StdDev = " << 100 * stat.stddev << "%\n";
    }
};

game_stat full_game_stat(game const& g) {
    using namespace std;
    vector<game::roll_result> all_states;
    for (auto it = g.begin(); it != g.end(); ++it) {
        all_states.push_back(*it);
    }

    vector<int> wins(all_states.size());
    for (int i = 0; i < all_states.size(); ++i) {
        wins[i] = all_states[i].win() / g.bet();
    }

    return {
        mean(wins.begin(), wins.end()),
        hit(wins.begin(), wins.end()),
        std_dev(wins.begin(), wins.end())
    };
}

void find_cylinders() {
    game g("input_files/lines.txt"
        , "input_files/lenta.txt"
        , "input_files/win_table.txt");
    g.random_lines();
    auto cyl = g.cylinders;
    game_stat stat = full_game_stat(g);
    for (int i = 0; i < 500; ++i)
    {
        if (i % 100 == 0) {
            std::cout << stat << std::endl;
        }
        g.random_lines();
        game_stat stat2 = full_game_stat(g);
        if (stat2.metric() < stat.metric()) {
            stat = stat2;
            cyl = g.cylinders;
        }
        else
        {
            g.cylinders = cyl;
        }
    }
    g.save_lines();
}

void random_stat(game &g) {
    using namespace std;
    vector<game::roll_result> states;
    for (int i = 0; i < 1000000; ++i) {
        states.push_back(g.roll());
    }

    vector<int> wins(states.size());
    for (int i = 0; i < states.size(); ++i) {
        wins[i] = states[i].win() / g.bet();
    }

    auto const begin = wins.begin();
    for (int i : {10000, 50000, 100000, 300000, 1000000}) {
        auto end = begin + i;
        cout << i << ' ' << mean(begin, end) << ' ' << hit(begin, end) << ' ' << std_dev(begin, end) << endl;
    }
}

int main() {
    using namespace std;
    //find_cylinders();

    game g("input_files/lines.txt"
          ,"input_files/lenta.txt"
          ,"input_files/win_table.txt");

    random_stat(g);

    cout << full_game_stat(g);

    return 0;
}