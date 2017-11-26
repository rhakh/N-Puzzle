#include "State.h"
#include <iostream>
#include "stdio.h"

State::State(std::vector<int> _map, int _const, int _length) {
    this->map = _map;
    this->cost = _const;
    this->length = _length;
}

State::State() {
    this->map = std::vector<int> (0);
    this->cost = 0;
    this->length = 0;
}

State::~State() {

}

void State::printState() const {
	for (int i = 0; i < this->map.size(); i++) {
		if (i % N_SIZE == 0)
			printf("\n");
		printf("%2d ", this->map[i]);
	}
	printf("\n");
}