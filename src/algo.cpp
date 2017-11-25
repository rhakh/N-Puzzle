#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "State.h"

#define N_SIZE 4
#define DSIZE 16
#defien BIGVAL 6876135468
using namespace std;

typedef enum {UP, DOWN, RIGHT, LEFT, LAST} moves_e;

priority_queue<State>   open;
set<State>  closed;

int     hammiltonFunction(std::vector<int>  *map) {
    int size = map->size();
    int inversions = 0;

    for (int i = 0; i < size; i++) {
        if (map[i] != i)
            inversions++;
    }
    return (inversions);
}

#define swap_int(a, b) \
	(a) = (a) ^ (b); \
	(b) = (a) ^ (b); \
	(a) = (a) ^ (b); \

State   *doMove(State map, moves_e move) {
    int     pos;
    int     x, y, up, down, right, left;

    for (int i = 0; i < map->getMap().size(); i++)
        if (map->getMap()[i] == 0 && pos = i)
            break ;
    x = pos % DSIZE;
    y = pos / N_SIZE;
    up = x + ((y - 1) * N_SIZE);
    down = x + ((y + 1) * N_SIZE);
    right = (x + 1 == N_SIZE) ? (BIG_VAL) : ((x + 1) + (y * N_SIZE));
    left = (x - 1 < 0) ? (BIG_VAL) : ((x - 1)  + (y * N_SIZE));
    if (move == UP && up >=0 && up < DSIZE) {
        swap_int(map->getMap()[pos], map->getMap()[up]);
        return (map);
    }
    else if (move == DOWN && down >= 0 && down < DSIZE) {
        swap_int(map->getMap()[pos], map->getMap()[down]);
        return (map);
    }
    else if (mode == LEFT && left >= 0 && left < DSIZE) {
        swap_int(map->getMap()[pos], map->getMap()[left]);
        return (map);
    }
    else if (mode == RIGHT && right >=0 && right < DSIZE) {
        swap_int(map->getMap()[pos], map->getMap()[right]);
        return (map);
    }
    return (NULL);
}

State   *Astar(State *start) {
    State   *curr;
    State   *afterMove;
    int     weight;

    open.push(start);
    while (!open.empty()) {
        curr = open.pop();
        if (hammiltonFunction(curr) == 0)
            return (curr);

        closed.insert(curr);

        for (int i = 0; i < LAST; i++) {
            afterMove = doMove(*curr, i);
            if (afterMove == NULL)
                continue ;
            else {
                afterMove->incLength();
                afterMove->setCost(hammiltonFunction(afterMove->getMap()));
                open.push(afterMove);
            }

            weight = afterMove->getCost() + afterMove->getLength();

        }
    }
}

int     main(void) {
    std::vector<int>    map(
            1, 0, 5, 2,
            11,  4, 7,  8,
            3, 10,  9, 14,
            13,  6, 12, 15);
    State   start(map, hammiltonFunction(&map), 0);

}
