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

#include <functional>
bool NPuzzleSolver::isSolvable(const int *map, int mapSize, int solutionType) {
	int	inversions = 0;
	int size = (int)std::sqrt(mapSize);
	int zeroY = -1;

	auto getInversions = [&map, &mapSize, &inversions]() {
		for (int i = 0; i < mapSize; i++)
		for (int j = i + 1; j < mapSize; j++) {
			if (map[i] > map[j])
				inversions++;
		}
	};

	auto getZeroPos = [&map, &mapSize, &zeroY, &size]() {
		for (int i = mapSize - 1; i >= 0; i--)
			if (map[i] == 0) {
				zeroY = i / size;
			}
	};

	getInversions();
	getZeroPos();

	if (zeroY == -1)
		return (false);

	zeroY = size - zeroY;

	printf("size = %d, inversions = %d, zeroY = %d\n", size, inversions, zeroY);
	printf("(%d) == (%d)\n", (zeroY % 2 != 0), (inversions % 2 == 0));

	if (size % 2 != 0)
		return (inversions % 2 == 0);
	else
		return ((zeroY % 2 != 0) == (inversions % 2 == 0));
}

std::tuple<size_t, size_t, size_t>
NPuzzleSolver::solve(int func, int algo, int solutionType,
		const int *map, const int mapSize, std::list<int> &result) {

	if (mapSize < 9)
		throw NP_InvalidMapSize();

	if (map == nullptr)
		throw NP_MapisNullException();

	if (std::sqrt(mapSize) - (int)(std::sqrt(mapSize)) != 0.0)
		throw NP_InvalidMapSize();

	// if (!isSolvable(map, mapSize, solutionType))
	// 	throw NP_InvalidMap();

	switch (func) {
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
		default:
			this->heuristicFunc = misplacedTiles;
			break;
	}

	switch (algo) {
		case ASTAR:
			return (aStar(map, mapSize, solutionType, result));
			break;
		default:
			return (aStar(map, mapSize, solutionType, result));
			break;
	}
}
