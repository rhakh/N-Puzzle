#include "heuristicFunctions.hpp"
#include <iostream>

int	mislacedTiles(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize) {
	int inversions = 0;

	for (int i = 0; i < mapSize; i++) {
		if (map[i] != finishMap[i])
			inversions++;
	}
	return (inversions);
}

int	manhattanDistance(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize) {
	int	price = 0;
	uint8_t	x1, x2, y1, y2, xres, yres;

	for (int i = 0; i < mapSize; i++) {
		if (map[i]) {
			x2 = (map[i] - 1) % State::size;
			y2 = (map[i] - 1) / State::size;

			x1 = (finishMap[i] - 1) % State::size;
			y1 = (finishMap[i] - 1) / State::size;

			if ((int8_t)(xres = x1 - x2) < 0)
				xres *= -1;

			if ((int8_t)(yres = y1 - y2) < 0)
				yres *= -1;

			price += xres + yres;
		}
	}

	return (price);
}

int	linearConflicts(const uint8_t *map, const uint8_t *finishMap, uint8_t mapSize) {
	uint8_t	x1, x2, y1, y2, fx1, fx2, fy1, fy2;
	uint8_t	linearConflicts = 0;

	for (int i = 0; i < mapSize; i++) {
		for (int j = i + 1; j < mapSize; j++) {
			x1 = (map[i] - 1) % State::size;
			y1 = (map[i] - 1) / State::size;

			x2 = (map[j] - 1) % State::size;
			y2 = (map[j] - 1) / State::size;

			fx1 = (finishMap[i] - 1) % State::size;
			fy1 = (finishMap[i] - 1) / State::size;

			fx2 = (finishMap[j] - 1) % State::size;
			fy2 = (finishMap[j] - 1) / State::size;

			if (x1 == x2 && x1 == fx1 && x1 == fx2 &&
					(map[i] != finishMap[i] || map[j] != finishMap[j]))
				linearConflicts++;
			else if (y1 == y2 && y1 == fy1 && y1 == fy2 &&
					(map[i] != finishMap[i] || map[j] != finishMap[j]))
				linearConflicts++;
		}
	}

	return (linearConflicts);
}
