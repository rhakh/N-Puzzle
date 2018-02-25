#include "State.hpp"

int State::size = 4;
int State::dsize = size * size;

State::State(const uint8_t *map, int price, int length) {
	//todo: check for mem alloc
	this->map = new uint8_t[State::dsize];

	for (int i = 0; i < State::dsize; i++)
		this->map[i] = map[i];

	this->price = price;
	this->length = length;
}

//State::State(State const&src) {
//	const uint8_t	*src_map;
//
//	//todo: check for mem alloc
//	this->map = new uint8_t[State::dsize];
//	src_map = src.getMapPtr();
//
//	for (int i = 0; i < State::dsize; i++)
//		this->map[i] = src_map[i];
//
//	this->price = src.getPrice();
//	this->length = src.getLength();
//}

State::State() {
	this->map = nullptr;
	this->price = 0;
	this->length = 0;
}

State::~State() {
	delete[] this->map;
}

void	State::printState() const {
	printf("State price = %d, length = %d\n", this->price, this->length);
    if (this->map != nullptr) {
        for (int i = 0; i < State::dsize; i++) {
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

//bool	State::operator==(State const *rhs) const {
//	const uint8_t *pa;
//
//	pa = rhs->getMapPtr();
//	for (int i = 0; i < State::dsize; i++) {
//		if (pa[i] != this->map[i])
//			return (false);
//	}
//	return (true);
//}
