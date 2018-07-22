#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <iostream>

enum moves_e {ROOT, UP, DOWN, LEFT, RIGHT, LAST};

enum solutionType {SNAIL_SOLUTION, NORMAL_SOLUTION};

class State
{
public:
	static int	size;
	static int	mapSize;

private:
	uint8_t	*map = nullptr;
	int		price;
	int		length;
	uint8_t	movement;
	const State	*prev;

	//disable copy constructor;
	State(const State &rhs) {};
	State	&operator=(const State &rhs) {return (*this);};

	void	makeSnailState();
	void	makeNormalState();

public:
	State(const uint8_t *map, int price, int length);
	State(int solutionType); //build finish state
	State(const State &src, int move);
	~State();

	void			setLength(int length) { this->length = length; }
	void			setPrice(int price) { this->price = price; }
	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	const uint8_t	*getMapPtr() const { return (this->map); }
	void			printState() const;
	void			swapPieces(int a, int b);
	uint8_t			getMove() const;
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
