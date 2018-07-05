#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include <cstdio>
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

public:
	State(const uint8_t *map, int price, int length);
	~State();

	void			setLength(int length) { this->length = length; }
	void			setPrice(int price) { this->price = price; }
	int				getLength() const { return (this->length); }
	int				getPrice() const { return (this->price); }
	const uint8_t	*getMapPtr() const { return (this->map); }
	void			printState() const;
	void			swapPieces(int a, int b) {	std::swap(map[a], map[b]); }

};

struct HashState {
	size_t operator()(const State* a) const {
		size_t	seed = State::mapSize;

		const uint8_t	*map = a->getMapPtr();

		for(int i = 0; i < State::mapSize; i++) {
			seed ^= map[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return (seed);
	}
};

struct CompareState {
	bool operator()(const State *a, const State *b) {
		if (a->getPrice() != b->getPrice())
			return a->getPrice() > b->getPrice();
		else
			return a->getLength() > b->getLength();
	}
};

struct EqualState {
	bool operator()(const State *lhs, const State *rhs) const {
		const uint8_t *pa = rhs->getMapPtr();
		const uint8_t *pb = lhs->getMapPtr();

		for (int i = 0; i < State::mapSize; i++) {
			if (pa[i] != pb[i])
				return (false);
		}
		return (true);
	}
};

#endif /* STATE_HPP */
