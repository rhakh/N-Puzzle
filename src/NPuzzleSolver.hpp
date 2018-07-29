#ifndef NPUZZLE_SOLVER_HPP
#define NPUZZLE_SOLVER_HPP

#include "State.hpp"

#include <list>
#include <exception>

typedef enum {
	MISPLACED_TILES,
	MANHATTAN_DISTANCE,
	MISPLACED_TILES_PLUS_LINEAR_CONFLICTS,
	MANHATTAN_DISTANCE_PLUS_LINEAR_CONFLICTS,
	N_MAXSWAP
}		heuristicFunc_e;

class NPuzzleSolver
{
	State				*finishState;

	int		(*heuristicFunc)(const int *map, const int *finishMap, const int mapSize, const int size);

	// algo functions
	std::tuple<size_t, size_t, size_t>
		aStar(const int *map, const int mapSize, int solutionType, std::list<int> &result);

	void 	createPath(std::list<int> &result, const State *curr) const;

	// use to check path if it's correct
	void	checkPath(const State *root, const std::list<int> &result, const int mapSize, const int size) const;
	bool	isSolvable(const int *map, int mapSize, int solutionType);

public:
	std::tuple<size_t, size_t, size_t>
	solve(int heuristic, int solutionType,
			const int *map, const int mapSize, std::list<int> &result);

	NPuzzleSolver();
	~NPuzzleSolver();

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

	class	NP_InvalidHeuristic : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Invalid heuristic");};
	};
};

#endif /* NPUZZLE_SOLVER_HPP */
