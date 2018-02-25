#include "NPuzzleSolver.h"

int		main(void) {
//	uint8_t	map[16] = {
//		11,	0,	9,	4,
//		2,	15,	7,	1,
//		13,	3,	12,	5,
//		8,	6,	10,	14
//	};

	uint8_t map[9] = {
			0, 3, 5,
			6, 7, 1,
			4, 2, 8
	};

	State::size = 3;
	State::dsize = State::size * State::size;
	State	*a = new State(map, 0, 0);
	NPuzzleSolver	solver(HAMILTON_DISTANCE);

	solver.solve(a, ASTAR);

	//delete a;
}