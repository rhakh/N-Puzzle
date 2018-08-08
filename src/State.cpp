#include "State.hpp"

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <iomanip>

State	*State::finishState = nullptr;
int		(*State::heuristicFunc)(const State *state) = nullptr;
int		State::mapSize = 0, State::mapLength = 0;

auto findIndexInMap = [](int value, const int *map, const int mapLength) {
	for (int i = 0; i < mapLength; i++)
		if (map[i] == value)
			return (i);
	return (-1);
};

State::State(const int *map) {
	if (finishState == nullptr || State::heuristicFunc == nullptr ||
		mapSize == 0 || mapLength == 0) {
		printf("%s:%d %d %d %d %d\n", __func__, __LINE__,
			finishState == nullptr, State::heuristicFunc == nullptr,
			mapSize == 0, mapLength == 0);
		throw (NP_StaticVarsUnset());
	}

	this->map.resize(State::mapLength);
	for (int i = 0; i < State::mapLength; i++)
		this->map[i] = map[i];

	this->price = State::heuristicFunc(this);
	this->length = 0;
	this->cost = price;
	this->movement = ROOT;
	this->prev = nullptr;
}

void	State::makeSnailState() {
	int	row = 0;
	int	col = 0;
	int	dx = 1;
	int	dy = 0;
	int matr[State::mapSize][State::mapSize];

	std::fill(&matr[0][0], &matr[0][0] + (sizeof(matr) / sizeof(matr[0][0])), -1);
	for (int i = 0; i < State::mapLength; i++) {
		matr[row][col] = i + 1;
		if (i + 1 == State::mapLength)
			matr[row][col] = 0;

		if ((col + dx == State::mapSize || col + dx < 0 ||
			(dx != 0 && matr[row][col + dx] != -1)) ||
			(row + dy == State::mapSize || row + dy < 0 ||
			(dy != 0 && matr[row + dy][col] != -1)))
		{
			std::swap(dx, dy);
			dx *= -1;
		}

		col += dx;
		row += dy;
	}

	this->map.resize(State::mapLength);
	int m = 0;
	for (int i = 0; i < State::mapSize; i++)
		for (int j = 0; j < State::mapSize; j++)
			this->map[m++] = matr[i][j];
}

void	State::makeNormalState() {
	this->map.resize(State::mapLength);
	for (int i = 0; i < State::mapLength; i++)
		this->map[i] = i + 1;
	this->map[State::mapLength - 1] = 0;
}

State::State(const int solutionType) {
	if (mapSize == 0 || mapLength == 0) {
		printf("%s:%d %d %d %d %d\n", __func__, __LINE__,
			finishState == nullptr, State::heuristicFunc == nullptr,
			mapSize == 0, mapLength == 0);
		throw (NP_StaticVarsUnset());
	}

	if (solutionType == SNAIL_SOLUTION)
		makeSnailState();
	else
		makeNormalState();
	this->price = 0;
	this->cost = 0;
	this->length = 0;
	this->movement = 0;
	this->prev = nullptr;
	this->movement = ROOT;
}

State::State(const State &src, const int move)
{
	const int	*map = src.getMapPtr();
	int			x, y, newPos, zeroIndex;

	if (finishState == nullptr || State::heuristicFunc == nullptr ||
		mapSize == 0 || mapLength == 0) {
		printf("%s:%d %d %d %d %d\n", __func__, __LINE__,
			finishState == nullptr, State::heuristicFunc == nullptr,
			mapSize == 0, mapLength == 0);
		throw (NP_StaticVarsUnset());
	}

	zeroIndex = findIndexInMap(0, map, State::mapLength);

	x = zeroIndex % State::mapSize;
	y = zeroIndex / State::mapSize;

	switch (move) {
		case UP:
			if (y - 1 < 0)
				throw NP_InvalidMove();
			newPos = x + ((y - 1) * State::mapSize);
			break;
		case DOWN:
			if (y + 1 == State::mapSize)
				throw NP_InvalidMove();
			newPos = x + ((y + 1) * State::mapSize);
			break;
		case LEFT:
			if (x - 1 < 0)
				throw NP_InvalidMove();
			newPos = (x - 1) + (y * State::mapSize);
			break;
		case RIGHT:
			if (x + 1 == State::mapSize)
				throw NP_InvalidMove();
			newPos = (x + 1) + (y * State::mapSize);
			break;
		case ROOT: // just make a copy
			newPos = zeroIndex;
			break;
		default:
			throw NP_InvalidMove();
			break;
	}

	this->map.resize(State::mapLength);
	for (int i = 0; i < State::mapLength; i++)
		this->map[i] = map[i];
	this->swapPieces(zeroIndex, newPos);

	this->price = State::heuristicFunc(this);
	this->length = src.getLength() + 1;
	this->cost = this->price + this->length;
	this->movement = move;
	this->prev = &src;
}

void	State::printState() const {
	printf("State price = %d, length = %d, mapSize = %d\n", this->price, this->length, State::mapSize);
	for (int i = 0; i < State::mapLength; i++) {
		if (i % State::mapSize == 0)
			std::cout << std::endl;

		if (this->map[i])
			std::cout << std::setw(2) << this->map[i] << " ";
		else
			std::cout << std::setw(2) << "__ ";
	}
	std::cout << std::endl << std::endl;
}

size_t HashState::operator()(const State* a) const {
	const int	*map = a->getMapPtr();
	const int	mapLength = a->getMapLength();
	size_t		seed = mapLength;

	for(int i = 0; i < mapLength; i++) {
		seed ^= map[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	// return boost::hash_range(map, map + mapSize);

	return (seed);
}

bool CompareState::operator()(const State *a, const State *b) {

	// fast version
	if (a->getPrice() == b->getPrice())
		return a->getLength() > b->getLength();
	return a->getPrice() > b->getPrice();

	// true version ?
	// if (a->getCost() == b->getCost())
	// 	return a->getLength() > b->getLength();
	// return a->getCost() > b->getCost();
}

bool EqualState::operator()(const State *lhs, const State *rhs) const {
	const int *pa = rhs->getMapPtr();
	const int *pb = lhs->getMapPtr();
	const int mapLength = lhs->getMapLength();

	for (int i = 0; i < mapLength; i++) {
		if (pa[i] != pb[i])
			return (false);
	}
	return (true);
}
