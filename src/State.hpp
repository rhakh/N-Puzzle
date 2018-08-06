#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <iostream>
#include <vector>
#include "NPuzzleSolver.hpp"

enum moves_e {ROOT, UP, DOWN, LEFT, RIGHT, LAST};

enum solutionType {SNAIL_SOLUTION, NORMAL_SOLUTION};

class State
{
private:
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
	State(const State &rhs) {};
	State	&operator=(const State &rhs) {return (*this);};

	void	makeSnailState();
	void	makeNormalState();

public:
	State(const int *map, int price, int length);
	State(const int solutionType); //build finish state
	State(const State &src, const int move);
	~State() {};

	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	int				getCost() const { return (this->cost); }
	const int		*getMapPtr() const { return (this->map.data()); }
	int				getMapSize() const { return (this->mapLength; }
	void			swapPieces(int a, int b) { std::swap(map[a], map[b]); };
	int				getMove() const { return (this->movement); };
	const State		*getPrev() const { return (this->prev); };
	void			printState() const;

	class	NP_InvalidMove : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Invalid move, can't create state");};
	};
	friend	NPuzzleSolver;
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
