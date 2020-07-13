#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include "Heuristic.hpp"

enum moves { ROOT, UP, DOWN, LEFT, RIGHT, LAST };

enum solutionTypes { SNAIL_SOLUTION, NORMAL_SOLUTION };

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
	static State	*finishState;
	static int		(*heuristicFunc)(const State *state);
	static int		mapSize, mapLength;

	int		cost;	// price + length
	int		price;	// value of heuristic func
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
	friend class	NPuzzleSolver;
	friend class	Heuristic;
	friend class	NP_retVal;
};

struct HashState {
	size_t operator()(const std::unique_ptr<State> &a) const;
};

struct CompareState {
	bool operator()(const std::unique_ptr<State> &a, const std::unique_ptr<State> &b);
};

struct EqualState {
	bool operator()(const std::unique_ptr<State> &lhs, const std::unique_ptr<State> &rhs) const;
};

#endif /* STATE_HPP */
