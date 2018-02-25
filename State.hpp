#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>


enum moves_e {UP, DOWN, LEFT, RIGHT, LAST};

class State
{
public:
	static int	size;
	static int	dsize;

private:
	uint8_t	*map = nullptr;
	int		price;
	int		length;

public:
	State(const uint8_t *map, int price, int length);
	//State( State const &src);
	State();
	~State();

	void			setLength(int length) { this->length = length; }
	void			setPrice(int price) { this->price = price; }
	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	const uint8_t	*getMapPtr() const { return (this->map); }
	void			printState() const;
	State			&swapPieces(int a, int b) {	std::swap(map[a], map[b]); return (*this); }

//	bool operator==(State const *rhs) const;

};

struct HashState {
	size_t operator()(const State* a) const {
		const uint8_t *map = a->getMapPtr();
		std::size_t seed = State::size;
		for(int i = 0; i < State::size; i++) {
			seed ^= map[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

class CompareState {
public:
	bool operator()(const State *a, const State *b) {
		/* Maybe it is possible to use elvis operator here ? */
		if (a->getPrice() == b->getPrice())
			return a->getLength() > b->getLength();
		else
			return a->getPrice() > b->getPrice();
	}
};

class EqualState {
public:
	bool operator()(const State *rhs, const State *lhs) {
		const uint8_t *pa;
		const uint8_t *pb;

		pa = rhs->getMapPtr();
		pb = lhs->getMapPtr();
		for (int i = 0; i < State::dsize; i++) {
			if (pa[i] != pb[i])
				return (false);
		}
		return (true);
	}
};

#endif /* STATE_H */
