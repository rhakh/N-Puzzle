#include "NPuzzleSolver.hpp"
#include "State.hpp"
#include "heuristicFunctions.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <functional>

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
			std::cout << "******* SOLVED" << std::endl;
			curr->printState(size);
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

static int	getInversionsForSnail(const int *map, int size, int mapSize, int value) {
	int inversions = 0;
	int	row = 0;
	int	col = 0;
	int	dx = 1;
	int	dy = 0;
	int matr[size][size];
	bool	find = false;

	std::fill(&matr[0][0], &matr[0][0] + (sizeof(matr) / sizeof(matr[0][0])), -1);
	for (int i = 0; i < mapSize; i++) {
		if (map[i] == 0)
			continue;
		if (map[i] == value)
			find = true;
		if (find && map[i] < value)
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
			if (map[i] == 0)
				continue;
			inversions += getInversionsForSnail(map, size, mapSize, map[col + row * size]);
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

bool NPuzzleSolver::isSolvable(const int *map, int mapSize, int solutionType) {
	int	inversionsMap = getInversions(map, mapSize, solutionType);
	int	zeroRow = 0;
	int	size = (int)std::sqrt(mapSize);

	auto	isEven = [](const int number) { return ((number & 0x1) == 0); };
	auto	isOdd = [](const int number) { return (number & 0x1); };
	auto	getZeroPosition = [&map, &size, &mapSize]() {
				for (int i = mapSize; i >= 0; i--)
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
