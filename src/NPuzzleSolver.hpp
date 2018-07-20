#ifndef NPUZZLE_SOLVER_HPP
#define NPUZZLE_SOLVER_HPP

#include "State.hpp"

#include <list>

typedef enum {
	MISPLACED_TILES = 0x01,
	MANHATTAN_DISTANCE = 0x02,
	LINEAR_CONFLICTS = 0x04
}		heuristicFunc_e;

typedef enum {
	ASTAR,
	ASTAR_IDA,
}		solver_e;

class NPuzzleSolver
{
	State				*finishState;

	int		(*conflictsFunc)(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);
	int		(*heuristicFunc)(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

	// algo functions
	std::tuple<size_t, size_t, size_t>
		aStar(const uint8_t *map, uint8_t mapSize, int solutionType, std::list<uint8_t> &result);
	std::tuple<size_t, size_t, size_t>
		aStarIDA(const uint8_t *map, uint8_t mapSize, int solutionType, std::list<uint8_t> &result);

	void 	createPath(std::list<uint8_t> &result, const State *curr) const;
	State	*doMove(const State *curr, uint8_t move);
	State	*getNewState(const State *curr, int emptyPos, int newPos, uint8_t move);

public:
	std::tuple<size_t, size_t, size_t>
	solve(int func, int algo, int solutionType,
			const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);

	NPuzzleSolver();
	~NPuzzleSolver();

	// to stop solving puzzle
	void 	stopProcess(void);

	class	NP_MapisNullException : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Map is NULL");};
	};

	class	NP_InvalidMapSize : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Invalid map size");};
	};

	class	NP_InvalidMap : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Map is unsolvable or not squared");};
	};
};

#endif /* NPUZZLE_SOLVER_HPP */
