#ifndef N_PUZZLE_HEURISTICFUNCTIONS_HPP
#define N_PUZZLE_HEURISTICFUNCTIONS_HPP

#include <cstdint>

int	misplacedTiles(const int *map, const int *finishMap, int mapSize);

int	manhattanDistance(const int *map, const int *finishMap, int mapSize);

int	MDplusLinearConflicts(const int *map, const int *finishMap, int mapSize);

int	MTplusLinearConflicts(const int *map, const int *finishMap, int mapSize);

int	nMaxSwap(const int *map, const int *finishMap, int mapSize);

#endif //N_PUZZLE_HEURISTICFUNCTIONS_HPP
