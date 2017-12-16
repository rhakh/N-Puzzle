#include "State.h"

uint8_t State::n_size = 0;
uint8_t State::d_size = 0;

State::State(uint8_t cost, int length, const uint8_t *map, uint8_t size) {
	this->map = new uint8_t[size];
	/* TODO: Add exception handler, if memory can't be allocated */
	if (this->map != nullptr)
		memcpy(this->map, map, size);
	else
		dbg("Can't allocate memory\n");
	this->cost = cost;
	this->length = length;
	this->d_size = size;
	this->n_size = (uint8_t)sqrt(this->d_size);
}

State::State(uint8_t cost, int length) {
	this->cost = cost;
	this->length = length;
	this->map = nullptr;
}

State::~State() {
	if (this->map)
		delete[](this->map);
}

void 	State::printState() const {
	printf("State.cost = %u, State.length = %d\n", this->cost, this->length);
	for (int i = 0; i < State::d_size; ++i) {
		if (i % n_size == 0)
			printf("\n");
		printf("%2u ", this->map[i]);
	}
	printf("\n");
}


