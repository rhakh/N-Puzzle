#ifndef N_PUZZLE_HEURISTICFUNCTIONS_HPP
#define N_PUZZLE_HEURISTICFUNCTIONS_HPP

class State;

class Heuristic {
	static int	linearConflicts(const State *state);
public:
	static int	misplacedTiles(const State *state);
	static int	manhattanDistance(const State *state);
	static int	MDplusLinearConflicts(const State *state);
	static int	MTplusLinearConflicts(const State *state);
	static int	nMaxSwap(const State *state);
};

#endif //N_PUZZLE_HEURISTICFUNCTIONS_HPP
