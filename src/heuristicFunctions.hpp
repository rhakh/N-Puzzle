#ifndef N_PUZZLE_HEURISTICFUNCTIONS_HPP
#define N_PUZZLE_HEURISTICFUNCTIONS_HPP

#include <cstdint>
#include "State.hpp"

int	hammiltonDistance(const uint8_t *map, uint8_t mapSize);

int	manhattanDistance(const uint8_t *map, uint8_t mapSize);

#endif //N_PUZZLE_HEURISTICFUNCTIONS_HPP
