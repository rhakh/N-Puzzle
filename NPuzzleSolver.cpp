#include "NPuzzleSolver.h"
#include "State.hpp"

static int	hammiltonFunction(const uint8_t *map, uint8_t mapSize) {
	int inversions = -1;

	for (int i = 0; i < mapSize; i++) {
		if (map[i] != (i + 1))
			inversions++;
	}
	return (inversions);
}

#define INVAL_MOVE	123456
#define SWAP(_state, _curr, _emptyPos, _newPos)	\
			switch((_newPos)) {	\
				case INVAL_MOVE:	\
					goto fail;	\
					break;		\
				default:		\
					(_state) = new State((_curr)->getMapPtr(), 0, (_curr)->getLength() + 1);	\
                    (_state)->swapPieces((_emptyPos), (_newPos)); \
					(_state)->setPrice(this->euristicFunc((_state)->getMapPtr(), State::mapSize));	\
					break;		\
			}

//todo: check for no memory alloc
State * NPuzzleSolver::doMove(const State *curr, int move, int emptyPos) {
	State	*newState = nullptr;
	const int	size = State::size;
	int	x, y, up, down, right, left;

	x = emptyPos % size;
	y = emptyPos / size;

	switch (move) {
		case UP:
			up = (y - 1 < 0) ? (INVAL_MOVE) : x + ((y - 1) * size);
			SWAP(newState, curr, emptyPos, up);
			break;
		case DOWN:
			down = (y + 1 == size) ? (INVAL_MOVE) : x + ((y + 1) * size);
			SWAP(newState, curr, emptyPos, down);
			break;
		case LEFT:
			left = (x - 1 < 0) ? (INVAL_MOVE) : (x - 1) + (y * size);
			SWAP(newState, curr, emptyPos, left);
			break;
		case RIGHT:
			right = (x + 1 == size) ? (INVAL_MOVE) : (x + 1) + (y * size);
			SWAP(newState, curr, emptyPos, right);
			break;
		default: /* Bug */
			assert(0);
	}

fail:
	return (newState);
}
#undef INVAL_MOVE
#undef SWAP

class iterator;

void    freeMem(std::priority_queue<State *, std::vector<State *>, CompareState>   *open,
				std::unordered_set<State *, HashState, EqualState>  *closed) {
    State   *curr = nullptr;

    while (!open->empty()) {
        curr = open->top();
		open->pop();
		delete curr;
    }

    curr = nullptr;
	std::unordered_set<State*, HashState, EqualState>::iterator i;
    for (i = closed->begin(); i != closed->end(); i++) {
		State *element = *i;
		delete element;
	}
}

path_t	*NPuzzleSolver::aStar(const uint8_t *map, uint8_t mapSize) {
	std::priority_queue<State *, std::vector<State *>, CompareState>   open;
	std::unordered_set<State *, HashState, EqualState>	closed;
	State	*root;
	State	*curr;
    State	*newMove;
	path_t	*path = nullptr;
	int		maxOpen = 0, emptyPos = 0;
	const uint8_t	*currMap = 0;

	State::mapSize = mapSize;
	State::size = (int)std::sqrt(mapSize);
	//todo check for mem alloc
	root = new State(map, euristicFunc(map, mapSize), 0);

	open.push(root);

	while (!open.empty()) {
		curr = open.top();
		open.pop();

		if (curr->getPrice() == 0) {
			/* create path */
            printf("\nFINISH\nmaxOpen = %d, closed.size = %zu\n", maxOpen, closed.size());
			curr->printState();
            freeMem(&open, &closed);
            delete curr;
			return (nullptr);
		}

		if (closed.find(curr) != closed.end()) {
			/* curr already exist in closed set
			 * pop another one */
			delete curr;
			continue;
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

    return (nullptr);
}

NPuzzleSolver::NPuzzleSolver(euristicFunc_e func, solver_e algo) {
	switch (func) {
		case HAMMILTON_DISTANCE:
			this->euristicFunc = hammiltonFunction;
			break;
		default:
			this->euristicFunc = hammiltonFunction;
			break;
	}

	switch (algo) {
		case ASTAR:
			this->algoFunc = &NPuzzleSolver::aStar;
			break;
		default:
			this->algoFunc = &NPuzzleSolver::aStar;
			break;
	}
}

path_t	*NPuzzleSolver::solve(const uint8_t *map, uint8_t mapSize) {
	//todo: replace nullptr for error codes.

	if (mapSize < 9 || mapSize > 100) {
		printf("This map size is invalid or not supported.\n" );
		return (nullptr);
	}

	if (map == nullptr) {
		printf("Map is null.\n");
		return (nullptr);
	}

	//todo check map (map is squared ? , map is solvable ?)
	// if (checkMap(map, mapSize) == 0) {
	// 	printf("Map is unsolvable.\n");
	// 	return (nullptr);
	// }
	return ((this->*algoFunc)(map, mapSize));
}
