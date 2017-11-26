#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "State.h"


using namespace std;

typedef enum {UP, DOWN, RIGHT, LEFT, LAST} moves_e;

bool    compareState(State a, State b) {
    if (a.getCost() == b.getCost())
        return a.getLength() < b.getLength();
    else
        return a.getCost() < b.getCost();
}

priority_queue<State, std::vector<State>, CompareState>   open;
set<State>  closed;

void 	print_queue(std::priority_queue<State, std::vector<State>, CompareState> open) {
	State curr;

	while (!open.empty()) {
		curr = open.top();
		open.pop();
		curr.printState();
		printf("cost = %d, length = %d\n", curr.getCost(), curr.getLength());
	}
}

void 	print_set(std::set<State> closed) {
	std::set<State>::iterator iter;
	State	elem;
	for (iter = closed.begin(); iter != closed.end(); iter++) {
		elem = *iter;
		elem.printState();
		printf("cost = %d, length = %d\n", elem.getCost(), elem.getLength());
	}
}

std::vector<int> etalon({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0});

int     hammiltonFunction(std::vector<int>  map) {
    int size = map.size();
    int inversions = 0;

    for (int i = 0; i < size; i++) {
        if (map[i] != etalon[i])
            inversions++;
    }
    return (inversions);
}

#define swap_int(a, b) \
	(a) = (a) ^ (b); \
	(b) = (a) ^ (b); \
	(a) = (a) ^ (b); \

State   doMove(State map, moves_e move) {
    int     pos = 0;
	int 	temp;
    int     x, y, up, down, right, left;

    for (int i = 0; i < map.getMap().size(); i++)
        if (map.getMap()[i] == 0 && (pos = i) >= 0)
            break ;
    x = pos % N_SIZE;
    y = pos / N_SIZE;
    up = x + ((y - 1) * N_SIZE);
    down = x + ((y + 1) * N_SIZE);
    right = (x + 1 == N_SIZE) ? (BIG_VAL) : ((x + 1) + (y * N_SIZE));
    left = (x - 1 < 0) ? (BIG_VAL) : ((x - 1)  + (y * N_SIZE));

//	switch (move) {
//		case UP:
//			printf("doMove UP\n");
//			break;
//		case DOWN:
//			printf("doMove DOWN\n");
//			break;
//		case LEFT:
//			printf("doMove LEFT\n");
//			break;
//		case RIGHT:
//			printf("doMove RIGHT\n");
//			break;
//	}
//	printf("pos = %d\n", pos);
//	printf("up = %d, down = %d, left = %d, right = %d\n", up, down, left, right);

    if (move == UP && up >=0 && up < DSIZE) {
		temp = map.getMap()[pos];
		map.setMapVal(pos, map.getMapVal(up));
		map.setMapVal(up, temp);
        return (map);
    }
    else if (move == DOWN && down >= 0 && down < DSIZE) {
		temp = map.getMap()[pos];
		map.setMapVal(pos, map.getMapVal(down));
		map.setMapVal(down, temp);
        return (map);
    }
    else if (move == LEFT && left >= 0 && left < DSIZE) {
		temp = map.getMap()[pos];
		map.setMapVal(pos, map.getMapVal(left));
		map.setMapVal(left, temp);
        return (map);
    }
    else if (move == RIGHT && right >=0 && right < DSIZE) {
		temp = map.getMap()[pos];
		map.setMapVal(pos, map.getMapVal(right));
		map.setMapVal(right, temp);
        return (map);
    }
    return (State());
}

State   aStar(State *start) {
    State   curr;
    State   afterMove;
    int     weight;

    open.push(*start);
//	start->printState();
    while (!open.empty()) {
//		printf("\n\n\n");
		curr = open.top();
		open.pop();

		/*check if it solved*/
		if (hammiltonFunction(curr.getMap()) == 0) {
//			printf("!!!!!!!!!! FINISH !!!!!!!!!!\n");
//			curr.printState();
			return (curr);
		}

		if (closed.find(curr) != closed.end()) {
//			printf("$$$$$$ THIS NODE EXIST $$$$$$\n");
//			curr.printState();
//			printf("$$$$$$ END $$$$$$\n");
			continue;
		}

        closed.insert(curr);

//		printf("\nopen.size = %d, closed.size = %d\n", open.size(), closed.size());

//		printf("@@@@@@@@@ CLOSED QUEUE @@@@@@@@@@@@\n");
//		print_set(closed);
//		printf("@@@@@@@@@ END @@@@@@@@@@@@\n");

//		printf("########### POP FROM QUEUE #########\n");
//		curr.printState();
//		printf("cost = %d, length = %d\n", curr.getCost(), curr.getLength());
//		printf("########### END #########\n");

//		printf("<<<<<<<<< PRIORITY QUEUE >>>>>>>>>>\n");
//		print_queue(open);
//		printf("<<<<<<<<< END >>>>>>>>>>\n");

        for (int i = UP; i < LAST; i++) {
            afterMove = doMove(curr, (moves_e)i);
            if (afterMove.getMap().empty())
                continue ;
            else {
                afterMove.incLength();
                afterMove.setCost(hammiltonFunction(afterMove.getMap()));
//				afterMove.printState();
//				printf("const = %d, length = %d\n", afterMove.getCost(), afterMove.getLength());
                open.push(afterMove);
            }
        }
    }
	return (State());
}

int     main(void) {
    std::vector<int>    map ({11,  0,  9,  4,
	2, 15,  7,  1,
	13,  3, 12, 5,
	8,  6, 10, 14});
    State   start(map, hammiltonFunction(map), 0);
	State	solved = aStar(&start);
}
