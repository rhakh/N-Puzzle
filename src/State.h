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
	std::vector<int>	map;
	State   *prev;

public:
	State();

	State(std::vector<int> _map, int _const, int _length);

	virtual	~State() {};

	void	setCost(int _cost);

	void	setLength(int _length);

	void	incLength();

	int		getCost() const ;

	int		getLength() const ;

	void 	printState() const ;

	void 	setMapVal(int pos, int val);

	int 	getMapVal(int pos) const ;

	std::vector<int>	getMap() const ;

	std::vector<int>	*getptrMap();

	bool	operator<(const State &b) const ;

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


#endif //N_PUZZLE_STATE_H
