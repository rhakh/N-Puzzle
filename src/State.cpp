#include "State.hpp"

int State::size = 4;
int State::mapSize = State::size * State::size;

State::State(const uint8_t *map, int price, int length) {
	//todo: check for mem alloc
	this->map = new uint8_t[State::mapSize];

	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = map[i];

	this->price = price;
	this->length = length;
	this->movement = ROOT;
	this->prev = nullptr;
}

State::State(const State &src, int price, int length, uint8_t move) {
	const uint8_t	*map = src.getMapPtr();

	//todo: check for mem alloc
	this->map = new uint8_t[State::mapSize];
	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = map[i];

	this->price = price;
	this->length = length;
	this->movement = move;
	this->prev = &src;
}

uint8_t	State::getMove() const {
	return (this->movement);
}

const State	*State::getPrev() const {
	return (this->prev);
}

State::~State() {
	delete[] this->map;
}

void	State::printState() const {
	printf("State price = %d, length = %d\n", this->price, this->length);
	if (this->map != nullptr) {
		for (int i = 0; i < State::mapSize; i++) {
			if ((i + 1) % State::size == 0)
			printf("%2u\n", this->map[i]);
			else
			printf("%2u ", this->map[i]);
		}
	}
	else
	printf("map is null\n");
	printf("\n\n");
}
