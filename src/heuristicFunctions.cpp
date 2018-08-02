#include "heuristicFunctions.hpp"
#include "State.hpp"
#include <iostream>
#include <functional>

auto findIndexInMap = [](int value, const int *map, const int mapSize) {
	for (int i = 0; i < mapSize; i++)
		if (map[i] == value)
			return (i);
	return (-1);
};

int	misplacedTiles(const int *map, const int *finishMap, const int mapSize, const int size) {
	int	inversions = 0;

	for (int i = 0; i < mapSize; i++) {
		if (map[i] != finishMap[i])
			inversions++;
	}
	return (inversions);
}

int	manhattanDistance(const int *map, const int *finishMap, const int mapSize, const int size) {
	int	price = 0;
	int	x1, x2, y1, y2, xres, yres, j;

	for (int i = 0; i < mapSize; i++) {
		if (map[i]) {
			x1 = i % size;
			y1 = i / size;

			j = findIndexInMap(map[i], finishMap, mapSize);

			x2 = j % size;
			y2 = j / size;

			if ((xres = x1 - x2) < 0)
				xres *= -1;

			if ((yres = y1 - y2) < 0)
				yres *= -1;

			price += xres + yres;
		}
	}

	return (price);
}

static int linearConflicts(const int *map, const int *finishMap, const int mapSize, const int size) {
	int	linearConflicts = 0;

	// conflicts in rows
	for (int row = 0; row < size; row++) {
		for (int x1 = row * size; x1 < row + size; x1++) {
			for (int x2 = x1 + 1; x2 < row + size; x2++) {
				if (map[x1] != finishMap[x1] && map[x2] != finishMap[x2] &&
					(map[x1] == finishMap[x2] && map[x2] == finishMap[x1]))
							linearConflicts++;
			}
		}
	}

	// conflicts in columns
	for (int col = 0; col < size; col++) {
		for (int y1 = col; y1 < col + size * (size - 1); y1 += size) {
			for (int y2 = col + size; y2 < col + size * (size - 1); y2 += size) {
				if (map[y1] != finishMap[y1] && map[y2] != finishMap[y2] &&
					(map[y1] == finishMap[y2] && map[y2] == finishMap[y1]))
							linearConflicts++;
			}
		}
	}

	return (linearConflicts);
}

int	MDplusLinearConflicts(const int *map, const int *finishMap, const int mapSize, const int size) {
	return (manhattanDistance(map, finishMap, mapSize, size) + linearConflicts(map, finishMap, mapSize, size));
}

int	MTplusLinearConflicts(const int *map, const int *finishMap, const int mapSize, const int size) {
	return (misplacedTiles(map, finishMap, mapSize, size) + linearConflicts(map, finishMap, mapSize, size));
}

int	nMaxSwap(const int *map, const int *finishMap, const int mapSize, const int size) {

	int	mapCopy[mapSize];
	int	retVal = 0;
	int	zeroI = 0;

	for (int i = 0; i < mapSize; i++)
		mapCopy[i] = map[i];

	for (int i = 0; i < mapSize; i++) {
		if (mapCopy[i] != finishMap[i]) {
			zeroI = findIndexInMap(0, mapCopy, mapSize);
			if (i != zeroI) {
				retVal++;
				std::swap(mapCopy[i], mapCopy[zeroI]);
				zeroI = i;
			}
			int j = findIndexInMap(finishMap[i], mapCopy, mapSize);
			std::swap(mapCopy[j], mapCopy[zeroI]);
		}
	}

	return (retVal);
}
