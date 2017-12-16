#include <iostream>
#include <cmath>
#include <queue>
#include <unordered_set>
#include "State.h"

#define BIG_VAL 12414532

enum moves_e {UP, DOWN, LEFT, RIGHT, LAST};

uint8_t	hamiltonDistance(const uint8_t *map, size_t size) {
	uint8_t inversions = -1;

	for (uint8_t i = 0, j = 1; i < size; ++i, ++j) {
		if (map[i] != j)
			inversions++;
	}

	return (inversions);
}

#define swap_ints(x, y) \
		(x) = (x) ^ (y); \
		(y) = (x) ^ (y); \
		(x) = (x) ^ (y); \

 static uint8_t inline *ret_new_map(uint8_t *ret_map, const uint8_t *map, size_t size,
								   int pos, int new_pos) {
	if ((ret_map = new uint8_t[size]) != nullptr) {
		memcpy(ret_map, map, size);
		swap_ints(ret_map[pos], ret_map[new_pos]);
		return (ret_map);
	}
	return (nullptr);
}

uint8_t *doMove(const uint8_t *map, size_t size, moves_e move) {
	int	pos;
	int	x, y, up, down, right, left;
	uint8_t *ret_map = nullptr;

	for (pos = 0; pos < size; ++pos)
		if (map[pos] == 0)
			break ;

	x = pos % State::n_size;
	y = pos / State::n_size;
	up = x + ((y - 1) * State::n_size);
	down = x + ((y + 1) * State::n_size);
	right = (x + 1 == State::n_size) ? (BIG_VAL) : ((x + 1) + (y * State::n_size));
	left = (x - 1 < 0) ? (BIG_VAL) : ((x - 1)  + (y * State::n_size));

	if (move == UP && up >= 0 && up < State::d_size)
		return ret_new_map(ret_map, map, size, pos, up);
	else if (move == DOWN && down >= 0 && down < State::d_size)
		return ret_new_map(ret_map, map, size, pos, down);
	else if (move == LEFT && left >= 0 && left < State::d_size)
		return ret_new_map(ret_map, map, size, pos, left);
	else if (move == RIGHT && right >= 0 && right < State::d_size)
		return ret_new_map(ret_map, map, size, pos, right);
	else
		return (nullptr);
}

std::priority_queue<State *, std::vector<State *>, CompareState>   open;
std::unordered_set<State *, StateHash, StateEq>  closed;

State *aStar(State *start, uint8_t (* heuristicFunc)(const uint8_t *map, size_t size)) {
	State	*curr;
	State	*new_state;
	uint8_t *new_map = nullptr;
	uint8_t	i = 0;
	unsigned long 	max_open = 0;

	int a = 0;
	open.push(start);
	while (!open.empty()) {
		curr = open.top();
		open.pop();

		if (!heuristicFunc(curr->getConstMap(), State::d_size)) {
			curr->printState();
			printf("max_open = %d, closed.size = %d\n", max_open, closed.size());
			return (curr);
		}

		if (closed.find(curr) != closed.end())
			continue;

		closed.insert(curr);

		for (i = UP; i < LAST; ++i) {
			new_map = doMove(curr->getConstMap(), State::d_size, (moves_e)i);
			if (new_map == nullptr)
				continue;
			new_state = new State(heuristicFunc(new_map, State::d_size), curr->getLength() + 1);
			new_state->setMap(new_map);
			open.push(new_state);
		}
		if (max_open < open.size())
			max_open = open.size();
	}
}

int main() {
	uint8_t (*heuristicFunc)(const uint8_t *map, size_t size);
	uint8_t	map4[16] = {11,  0,  9,  4,
								  2, 15,  7,  1,
								  13,  3, 12,  5,
								  8,  6, 10, 14};
	uint8_t map6[36] = {1, 14, 2, 4, 6, 18,
						9, 13, 3, 17, 11, 33,
						19, 7, 16, 10, 5, 12,
						8, 26, 20, 15, 22, 24,
						21, 31, 27, 29, 23, 30,
						25, 0, 32, 28, 34, 35};
	State	*start, *finish;

	heuristicFunc = hamiltonDistance;
	start = new State(heuristicFunc(map6, sizeof(map6)),
								0, map6, sizeof(map6));
	finish = aStar(start, heuristicFunc);

}
