#ifndef NPUZZLE_SOLVER_HPP
#define NPUZZLE_SOLVER_HPP

#include "State.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <atomic>
#include <list>

typedef struct	path_s path_t;

typedef enum {
	HAMMILTON_DISTANCE, //hammingDistance
	MANHATTAN_DISTANCE,
}		heuristicFunc_e;

typedef enum {
	ASTAR,
	ASTAR_IDA,
}		solver_e;

class NPuzzleSolver
{
	std::atomic_bool	stopRequested;
	int		(*heuristicFunc)(const uint8_t *map, uint8_t mapSize);
	path_t	*(NPuzzleSolver::*algoFunc)(const uint8_t *map, uint8_t mapSize);

	// algo functions
	path_t	*aStar(const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);
	path_t	*aStarIDA(const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);

	State	*doMove(const State *curr, uint8_t move, int emptyPos);
	State	*getNewState(const State *curr, int emptyPos, int newPos, uint8_t move);
public:
	path_t	*solve(int func, int algo, const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);

	NPuzzleSolver();
	~NPuzzleSolver();

	// to stop solving puzzle
	void 	stopProcess(void);
};

#endif /* NPUZZLE_SOLVER_HPP */
