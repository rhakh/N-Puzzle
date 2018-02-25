#ifndef NPUZZLE_SOLVER_H
#define NPUZZLE_SOLVER_H

#include "State.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>

typedef struct	path_s path_t;

typedef struct	path_s {
	State		*state;
	path_t		*next;
	path_s() : state(NULL), next(NULL) {}
}				path_t;	

typedef enum {
	HAMILTON_DISTANCE,
	manhattanDistance,
}		euristicFunc_e;

typedef enum {
	ASTAR,
	ASTAR_IDA,
}		solver_e;

class NPuzzleSolver
{
	int		(*euristicFunc)(const uint8_t *map, uint8_t dsize);

	path_t	*aStar(State *root);
	path_t	*aStarIDA(State *root);
	State * doMove(const State *curr, int move, int emptyPos);
public:
	path_t	*solve(State *root, solver_e algo = ASTAR);
	NPuzzleSolver(euristicFunc_e func = HAMILTON_DISTANCE);
	~NPuzzleSolver() {};
	
};

#endif /* NPUZZLE_SOLVER_H */