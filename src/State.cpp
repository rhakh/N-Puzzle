#include "State.hpp"

#include <cstdio>
#include <algorithm>
#include <cmath>

auto findIndexInMap = [](int value, const int *map, int mapSize) {
    for (int i = 0; i < mapSize; i++)
		if (map[i] == value)
			return (i);
	return (-1);
};

State::State(const int *map, int price, int length, const int mapSize) {
	this->map.reserve(mapSize);
	for (int i = 0; i < mapSize; i++)
		this->map.push_back(map[i]);

	this->price = price;
	this->length = length;
	this->movement = ROOT;
	this->prev = nullptr;
}

void	State::makeSnailState(const int mapSize) {
	int	row = 0;
	int	col = 0;
	int	dx = 1;
	int	dy = 0;
	int	dirCh = 0;
	int	vis = (int)std::sqrt(mapSize);
	int	size = (int)std::sqrt(mapSize);

	int matr[size][size];

	for (int i = 0; i < mapSize; i++) {
		matr[row][col] = i + 1;
		if (i + 1 == mapSize)
			matr[row][col] = 0;
		vis--;
		if (vis == 0) {
			vis = size * (dirCh % 2) + size * ((dirCh + 1) % 2) - (dirCh / 2 - 1) - 2;
			int tmp = dx;
			dx = -dy;
			dy = tmp;
			dirCh++;
		}
		col += dx;
		row += dy;
	}

	this->map.reserve(mapSize);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			this->map.push_back(matr[i][j]);

	this->price = 0;
	this->length = 0;
	this->movement = 0;
	this->prev = nullptr;
}

void	State::makeNormalState(const int mapSize) {
	this->map.reserve(mapSize);
	for (int i = 0; i < mapSize; i++)
		this->map.push_back(i + 1);
	this->map[mapSize - 1] = 0;

	this->price = 0;
	this->length = 0;
	this->movement = 0;
	this->prev = nullptr;
}

State::State(const int solutionType, const int mapSize) {
	if (solutionType == SNAIL_SOLUTION)
		makeSnailState(mapSize);
	else
		makeNormalState(mapSize);
}

State::State(const State &src, const int move, const int mapSize, const int size) {
	const int	*map = src.getMapPtr();
	int			x, y, newPos, zeroIndex;

    zeroIndex = findIndexInMap(0, map, mapSize);

	x = zeroIndex % size;
	y = zeroIndex / size;

	switch (move) {
		case UP:
			if (y - 1 < 0)
				throw NP_InvalidMove();
			newPos = x + ((y - 1) * size);
			break;
		case DOWN:
			if (y + 1 == size)
				throw NP_InvalidMove();
			newPos = x + ((y + 1) * size);
			break;
		case LEFT:
			if (x - 1 < 0)
				throw NP_InvalidMove();
			newPos = (x - 1) + (y * size);
			break;
		case RIGHT:
			if (x + 1 == size)
				throw NP_InvalidMove();
			newPos = (x + 1) + (y * size);
			break;
		case ROOT: // just make a copy
			newPos = zeroIndex;
			break;
		default:
			throw NP_InvalidMove();
			break;
	}

	this->map.reserve(mapSize);
	for (int i = 0; i < mapSize; i++)
		this->map.push_back(map[i]);
	this->swapPieces(zeroIndex, newPos);

    this->price = 0;
	this->length = src.getLength() + 1;
	this->movement = move;
	this->prev = &src;
}

int	State::getMove() const {
	return (this->movement);
}

const State	*State::getPrev() const {
	return (this->prev);
}

State::~State() {}

void	State::printState(const int size) const {
	int mapSize = this->map.size();

	printf("State price = %d, length = %d\n", this->price, this->length);
	for (int i = 0; i < mapSize; i++) {
		if ((i + 1) % size == 0)
			printf("%2u\n", this->map[i]);
		else
			printf("%2u ", this->map[i]);
	}
	printf("\n\n");
}

void	State::swapPieces(int a, int b) {
	std::swap(map[a], map[b]);
}

size_t HashState::operator()(const State* a) const {
	const int	*map = a->getMapPtr();
	const int	mapSize = a->getMapSize();
	size_t		seed = mapSize;

	for(int i = 0; i < mapSize; i++) {
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
	const int *pa = rhs->getMapPtr();
	const int *pb = lhs->getMapPtr();
	const int mapSize = lhs->getMapSize();

	for (int i = 0; i < mapSize; i++) {
		if (pa[i] != pb[i])
			return (false);
	}
	return (true);
}
