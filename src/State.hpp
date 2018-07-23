#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <iostream>
#include <vector>

enum moves_e {ROOT, UP, DOWN, LEFT, RIGHT, LAST};

enum solutionType {SNAIL_SOLUTION, NORMAL_SOLUTION};

class State
{
public:
	static int	size;
	static int	mapSize;

private:
	// int	*map = nullptr;
	std::vector<int> map;
	int		price;
	int		length;
	int		movement;
	const State	*prev;

	//disable copy constructor;
	State(const State &rhs) {};
	State	&operator=(const State &rhs) {return (*this);};

	void	makeSnailState(const int mapSize);
	void	makeNormalState(const int mapSize);

public:
	State(const int *map, int price, int length, const int mapSize);
	State(const int solutionType, const int mapSize); //build finish state
	State(const State &src, const int move, const int size);
	~State();

	void			setLength(int length) { this->length = length; }
	void			setPrice(int price) { this->price = price; }
	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	const int		*getMapPtr() const { return (this->map.data()); }
	int				getMapSize() const { return (this->map.size()); }
	void			printState(const int size) const;
	void			swapPieces(int a, int b);
	int				getMove() const;
	const State		*getPrev() const;

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
