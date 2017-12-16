#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include <iomanip>

#define DEBUG_FLAG 0

#if DEBUG_FLAG
#define dbg(fmt, args...) \
    do { \
        printf("%s():%d: ", __func__, __LINE__); \
        printf(fmt, ##args); \
       } while (0)
#else /* DEBUG_FLAG */
#define dbg(fmt, args...)
#endif /* DEBUG_FLAG */

class State {

private:
	uint8_t	cost = 0;
	int 	length = 0;
	uint8_t *map = nullptr;
    State   *prev = nullptr;

public:
	static uint8_t n_size;
	static uint8_t d_size;

	State(uint8_t cost, int length, const uint8_t *map, uint8_t size);
	State(uint8_t cost, int length);
    State(const State *src);
	~State();

	uint8_t getCost() const { return this->cost; };
	int 	getLength() const { return this->length; }
	const uint8_t *getConstMap() const { return this->map; }
	void 	setMap(uint8_t *map) { this->map = map; }
    void    setPrev(State *prev) { this->prev = prev; }
    const State   *getPrev() const { return this->prev; }

	void 	printState() const ;
};

struct CompareState {
	bool operator()(const State *a, const State *b) {
		if (a->getCost() != b->getCost())
			return a->getCost() > b->getCost();
		return a->getLength() > b->getLength();
	}
};

struct StateHash {
	size_t operator()(const State *a) const {
		const uint8_t *map = a->getConstMap();
		std::size_t seed = State::d_size;

		for(int i = 0; i < State::d_size; ++i)
			seed ^= map[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

struct StateEq {
	bool    operator()(State * const a, State * const b) const {
		const uint8_t *mapa = a->getConstMap();
		const uint8_t *mapb = b->getConstMap();

		for (uint8_t i = 0; i < State::d_size; ++i) {
			if (mapa[i] != mapb[i])
				return false;
		}
		return true;
	}
};

#endif /* STATE_H */
