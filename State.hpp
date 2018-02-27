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
	static int	mapSize;

private:
	uint8_t	*map = nullptr;
	int		price;
	int		length;
	size_t	hash;

public:
	State(const uint8_t *map, int price, int length);
	State();
	~State();

	void			setLength(int length) { this->length = length; }
	void			setPrice(int price) { this->price = price; }
	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	const uint8_t	*getMapPtr() const { return (this->map); }
	size_t 			getHash() const { return this->hash; }
	void			printState() const;
	void			swapPieces(int a, int b) {	std::swap(map[a], map[b]); }

};

struct HashState {
	size_t operator()(const State* a) const {
		return (a->getHash());
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
	bool operator()(State const *rhs, State const *lhs) const {
		const uint8_t *pa;
		const uint8_t *pb;

		pa = rhs->getMapPtr();
		pb = lhs->getMapPtr();
		for (int i = 0; i < State::mapSize; i++) {
			if (pa[i] != pb[i])
				return (false);
		}
		return (true);
	}
};

#endif /* STATE_H */
