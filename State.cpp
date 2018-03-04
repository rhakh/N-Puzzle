#include "State.hpp"

int State::size = -1;
int State::mapSize = -1;

State::State(const uint8_t *map, int price, int length) {
	//todo: check for mem alloc
	this->map = new uint8_t[State::mapSize];

	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = map[i];

	this->price = price;
	this->length = length;
}

State::State() {
	this->map = new uint8_t[State::mapSize];
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

void	State::setMap(const uint8_t *srcMap) {
	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = srcMap[i];
}
