#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <iostream>
#include <vector>

enum moves_e {ROOT, UP, DOWN, LEFT, RIGHT, LAST};

enum solutionType {SNAIL_SOLUTION, NORMAL_SOLUTION};

class State
{
private:
	int		cost;
	int		price;
	int		length;
	int		movement;
	std::vector<int>	map;
	const State			*prev;

	//disable copy constructor;
	State(const State &rhs) {};
	State	&operator=(const State &rhs) {return (*this);};

	void	makeSnailState(const int mapSize);
	void	makeNormalState(const int mapSize);

public:
	State(const int *map, int price, int length, const int mapSize);
	State(const int solutionType, const int mapSize); //build finish state
	State(const State &src, const int move, const int mapSize,
			const int size, const int *finishMap,
			int (*heuristicFunc)(const int *, const int *, const int, const int));
	~State() {};

	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	int				getCost() const { return (this->cost); }
	const int		*getMapPtr() const { return (this->map.data()); }
	int				getMapSize() const { return (this->map.size()); }
	void			swapPieces(int a, int b) { std::swap(map[a], map[b]); };
	int				getMove() const { return (this->movement); };
	const State		*getPrev() const { return (this->prev); };
	void			printState(const int size) const;

	class	NP_InvalidMove : public std::exception {
	public:
		virtual const char	*what() const throw() {return ("Invalid move, can't create state");};
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
