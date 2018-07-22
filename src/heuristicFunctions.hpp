#ifndef N_PUZZLE_HEURISTICFUNCTIONS_HPP
#define N_PUZZLE_HEURISTICFUNCTIONS_HPP

#include <cstdint>

int	misplacedTiles(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

int	manhattanDistance(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

int	MDplusLinearConflicts(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

int	MTplusLinearConflicts(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

int	nMaxSwap(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

#endif //N_PUZZLE_HEURISTICFUNCTIONS_HPP
