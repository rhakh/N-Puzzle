#include "State.hpp"

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <iomanip>

auto findIndexInMap = [](int value, const int *map, int mapSize) {
	for (int i = 0; i < mapSize; i++)
		if (map[i] == value)
			return (i);
	return (-1);
};

State::State(const int *map, int price, int length, const int mapSize) {
	this->map.resize(mapSize);
	for (int i = 0; i < mapSize; i++)
		this->map[i] = map[i];

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
	int	size = (int)std::sqrt(mapSize);
	int matr[size][size];

	std::fill(&matr[0][0], &matr[0][0] + (sizeof(matr) / sizeof(matr[0][0])), -1);
	for (int i = 0; i < mapSize; i++) {
		matr[row][col] = i + 1;
		if (i + 1 == mapSize)
			matr[row][col] = 0;

		if ((col + dx == size || col + dx < 0 ||
			(dx != 0 && matr[row][col + dx] != -1)) ||
			(row + dy == size || row + dy < 0 ||
			(dy != 0 && matr[row + dy][col] != -1)))
		{
			std::swap(dx, dy);
			dx *= -1;
		}

		col += dx;
		row += dy;
	}

	this->map.resize(mapSize);
	int m = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			this->map[m++] = matr[i][j];

	this->price = 0;
	this->length = 0;
	this->movement = 0;
	this->prev = nullptr;
}

void	State::makeNormalState(const int mapSize) {
	this->map.resize(mapSize);
	for (int i = 0; i < mapSize; i++)
		this->map[i] = i + 1;
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

	this->map.resize(mapSize);
	for (int i = 0; i < mapSize; i++)
		this->map[i] = map[i];
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

	printf("State price = %d, length = %d, mapSize = %d\n", this->price, this->length, mapSize);
	for (int i = 0; i < mapSize; i++) {
		if ((i + 1) % size == 0)
			std::cout << std::setw(2) << this->map[i] << std::endl;
		else
			std::cout << std::setw(2) << this->map[i] << " ";
	}
	std::cout << std::endl << std::endl;
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

	// TODO: think about that
	// return (a->getPrice() + a->getLength() > b->getPrice() + b->getLength());
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
