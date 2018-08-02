#include "NPuzzleSolver.hpp"
#include "State.hpp"
#include "heuristicFunctions.hpp"
#include "main.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <functional>
#include <map>

typedef std::priority_queue<State *, std::vector<State *>, CompareState>	NPqueue;
typedef std::unordered_set<State *, HashState, EqualState>	NPset;

void	NPuzzleSolver::checkPath(const State *root, const std::list<int> &result, const int mapSize, const int size) const {
	std::string ss[] = {"ROOT", "UP", "DOWN", "LEFT", "RIGHT"};
	State *state;
	State *prev = nullptr;

	std::cout << "###### PRINT PATH ######" << std::endl;
	for (auto const &move: result) {
		std::cout << "Move: " << ss[move] << std::endl << std::flush;
		if (move == 0)
			state = new State(*root, move, mapSize, size);
		else // if can't create State with this move, then constructor throw exception
			state = new State(*prev, move, mapSize, size);
		prev = state;
		state->printState(size);
	}
	std::cout << "###### END PRINT PATH ######" << std::endl;
	// if you see this message, then path is correct
}

void    freeMem(NPqueue *open, NPset *closed) {
	State	*curr = nullptr;

	while (!open->empty()) {
		curr = open->top();
		open->pop();
		delete curr;
	}

	State	*element = nullptr;
	std::unordered_set<State*, HashState, EqualState>::iterator i;
	for (i = closed->begin(); i != closed->end(); i++) {
		element = *i;
		delete element;
	}
}

void 	NPuzzleSolver::createPath(std::list<int> &result, const State *curr) const {
	while (curr->getMove() != ROOT) {
		result.push_front(curr->getMove());
		curr = curr->getPrev();
	}
	result.push_front(ROOT);
}

std::tuple<size_t, size_t, size_t>
constructRetVal(NPqueue *open, NPset *closed, unsigned int maxOpen, const int mapSize) {
	size_t	summ = open->size() + closed->size();

	// used memory
	summ = summ * (sizeof(State) + sizeof(int) * mapSize);
	return (std::make_tuple(maxOpen, closed->size(), summ));
}

std::tuple<size_t, size_t, size_t>
NPuzzleSolver::aStar(const int *map, const int mapSize, int solutionType, std::list<int> &result) {
	std::tuple<size_t, size_t, size_t>	retVal;
	NPqueue	open;
	NPset	closed;
	State	*root;
	State	*curr = nullptr;
	State	*newState;
	size_t	maxOpen = 0;
	const int size = (int)std::sqrt(mapSize);

	this->finishState = new State(solutionType, mapSize);
	root = new State(map, heuristicFunc(map, this->finishState->getMapPtr(), mapSize, size), 0, mapSize);
	open.push(root);

	if (verboseLevel) {
		std::cout << "******* ROOT" << std::endl;
		root->printState(size);
	}

	while (!open.empty()) {
		curr = open.top();
		open.pop();

		if (closed.find(curr) != closed.end()) {
			// curr already exist in closed set
			// pop another one
			delete curr;
			continue;
		}
		closed.insert(curr);

		if (curr->getPrice() == 0) {
			if (verboseLevel) {
				std::cout << "******* SOLVED" << std::endl;
				curr->printState(size);
			}
			createPath(result, const_cast<const State *>(curr));
			retVal = constructRetVal(&open, &closed, maxOpen, mapSize);

			// del allocated mem
			freeMem(&open, &closed);
			delete this->finishState;
			return retVal;
		}

		for (int move = UP; move < LAST; move++) {
			try {
				newState = new State(*curr, move, mapSize, size);
				newState->setPrice(this->heuristicFunc(newState->getMapPtr(),
									this->finishState->getMapPtr(),
									mapSize, size));
				open.push(newState);
			}
			catch (State::NP_InvalidMove &e) {
				// can't create State with this move, let's try next move
			}
			catch (std::exception &e) {
				std::cout << "Error:" << __func__ << ":" << __LINE__ << ":"
							<< e.what() << std::endl;

				// del allocated mem
				freeMem(&open, &closed);
				delete this->finishState;
				throw e;
			}
		}

		if (open.size() > maxOpen)
			maxOpen = open.size();
	}
	//TODO: throw exception that can't find solution
	retVal = constructRetVal(&open, &closed, maxOpen, mapSize);

	//del allocated mem
	freeMem(&open, &closed);
	delete this->finishState;
	return (retVal);
}

NPuzzleSolver::NPuzzleSolver() {
	this->heuristicFunc = nullptr;
	this->finishState = nullptr;
}

NPuzzleSolver::~NPuzzleSolver() {

}

static int	getInversionsForSnail(const int *map, int size, int mapSize, int tail) {
	int inversions = 0;
	int	row = 0;
	int	col = 0;
	int	dx = 1;
	int	dy = 0;
	int matr[size][size];
	bool	find = false;

	// skip zero tail
	if (!tail)
		return (0);

	std::fill(&matr[0][0], &matr[0][0] + (sizeof(matr) / sizeof(matr[0][0])), -1);
	for (int i = 0; i < mapSize; i++) {
		matr[row][col] = i;

		if (map[col + row * size] == tail)
			find = true;

		// skip tiles, before tail appear
		if (map[col + row * size] && find && map[col + row * size] < tail)
			inversions++;

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
	return (inversions);
}

static int getInversions(const int *map, int mapSize, int solutionType) {
	int inversions = 0;
	int size = (int)std::sqrt(mapSize);

	if (solutionType == SNAIL_SOLUTION) {
		int	row = 0;
		int	col = 0;
		int	dx = 1;
		int	dy = 0;
		int matr[size][size];
		bool	find = false;

		std::fill(&matr[0][0], &matr[0][0] + (sizeof(matr) / sizeof(matr[0][0])), -1);
		for (int i = 0; i < mapSize; i++) {
			matr[row][col] = i;

			inversions = getInversionsForSnail(map, size, mapSize, map[col + row * size]);
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
	}
	else {
		for (int i = 0; i < mapSize; i++)
			for (int j = i + 1; j < mapSize; j++) {
				if (map[i] == 0 || map[j] == 0)
					continue;
				if (map[i] > map[j])
					inversions++;
			}
	}

	return (inversions);
}

bool	isSolvableNormal(const int *map, int mapSize) {
	int	inversionsMap = getInversions(map, mapSize, NORMAL_SOLUTION);
	int	zeroRow = 0;
	int	size = (int)std::sqrt(mapSize);

	auto	isEven = [](const int number) { return ((number & 0x1) == 0); };
	auto	isOdd = [](const int number) { return (number & 0x1); };
	auto	getZeroPosition = [&map, &size, &mapSize]() {
				for (int i = mapSize - 1; i >= 0; i--)
						if (map[i] == 0)
							return (size - (i / size));
				return (-1);
	};

	if (isOdd(size)) {
		return (isEven(inversionsMap) ? true : false );
	}
	else {
		zeroRow = getZeroPosition();
		if (isEven(zeroRow) && isOdd(inversionsMap))
			return (true);
		if (isOdd(zeroRow) && isEven(inversionsMap))
			return (true);
	}
	return (false);
}

bool NPuzzleSolver::isSolvable(const int *map, int mapSize, int solutionType) {
#define SECOND_VER
#ifdef FIRST_VER
	int	inversionsMap = getInversions(map, mapSize, solutionType);
	int	zeroRow = 0;
	int	size = (int)std::sqrt(mapSize);

	auto	isEven = [](const int number) { return ((number & 0x1) == 0); };
	auto	isOdd = [](const int number) { return (number & 0x1); };
	auto	getZeroPosition = [&map, &size, &mapSize]() {
				for (int i = mapSize - 1; i >= 0; i--)
						if (map[i] == 0)
							return (size - (i / size));
				return (-1);
	};

	if (isOdd(size)) {
		return (isEven(inversionsMap) ? true : false );
	}
	else {
		zeroRow = getZeroPosition();
		if (isEven(zeroRow) && isOdd(inversionsMap))
			return (true);
		if (isOdd(zeroRow) && isEven(inversionsMap))
			return (true);
	}
	return (false);
#endif // FIRST_VER
#ifdef SECOND_VER
	State	finalState(solutionType, mapSize);
	int	inversionsMap = getInversions(map, mapSize, NORMAL_SOLUTION);
	int inversionsFin = getInversions(finalState.getMapPtr(), mapSize, NORMAL_SOLUTION);
	int size = (int)std::sqrt(mapSize);

	auto	isEven = [](const int number) { return ((number & 0x1) == 0); };
	auto findIndexInMap = [](const int *map, const int mapSize, const int value) {
		for (int i = 0; i < mapSize; i++)
			if (value == map[i])
				return (i);
		return (-1);
	};

	if (isEven(size)) {
		int zeroIdx;

		zeroIdx = findIndexInMap(map, mapSize, 0);
		assert(zeroIdx != -1);
		inversionsMap += zeroIdx;
		zeroIdx = findIndexInMap(finalState.getMapPtr(), mapSize, 0);
		assert(zeroIdx != -1);
		inversionsFin += zeroIdx;
	}
	return !(isEven(inversionsMap) ^ isEven(inversionsFin));
#endif // SECOND_VER
#ifdef THIRD_VER
	State	normalState(NORMAL_SOLUTION, mapSize);
	State	snailState(SNAIL_SOLUTION, mapSize);
	std::vector<int>	converted;
	std::map<int, int>	convertMap;
	auto printMap = [](const int *map, const int mapSize) {
		int size = (int)std::sqrt(mapSize);
		for (int i = 0; i < mapSize; i++) {
			if (i % size == 0)
				std::cout << std::endl << map[i] << " ";
			else
				std::cout << map[i] << " ";
		}
		std::cout << std::endl;
	};

	const int	*normalMap = normalState.getMapPtr();
	const int	*snailMap = snailState.getMapPtr();

	for (int i = 0; i < mapSize; i++) {
		std::cout << "snail[i] = " << snailMap[i] << " === norm[i] = " << normalMap[i] << std::endl;
		convertMap.insert(std::make_pair(snailMap[i], normalMap[i]));
	}
	for (int i = 0; i < mapSize; i++)
		converted.push_back(map[i]);

	std::cout << "@@@@ BEFORE" << std::endl;
	printMap(converted.data(), mapSize);
	for (int i = 0; i < mapSize; i++) {
		auto pair = convertMap.find(map[i]);
		if (pair != convertMap.end()) {
			converted[i] = pair->second;
		}
	}
	std::cout << "@@@@ AFTER" << std::endl;
	printMap(converted.data(), mapSize);

	bool is_solvable = isSolvableNormal(converted.data(), mapSize);
	std::cout << "is solvale = " << is_solvable << std::endl;

	return (is_solvable);
#endif // THIRD_VER
}

std::tuple<size_t, size_t, size_t>
NPuzzleSolver::solve(int heuristic, int solutionType,
		const int *map, const int mapSize, std::list<int> &result) {

	if (mapSize < 9)
		throw NP_InvalidMapSize();

	if (map == nullptr)
		throw NP_MapisNullException();

	if (std::sqrt(mapSize) - (int)(std::sqrt(mapSize)) != 0.0)
		throw NP_InvalidMapSize();

	if (!isSolvable(map, mapSize, solutionType))
		throw NP_InvalidMap();

	switch (heuristic) {
		case MISPLACED_TILES:
			this->heuristicFunc = misplacedTiles;
			break;
		case MANHATTAN_DISTANCE:
			this->heuristicFunc = manhattanDistance;
			break;
		case MANHATTAN_DISTANCE_PLUS_LINEAR_CONFLICTS:
			this->heuristicFunc = MDplusLinearConflicts;
			break;
		case MISPLACED_TILES_PLUS_LINEAR_CONFLICTS:
			this->heuristicFunc = MTplusLinearConflicts;
			break;
		case N_MAXSWAP:
			this->heuristicFunc = nMaxSwap;
			break;
		default:
			throw NP_InvalidHeuristic();
			break;
	}

	return (aStar(map, mapSize, solutionType, result));
}
