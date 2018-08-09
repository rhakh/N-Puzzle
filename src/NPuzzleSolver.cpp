#include "main.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <functional>
#include <map>
#include "NPuzzleSolver.hpp"
#include "Heuristic.hpp"

typedef std::priority_queue<State *, std::vector<State *>, CompareState>	NPqueue;
typedef std::unordered_set<State *, HashState, EqualState>	NPset;

static inline void	freeMem(NPqueue *open, NPset *closed) {
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

void	NPuzzleSolver::checkPath(const State &root, const NP_retVal &result) const {
	std::string ss[] = {"ROOT", "UP", "DOWN", "LEFT", "RIGHT"};
	State *state = nullptr;
	State *prev = nullptr;

	std::cout << "###### PRINT PATH ######" << std::endl << "ROOT" << std::endl;
	root.printState();
	for (auto const &move: result.path) {
		std::cout << "Move: " << ss[move] << std::endl << std::flush;
		if (move == 0)
			state = new State(root, move);
		else // if can't create State with this move, then constructor throw exception
			state = new State(*prev, move);

		if (prev != nullptr)
			 delete prev;
		prev = state;
		state->printState();
	}
	if (state != nullptr)
		 delete state;
}

void 	NPuzzleSolver::createRetVal(NPqueue *open, NPset *closed, const State *curr, unsigned int maxOpen, NP_retVal &result) const {
	size_t	summ = open->size() + closed->size();

	result.maxOpen = maxOpen;
	result.usedMemory = summ * (sizeof(State) + sizeof(int) * State::mapLength);
	result.closedNodes = closed->size();
	while (curr->getMove() != ROOT) {
		result.path.push_front(curr->getMove());
		curr = curr->getPrev();
	}
	result.path.push_front(ROOT);
}

void	NPuzzleSolver::aStar(const int *map, NP_retVal &result) {
	NPqueue	open;
	NPset	closed;
	State	*curr = nullptr;
	State	*root;
	State	*newState;
	size_t	maxOpen = 0;

	root = new State(map);
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
			createRetVal(&open, &closed, curr, maxOpen, result);
			freeMem(&open, &closed);
			return ;
		}

		for (int move = UP; move < LAST; move++) {
			try {
				newState = new State(*curr, move);
				open.push(newState);
			}
			catch (State::NP_InvalidMove &e) {
				// can't create State with this move, let's try next move
			}
			catch (std::exception &e) {
				std::cout << "Error:" << __func__ << ":" << __LINE__ << ":"
							<< e.what() << std::endl;
				freeMem(&open, &closed);
				throw e;
			}
		}

		if (open.size() > maxOpen)
			maxOpen = open.size();
	}
	freeMem(&open, &closed);
	// can't find solution, throw an exception
	throw NP_InvalidMap();
	return ;
}

NPuzzleSolver::NPuzzleSolver() {
}

static inline int	getInversions(const int *map, int mapLength) {
	int inversions = 0;

	for (int i = 0; i < mapLength; i++)
		for (int j = i + 1; j < mapLength; j++) {
			if (map[i] == 0 || map[j] == 0)
				continue;
			if (map[i] > map[j])
				inversions++;
		}

	return (inversions);
}

bool NPuzzleSolver::isSolvable(const int *map, int mapLength, int solutionType) {
	State	finishState(solutionType);
	int		inversionsFin = getInversions(finishState.getMapPtr(), mapLength);
	int		inversionsMap = getInversions(map, mapLength);
	int		size = (int)std::sqrt(mapLength);

	auto	isEven = [](const int number) { return ((number & 0x1) == 0); };
	auto	findIndexInMap = [](const int *map, const int mapLength, const int value) {
				for (int i = 0; i < mapLength; i++)
					if (value == map[i])
						return (i);
				return (-1);
	};

	if (isEven(size)) {
		int zeroIdx;

		zeroIdx = findIndexInMap(map, mapLength, 0);
		assert(zeroIdx != -1);
		inversionsMap += zeroIdx;
		zeroIdx = findIndexInMap(finishState.getMapPtr(), mapLength, 0);
		assert(zeroIdx != -1);
		inversionsFin += zeroIdx;
	}
	return !(isEven(inversionsMap) ^ isEven(inversionsFin));
}

void	NPuzzleSolver::solve(int heuristic, int solutionType,
		const int *map, const int mapLength, NP_retVal &result)
{
	if (mapLength < 9 ||
		std::sqrt(mapLength) - (int)(std::sqrt(mapLength)) != 0.0)
		throw NP_InvalidMapSize();

	if (map == nullptr)
		throw NP_MapisNullException();

	switch (heuristic) {
		case MISPLACED_TILES:
			State::heuristicFunc = &Heuristic::misplacedTiles;
			break;
		case MANHATTAN_DISTANCE:
			State::heuristicFunc = &Heuristic::manhattanDistance;
			break;
		case MANHATTAN_DISTANCE_PLUS_LINEAR_CONFLICTS:
			State::heuristicFunc = &Heuristic::MDplusLinearConflicts;
			break;
		case MISPLACED_TILES_PLUS_LINEAR_CONFLICTS:
			State::heuristicFunc = &Heuristic::MTplusLinearConflicts;
			break;
		case N_MAXSWAP:
			State::heuristicFunc = &Heuristic::nMaxSwap;
			break;
		default:
			throw NP_InvalidHeuristic();
			break;
	}

	State::mapLength = mapLength;
	State::mapSize = (int)std::sqrt(mapLength);
	State::finishState = new State(solutionType);

	if (!isSolvable(map, mapLength, solutionType))
		throw NP_InvalidMap();

	aStar(map, result);
	if (verboseLevel == ALGO) {
		try {
			checkPath(State(map), result);
		}
		catch (std::exception &e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	delete State::finishState;
	return ;
}
