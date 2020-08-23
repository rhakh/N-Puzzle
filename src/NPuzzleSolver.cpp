#include "main.hpp"

#include <unordered_set>
#include <cassert>
#include <cmath>
#include <memory>
#include <iostream>
#include "NPuzzleSolver.hpp"
#include "Heuristic.hpp"

void NPuzzleSolver::checkPath(const State &root, const NP_retVal &result) const {
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

static void createRetVal(NPqueue &open, NPset &closed, const std::shared_ptr<State> &curr, unsigned int maxOpen, NP_retVal &result) {
	size_t	summ = open.size() + closed.size();
	size_t i = 0;
	const State *ptr = curr.get();

	result.maxOpen = maxOpen;
	result.usedMemory = summ * (sizeof(State) + sizeof(int) * 1 /* curr->mapLength */ );
	result.closedNodes = closed.size();

	while (ptr->getMove() != ROOT) {
	    i++;
		result.path.push_front(ptr->getMove());
        ptr = ptr->getPrev();
	}
	result.path.push_front(ROOT);
}

void NPuzzleSolver::aStar(const int *map, NP_retVal &result) {
    NPqueue	open;
    NPset	closed;

    auto addNewState = [&open, &closed](const std::shared_ptr<State> &curr, int move) {
        try {
            auto newState = std::make_shared<State>(*(curr.get()), move);
            open.push(std::move(newState));
        }
        catch (State::NP_InvalidMove &e) {
            // can't create State with this move, let's try next move
            // std::cout << "can't create State with this move, let's try next move" << std::endl;
        }
    };

    auto root = std::make_shared<State>(map);
    open.push(std::move(root));

    while (!open.empty()) {
        auto curr = open.top();
        open.pop();

        // If already exist in closed set
        if (closed.find(curr) != closed.end()) {
            // std::cout << "If already exist in closed set" << std::endl;
            continue;
        }

        if (curr->getPrice() == 0) {
            createRetVal(open, closed, curr, 0, result);
            return;
        }

        for (int move = UP; move < LAST; move++)
            addNewState(curr, move);

        closed.insert(std::move(curr));
    }

    // can't find solution, throw an exception
    throw NP_InvalidMap();
}

NPuzzleSolver::NPuzzleSolver() {
}

static inline int getInversions(const int *map, int mapLength) {
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

	for (int i = 0; i < mapLength; i++)
		if (findIndexInMap(map, mapLength, i) == -1)
			return (false);

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

void NPuzzleSolver::solve(int heuristic, int solutionType,
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
	if (verboseLevel & ALGO) {
		try {
			checkPath(State(map), result);
		}
		catch (std::exception &e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	delete State::finishState;
}
