#include "NPuzzleSolver.hpp"
#include "State.hpp"
#include "heuristicFunctions.hpp"

#define INVAL_MOVE	123456
inline State	*NPuzzleSolver::getNewState(const State *curr, int emptyPos, int newPos, uint8_t move) {
	State	*newState = nullptr;

	switch (newPos) {
		case INVAL_MOVE:
			break;
		default:
			newState = new State(*curr, 0, curr->getLength() + 1, move);
			newState->swapPieces(emptyPos, newPos);
			newState->setPrice(this->heuristicFunc(newState->getMapPtr(), State::mapSize));
			break;
	}
	return (newState);
}

//todo: check for no memory alloc
State * NPuzzleSolver::doMove(const State *curr, uint8_t move, int emptyPos) {
	const int	size = State::size;
	int			x, y, newPos;

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
		default: /* Bug */
			assert(0);
	}
}
#undef INVAL_MOVE

typedef std::priority_queue<State *, std::vector<State *>, CompareState>	NPqueue;
typedef std::unordered_set<State *, HashState, EqualState>	NPset;

void    freeMem(NPqueue *open, NPset *closed) {
	State   *curr = nullptr;

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

void NPuzzleSolver::stopProcess(void) {
	this->stopRequested = true;
}

path_t	*NPuzzleSolver::aStar(const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result) {
	NPqueue	open;
	NPset	closed;
	State	*root;
	State	*curr;
	State	*newMove;
	path_t	*path = nullptr;
	unsigned int	maxOpen = 0, emptyPos = 0;
	const uint8_t	*currMap = 0;

	State::mapSize = mapSize;
	State::size = (int)std::sqrt(mapSize);
	//todo check for mem alloc
	root = new State(map, heuristicFunc(map, mapSize), 0);

	printf("\nSTART\n");
	root->printState();
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
			/* todo: create path */
			printf("\nFINISH\nmaxOpen = %d, closed.size = %zu\n", maxOpen, closed.size());
			curr->printState();

			{
				const State *it = const_cast<const State *>(curr);

				while (it->getMove() != ROOT) {
					result.push_front(it->getMove());
					it = it->getPrev();
				}
				result.push_front(ROOT);
			}

			freeMem(&open, &closed);
			delete curr;
			return (nullptr);
		}

		closed.insert(curr);
		currMap = curr->getMapPtr();

		for (emptyPos = 0; emptyPos < mapSize; emptyPos++)
			if (currMap[emptyPos] == 0)
				break;

		for (int move = UP; move < LAST; move++) {
			newMove = doMove(curr, move, emptyPos);
			if (newMove != nullptr) {
				open.push(newMove);
			}
		}

//		 printf("\nPOP\nopen.size = %zu\n", open.size());
//		 curr->printState();

		if (open.size() > maxOpen)
			maxOpen = open.size();
	}
	printf("Can't find the solution\n");
	freeMem(&open, &closed);
	return (nullptr);
}

NPuzzleSolver::NPuzzleSolver() {
	stopRequested = false;
}

NPuzzleSolver::~NPuzzleSolver() {

}

path_t	*NPuzzleSolver::solve(int func, int algo, const uint8_t *map, uint8_t mapSize, std::list<uint8_t> &result) {
	//todo: replace nullptr for error codes.

	if (mapSize < 9 || mapSize > 100) {
		printf("This map size is invalid or not supported.\n" );
		return (nullptr);
	}

	if (map == nullptr) {
		printf("Map is null.\n");
		return (nullptr);
	}

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

	switch (algo) {
		case ASTAR:
			return aStar(map, mapSize, result);
			break;
		default:
			return aStar(map, mapSize, result);
			break;
	}

	//todo check map (map is squared ? , map is solvable ?)
	// if (checkMap(map, mapSize) == 0) {
	// 	printf("Map is unsolvable.\n");
	// 	return (nullptr);
	// }
}
