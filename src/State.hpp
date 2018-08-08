#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <iostream>
#include <vector>
// #include "NPuzzleSolver.hpp"
#include "heuristicFunctions.hpp"

enum moves {ROOT, UP, DOWN, LEFT, RIGHT, LAST};

enum solutionTypes {SNAIL_SOLUTION, NORMAL_SOLUTION};

enum heuristicFuncs {
	MISPLACED_TILES,
	MANHATTAN_DISTANCE,
	MISPLACED_TILES_PLUS_LINEAR_CONFLICTS,
	MANHATTAN_DISTANCE_PLUS_LINEAR_CONFLICTS,
	N_MAXSWAP
};

class State
{
private:
	friend class NPuzzleSolver;
	friend class Heuristic;
	// static int	linearConflicts(const State *state);
	// static int	misplacedTiles(const State *state);
	// static int	manhattanDistance(const State *state);
	// static int	MDplusLinearConflicts(const State *state);
	// static int	MTplusLinearConflicts(const State *state);
	// static int	nMaxSwap(const State *state);

	static State	*finishState;
	static int		(*heuristicFunc)(const State *state);
	static int		mapSize, mapLength;

	int		cost;
	int		price;
	int		length;
	int		movement;
	std::vector<int>	map;
	const State			*prev;

	//disable copy constructor;
	State(const State &rhs) {(void)rhs;};
	State	&operator=(const State &rhs) {(void)rhs; return (*this);};

	void	makeSnailState();
	void	makeNormalState();

public:
	State(const int *map);
	State(const int solutionType); //build finish state
	State(const State &src, const int move);
	~State() {};

	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	int				getCost() const { return (this->cost); }
	const int		*getMapPtr() const { return (this->map.data()); }
	int				getMapSize() const { return (this->mapSize); }
	int				getMapLength() const { return (this->mapLength); }
	void			swapPieces(int a, int b) { std::swap(map[a], map[b]); };
	int				getMove() const { return (this->movement); };
	const State		*getPrev() const { return (this->prev); };
	void			printState() const;

	class	NP_InvalidMove : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Invalid move, can't create state");};
	};

	class	NP_StaticVarsUnset : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Static variables for class State wasn't setted");};
	};
};

struct HashState {
	size_t operator()(const State* a) const;
};

struct CompareState {
	bool operator()(const State *a, const State *b);
};

struct EqualState {
	bool operator()(const State *lhs, const State *rhs) const;
};

#endif /* STATE_HPP */
