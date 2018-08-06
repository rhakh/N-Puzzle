#ifndef N_PUZZLE_HEURISTICFUNCTIONS_HPP
#define N_PUZZLE_HEURISTICFUNCTIONS_HPP

#include "State.hpp"

int	misplacedTiles(const State *state);
int	manhattanDistance(const State *state);
int	MDplusLinearConflicts(const State *state);
int	MTplusLinearConflicts(const State *state);
int	nMaxSwap(const State *state);

#endif //N_PUZZLE_HEURISTICFUNCTIONS_HPP
