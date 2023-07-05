#pragma once

#include <array>
#include <vector>
#include <istream>
#include <ostream>
#include "game_properties.h"

class cylinder
{
	std::vector<symbol> lenta;

	bool is_valid() const;

public:
	static cylinder random();

	std::array<symbol, WINDOW_SIZE> window(int) const;
	std::array<symbol, WINDOW_SIZE> random_window() const;

	size_t size() const noexcept;

	friend std::istream& operator>>(std::istream&, cylinder&);
	friend std::ostream& operator<<(std::ostream&, cylinder const&);
};

