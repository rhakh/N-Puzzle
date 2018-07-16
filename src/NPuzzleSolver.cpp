#include "NPuzzleSolver.hpp"
#include "State.hpp"
#include "heuristicFunctions.hpp"

#include <queue>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <functional>

#define INVAL_MOVE	123456
inline State	*NPuzzleSolver::getNewState(const State *curr, int emptyPos, int newPos, uint8_t move) {
	State	*newState = nullptr;

	switch (newPos) {
		case INVAL_MOVE:
			break;
		default:
			newState = new State(*curr, 0, curr->getLength() + 1, move);
			newState->swapPieces(emptyPos, newPos);
			newState->setPrice(this->heuristicFunc(newState->getMapPtr(), this->finishState->getMapPtr(), State::mapSize));
			break;
	}
	return (newState);
}

//todo: check for no memory alloc
State * NPuzzleSolver::doMove(const State *curr, uint8_t move) {
	const int	size = State::size;
	int			x, y, newPos, emptyPos;

	auto		_emptyPos = [curr]() -> int {
		const	uint8_t	*map = curr->getMapPtr();

		for (int i = 0; i < State::mapSize; i++)
			if (map[i] == 0)
				return (i);
		throw NP_InvalidMap();
	};

	emptyPos = _emptyPos();

	x = emptyPos % size;
	y = emptyPos / size;

	switch (move) {
		case UP:
			newPos = (y - 1 < 0) ? (INVAL_MOVE) : x + ((y - 1) * size);
			return (getNewState(curr, emptyPos, newPos, move));
			break;
		case DOWN:
			newPos = (y + 1 == size) ? (INVAL_MOVE) : x + ((y + 1) * size);
			return (getNewState(curr, emptyPos, newPos, move));
			break;
		case LEFT:
			newPos = (x - 1 < 0) ? (INVAL_MOVE) : (x - 1) + (y * size);
			return (getNewState(curr, emptyPos, newPos, move));
			break;
		case RIGHT:
			newPos = (x + 1 == size) ? (INVAL_MOVE) : (x + 1) + (y * size);
			return (getNewState(curr, emptyPos, newPos, move));
			break;
		default: // Bug
			assert(0);
	}
}
#undef INVAL_MOVE

typedef std::priority_queue<State *, std::vector<State *>, CompareState>	NPqueue;
typedef std::unordered_set<State *, HashState, EqualState>	NPset;

void    freeMem(NPqueue *open, NPset *closed, State *last) {
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
	if (last != nullptr)
		delete last;
}

void NPuzzleSolver::stopProcess(void) {
	this->stopRequested = true;
}

void 	NPuzzleSolver::createPath(std::list<uint8_t> &result, const State *curr) const {
	if (!this->stopRequested)
	{
		while (curr->getMove() != ROOT) {
			result.push_front(curr->getMove());
			curr = curr->getPrev();
		}
		result.push_front(ROOT);
	}
	else // If process was stopped
		result.push_front(ROOT);
}

std::tuple<size_t, size_t, size_t>
constructRetVal(NPqueue *open, NPset *closed, unsigned int maxOpen) {
	size_t	summ = open->size() + closed->size();

	// used memory
	summ = summ * (sizeof(State) + sizeof(uint8_t) * State::mapSize);
	return (std::make_tuple(maxOpen, closed->size(), summ));
}

std::tuple<size_t, size_t, size_t>
NPuzzleSolver::aStar(const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result) {
	std::tuple<size_t, size_t, size_t>	retVal;
	NPqueue	open;
	NPset	closed;
	State	*root;
	State	*curr = nullptr;
	State	*newMove;
	size_t	maxOpen = 0;
	unsigned int	emptyPos = 0;
	const uint8_t	*currMap = 0;

	State::mapSize = mapSize;
	State::size = (int)std::sqrt(mapSize);

	this->finishState = new State();
	root = new State(map, heuristicFunc(map, this->finishState->getMapPtr(), mapSize), 0);
	open.push(root);

	while (!open.empty()) {
		curr = open.top();
		open.pop();

		if (closed.find(curr) != closed.end()) {
			/* curr already exist in closed set
			 * pop another one */
			delete curr;
			continue;
		}

		if (curr->getPrice() == 0 || this->stopRequested) {
			createPath(result, const_cast<const State *>(curr));
			this->stopRequested = false;
			retVal = constructRetVal(&open, &closed, maxOpen);
			freeMem(&open, &closed, curr);
			delete this->finishState;
			return retVal;
		}

		closed.insert(curr);
		// std::cout << "******* POP" << std::endl;
		// curr->printState();
		for (int move = UP; move < LAST; move++) {
			try {
				newMove = doMove(curr, move);
			}
			catch (std::exception &e) {
				// if memory was not allocated, you should free mem
				this->stopRequested = false;
				freeMem(&open, &closed, nullptr);
				delete this->finishState;
				std::cout << "Error:" << __func__ << ":" << __LINE__ << ":"
							<< e.what() << std::endl;
				throw e;
			}
			if (newMove != nullptr)
				open.push(newMove);
		}

		if (open.size() > maxOpen)
			maxOpen = open.size();
	}
	retVal = constructRetVal(&open, &closed, maxOpen);
	freeMem(&open, &closed, curr);
	delete this->finishState;
	return (retVal);
}

NPuzzleSolver::NPuzzleSolver() {
	stopRequested = false;
}

NPuzzleSolver::~NPuzzleSolver() {

}

std::tuple<size_t, size_t, size_t>
NPuzzleSolver::solve(int func, int algo, const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result) {
	//todo: replace nullptr for error codes.

	if (mapSize < 9 || mapSize > 100)
		throw NP_InvalidMapSize();

	if (map == nullptr)
		throw NP_MapisNullException();

	switch (func) {
		case HAMMILTON_DISTANCE:
			this->heuristicFunc = hammiltonDistance;
			break;
		case MANHATTAN_DISTANCE:
			this->heuristicFunc = manhattanDistance;
			break;
		default:
			this->heuristicFunc = hammiltonDistance;
			break;
	}

	//todo check map (map is squared ? , map is solvable ?)
	// if (checkMap(map, mapSize) == 0) {
	// 	throw NP_InvalidMap;
	// }

	switch (algo) {
		case ASTAR:
			return (aStar(map, mapSize, result));
			break;
		default:
			return (aStar(map, mapSize, result));
			break;
	}
}
