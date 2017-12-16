#include "search_algo.h"
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>

std::priority_queue<State *, std::vector<State *>, CompareState>	open;
std::unordered_set<State *, HashState>	closed;
std::vector<moves_e>	path;

int		hammiltonFunction(const std::vector<uint8_t>&  map) {
	int inversions = 0;

	size_t size = map.size();
	for (int i = 0; i < size; i++) {
		if (map[i] != (i + 1))
			inversions++;
	}
	return (inversions);
}

State	*doMove(std::vector<uint8_t> map, moves_e move) {
	int	pos = 0;
	int	x, y, up, down, right, left;

	for (int i = 0; i < map.size(); i++)
		if (map[i] == 0 && (pos = i) >= 0)
			break ;
	x = pos % n_size;
	y = pos / n_size;
	up = x + ((y - 1) * n_size);
	down = x + ((y + 1) * n_size);
	right = (x + 1 == n_size) ? (BIG_VAL) : ((x + 1) + (y * n_size));
	left = (x - 1 < 0) ? (BIG_VAL) : ((x - 1)  + (y * n_size));

	if (move == UP && up >=0 && up < dsize) {
		std::swap(map[pos], map[up]);
	}
	else if (move == DOWN && down >= 0 && down < dsize) {
		std::swap(map[pos], map[down]);

	}
	else if (move == LEFT && left >= 0 && left < dsize) {
		std::swap(map[pos], map[left]);

	}
	else if (move == RIGHT && right >=0 && right < dsize) {
		std::swap(map[pos], map[right]);
	}
	else {
		return (State());
	}
	return (State(std::move(map), hammiltonFunction(map), 0));
}