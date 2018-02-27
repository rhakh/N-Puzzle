#ifndef NPUZZLE_SOLVER_H
#define NPUZZLE_SOLVER_H

#include "State.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>
#include <cmath>

typedef struct	path_s path_t;

typedef struct	path_s {
	State		*state;
	path_t		*next;
	path_s() : state(NULL), next(NULL) {}
}				path_t;

typedef enum {
	HAMMILTON_DISTANCE,
	MANHATTAN_DISTANCE,
}		euristicFunc_e;

typedef enum {
	ASTAR,
	ASTAR_IDA,
}		solver_e;

class NPuzzleSolver
{
	int		(*euristicFunc)(const uint8_t *map, uint8_t mapSize);
	path_t	*(NPuzzleSolver::*algoFunc)(const uint8_t *map, uint8_t mapSize);

	// algo functions
	path_t	*aStar(const uint8_t *map, uint8_t mapSize);
	path_t	*aStarIDA(const uint8_t *map, uint8_t mapSize);

	State	*doMove(const State *curr, int move, int emptyPos);
public:
	path_t	*solve(const uint8_t *map, uint8_t mapSize);

	NPuzzleSolver(euristicFunc_e func = HAMMILTON_DISTANCE, solver_e algo = ASTAR);
	~NPuzzleSolver() {};

};

#endif /* NPUZZLE_SOLVER_H */
