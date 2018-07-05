#include "heuristicFunctions.hpp"

int	hammiltonDistance(const uint8_t *map, uint8_t mapSize) {
	int inversions = -1;

	for (int i = 0; i < mapSize; i++) {
		if (map[i] != (i + 1))
			inversions++;
	}
	return (inversions);
}

int	manhattanDistance(const uint8_t *map, uint8_t mapSize) {
	int	price = 0;
	uint8_t	x1, x2, y1, y2, xres, yres;

	for (int i = 0; i < mapSize; i++) {
		if (map[i]) {
			x2 = (map[i] - 1) % State::size;
			y2 = (map[i] - 1) / State::size;

			x1 = i % State::size;
			y1 = i / State::size;

			if ((int8_t)(xres = x1 - x2) < 0)
				xres *= -1;

			if ((int8_t)(yres = y1 - y2) < 0)
				yres *= -1;

			price += xres + yres;
		}
	}

	return (price);
}
