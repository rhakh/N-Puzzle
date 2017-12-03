#ifndef N_PUZZLE_STATE_H
#define N_PUZZLE_STATE_H

#include <vector>
#include <set>
#include <cstddef>
#include <cmath>
#include <iostream>

typedef enum {UP, DOWN, RIGHT, LEFT, LAST} moves_e;

class State {
public:
	int	cost;
	int	length;
	std::vector<uint8_t>	map;
	State   *prev;

public:
	State();

	State(std::vector<uint8_t>&& _map, int _const, int _length);

	//virtual	~State() {};

	void	setCost(int _cost);

	void	setLength(int _length);

	void	incLength();

	int		getCost() const ;

	int		getLength() const ;

	void 	printState() const ;

	void 	setMapVal(int pos, uint8_t val);

	uint8_t 	getMapVal(int pos) const ;

	const std::vector<uint8_t>&	getMap() const ;

	bool	operator<(const State &b) const ;

	bool	operator==(const State &b) const ;
};

/* comparator for set */
struct CompareState {
	bool operator()(const State &a, const State &b) {
		if (a.cost == b.cost)
			return a.length > b.length;
		else
			return a.cost > b.cost;
	}
};

struct HashState {
	size_t operator()(const State& a) const {
		const std::vector<uint8_t>& map = a.getMap();
		std::size_t seed = map.size();
		for(auto& i : map) {
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

#endif //N_PUZZLE_STATE_H