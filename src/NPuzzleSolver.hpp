#ifndef NPUZZLE_SOLVER_HPP
#define NPUZZLE_SOLVER_HPP

#include <exception>
#include <list>
#include "State.hpp"

class NPuzzleSolver
{
	std::tuple<size_t, size_t, size_t>	aStar(const int *map,
												std::list<int> &result);

	void	checkPath(const State &root, const std::list<int> &result) const;
	void 	createPath(std::list<int> &result, const State *curr) const;
	bool	isSolvable(const int *map, int mapSize, int solutionType);

public:
	NPuzzleSolver();
	~NPuzzleSolver() {};
	std::tuple<size_t, size_t, size_t>	solve(int heuristic,
												int solutionType,
												const int *map,
												const int mapSize,
												std::list<int> &result);

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
