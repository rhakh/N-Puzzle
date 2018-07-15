#ifndef NPUZZLE_SOLVER_HPP
#define NPUZZLE_SOLVER_HPP

#include "State.hpp"

#include <atomic>
#include <list>

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
	void	(NPuzzleSolver::*algoFunc)(const uint8_t *map, uint8_t mapSize);

	// algo functions
	std::tuple<size_t, size_t, size_t>
		aStar(const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);

	std::tuple<size_t, size_t, size_t>
		aStarIDA(const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);

	void 	createPath(std::list<uint8_t> &result, const State *curr) const;
	State	*doMove(const State *curr, uint8_t move, int emptyPos);
	State	*getNewState(const State *curr, int emptyPos, int newPos, uint8_t move);
public:
	std::tuple<size_t, size_t, size_t>
		solve(int func, int algo, const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result);

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
