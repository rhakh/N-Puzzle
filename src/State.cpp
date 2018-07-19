#include "State.hpp"

#include <cstdio>
#include <algorithm>
#include <cmath>

int State::size = 4;
int State::mapSize = State::size * State::size;

State::State(const uint8_t *map, int price, int length) {
	this->map = new uint8_t[State::mapSize];

	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = map[i];

	this->price = price;
	this->length = length;
	this->movement = ROOT;
	this->prev = nullptr;
}

void	State::makeSnailState() {
	int	row = 0;
	int	col = 0;
	int	dx = 1;
	int	dy = 0;
	int	dirCh = 0;
	int	vis = State::size;

	uint8_t matr[State::size][State::size];

	for (int i = 0; i < State::mapSize; i++) {
		matr[row][col] = i + 1;
		if (i + 1 == State::mapSize)
			matr[row][col] = 0;
		vis--;
		if (vis == 0) {
			vis = State::size * (dirCh % 2) + State::size * ((dirCh + 1) % 2) - (dirCh / 2 - 1) - 2;
			int tmp = dx;
			dx = -dy;
			dy = tmp;
			dirCh++;
		}
		col += dx;
		row += dy;
	}

	int m = 0;
	this->map = new uint8_t[State::mapSize];
	for (int i = 0; i < State::size; i++)
		for (int j = 0; j < State::size; j++)
			this->map[m++] = matr[i][j];

	this->price = 0;
	this->length = 0;
	this->movement = 0;
	this->prev = nullptr;
}

void	State::makeNormalState() {
	this->map = new uint8_t[State::mapSize];
	for (int i = 0; i < State::mapSize; i++)
		this->map[i] = i + 1;
	this->map[State::mapSize - 1] = 0;

	this->price = 0;
	this->length = 0;
	this->movement = 0;
	this->prev = nullptr;
}

State::State(int solutionType) {
	if (solutionType == SNAIL_SOLUTION)
		makeSnailState();
	else
		makeNormalState();
}

State::State(const State &src, int price, int length, uint8_t move) {
	const uint8_t	*map = src.getMapPtr();

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

void			State::swapPieces(int a, int b) {
	std::swap(map[a], map[b]);
}

size_t HashState::operator()(const State* a) const {
	size_t	seed = State::mapSize;

	const uint8_t	*map = a->getMapPtr();

	for(int i = 0; i < State::mapSize; i++) {
		seed ^= map[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return (seed);
}

bool CompareState::operator()(const State *a, const State *b) {
	if (a->getPrice() != b->getPrice())
		return a->getPrice() > b->getPrice();
	else
		return a->getLength() > b->getLength();
}

bool EqualState::operator()(const State *lhs, const State *rhs) const {
	const uint8_t *pa = rhs->getMapPtr();
	const uint8_t *pb = lhs->getMapPtr();

	for (int i = 0; i < State::mapSize; i++) {
		if (pa[i] != pb[i])
			return (false);
	}
	return (true);
}
