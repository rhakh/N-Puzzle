#include "NPuzzleSolver.h"
#include "State.hpp"

static int	hammiltonFunction(const uint8_t *map, uint8_t dsize) {
	int inversions = -1;

	for (int i = 0; i < dsize; i++) {
		if (map[i] != (i + 1))
			inversions++;
	}
	return (inversions);
}

#define BIG_VAL	123456
#define SWAP(_state, _emptyPos, _newPos)	\
			switch((_newPos)) {	\
				case BIG_VAL:	\
					goto fail;	\
				default:		\
                    (_state)->swapPieces((_emptyPos), (_newPos)); \
			}

State * NPuzzleSolver::doMove(const State *curr, int move, int emptyPos) {
	State	*newState = new State(curr->getMapPtr(), 0, curr->getLength() + 1);
	int	size = State::size;
	int	x, y, up, down, right, left;

	x = emptyPos % size;
	y = emptyPos / size;
	up = (y - 1 < 0) ? (BIG_VAL) : x + ((y - 1) * size);
	down = (y + 1 == size) ? (BIG_VAL) : x + ((y + 1) * size);
	right = (x + 1 == size) ? (BIG_VAL) : (x + 1) + (y * size);
	left = (x - 1< 0) ? (BIG_VAL) : (x - 1) + (y * size);

	switch (move) {
		case UP:
			SWAP(newState, emptyPos, up);
			break;
		case DOWN:
			SWAP(newState, emptyPos, down);
			break;
		case LEFT:
			SWAP(newState, emptyPos, left);
			break;
		case RIGHT:
			SWAP(newState, emptyPos, right);
			break;
		default: /* Bug */
			assert(0);
	}

	newState->setPrice(this->euristicFunc(newState->getMapPtr(), State::dsize));

	return (newState);
fail:
	delete newState;
	return (nullptr);
}
#undef BIG_VAL
#undef SWAP

class iterator;

void    freeMem(std::priority_queue<State *, std::vector<State *>, CompareState>   *open,
				std::unordered_set<State *, HashState, EqualState>  *closed) {
    State   *curr = nullptr;

    while (!open->empty()) {
        curr = open->top();
		open->pop();
    }

    curr = nullptr;
    for (std::unordered_set<State*>::iterator i = closed->begin(); i != closed->end(); i++) {
		State *element = *i;
		delete element;
	}
}

path_t	*NPuzzleSolver::aStar(State *root) {
	std::priority_queue<State *, std::vector<State *>, CompareState>   open;
	std::unordered_set<State *, HashState, EqualState>  closed;
    State  *curr;
    State  *newMove;
	path_t	*path = nullptr;
	int		maxOpen = 0, emptyPos = 0;
	const uint8_t	*currMap = 0;	

    root->setLength(1);
	root->setPrice(euristicFunc(root->getMapPtr(), State::dsize));
	open.push(root);

	while (!open.empty()) {
		curr = open.top();
		open.pop();

		if (curr->getPrice() == 0) {
			/* create path */
            printf("\nFINISH\n");
			curr->printState();
            freeMem(&open, &closed);
			return (new path_t());
		}

		if (closed.find(curr) != closed.end()) {
			/* curr already exist in closed set
			 * pop another one */
			printf("\nEXIST\n");
			delete curr;
			continue;
		}

		closed.insert(curr);
		currMap = curr->getMapPtr();

		for (emptyPos = 0; emptyPos < State::dsize; emptyPos++)
			if (currMap[emptyPos] == 0)
				break;

		for (int move = UP; move < LAST; move++) {
			newMove = doMove(curr, move, emptyPos);
            if (newMove != nullptr) {
                open.push(newMove);
            }
		}

		printf("\nPOP\n");
		curr->printState();

		if (open.size() > maxOpen)
			maxOpen = open.size();
	}

    return (nullptr);
}

NPuzzleSolver::NPuzzleSolver(euristicFunc_e func) {
	switch (func) {
		case HAMILTON_DISTANCE:
			this->euristicFunc = hammiltonFunction;
			break;
		default:
			printf("This euristic function not implemented yet."
					" Hamilton distance will be used.\n");
			this->euristicFunc = hammiltonFunction;
	}
}

path_t	*NPuzzleSolver::solve(State *root, solver_e algo) {
	switch (algo) {
		case ASTAR:
			return (aStar(root));
		default:
			return (aStar(root));
	}
}

