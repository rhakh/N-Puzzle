#ifndef N_PUZZLE_HEURISTICFUNCTIONS_HPP
#define N_PUZZLE_HEURISTICFUNCTIONS_HPP

#include <cstdint>
#include "State.hpp"

int	mislacedTiles(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

int	manhattanDistance(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

int	linearConflicts(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize);

#endif //N_PUZZLE_HEURISTICFUNCTIONS_HPP
