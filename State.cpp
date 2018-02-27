#include "State.hpp"

int State::size = 4;
int State::mapSize = size * size;

State::State(const uint8_t *map, int price, int length) {
	//todo: check for mem alloc
	this->map = new uint8_t[State::mapSize];

	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = map[i];

	this->price = price;
	this->length = length;

	this->hash = State::size;
	for(int i = 0; i < State::size; i++) {
		this->hash ^= map[i] + 0x9e3779b9 + (this->hash << 6) + (this->hash >> 2);
	}
}

State::State() {
	this->map = nullptr;
	this->price = 0;
	this->length = 0;
	this->hash = 0;
}

State::~State() {
	delete[] this->map;
}

void	State::printState() const {
	printf("State price = %d, length = %d\n", this->price, this->length);
    if (this->map != nullptr) {
        for (int i = 0; i < State::mapSize; i++) {
            if ((i + 1) % State::size == 0)
                printf("%d\n", this->map[i]);
            else
                printf("%d ", this->map[i]);
        }
    }
    else
        printf("map is null\n");
	printf("\n\n");
}
