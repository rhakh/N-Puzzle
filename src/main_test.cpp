#include "NPuzzleSolver.hpp"

int		main(void) {
	uint8_t	map4[16] = {
		11,	0,	9,	4,
		2,	15,	7,	1,
		13,	3,	12,	5,
		8,	6,	10,	14
	};

	uint8_t map3[9] = {
			0, 3, 5,
			6, 7, 1,
			4, 2, 8
	};

	uint8_t map6[36] = {
		1, 14, 2, 4, 6, 18,
		9, 13, 3, 17, 11, 33,
		19, 7, 16, 10, 5, 12,
		8, 26, 20, 15, 22, 24,
		21, 31, 27, 29, 23, 30,
		25, 0, 32, 28, 34, 35};

	uint8_t map5[25] = {
			19, 0, 12, 18, 17,
			16, 23, 21, 13, 7,
			5, 24, 10, 20, 1,
			14, 6, 8, 3, 22,
			15, 9, 2, 4, 1
	};

	uint8_t map5a[25] = {
			13, 14, 1, 15, 8,
			20, 11, 0, 22, 23,
			6, 3, 12, 2, 5,
			4, 18, 7, 21, 9,
			10, 19, 16, 24, 17
	};

	NPuzzleSolver	solver;
	std::list<uint8_t> res;

	solver.solve(MISPLACED_TILES, ASTAR, map3, 9, res);

	while (1);
	return (0);
}

/*
 * How to profile N-Puzzle
 * $ g++ -std=c++11 -O2 -pg ...
 * $ ./a.out
 * $ gprof a.out > prof.txt
 * g++ -std=c++11 -I $HOME/.brew/Cellar/boost/1.67.0_1/include -L $HOME/.brew/Cellar/boost/1.67.0_1/lib  -lboost_system  -lboost_thread-mt
 */
