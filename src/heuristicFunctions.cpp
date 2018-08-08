#include "heuristicFunctions.hpp"
#include "State.hpp"
#include <iostream>
#include <functional>

auto findIndexInMap = [](int value, const int *map, const int mapLength) {
	for (int i = 0; i < mapLength; i++)
		if (map[i] == value)
			return (i);
	return (-1);
};

int	Heuristic::misplacedTiles(const State *state) {
	const int	*finishMap = state->finishState->getMapPtr();
	const int	*map = state->getMapPtr();
	int			inversions = 0;

	for (int i = 0; i < State::mapLength; i++) {
		if (map[i] != finishMap[i])
			inversions++;
	}
	return (inversions);
}

int	Heuristic::manhattanDistance(const State *state) {
	const int	*finishMap = state->finishState->getMapPtr();
	const int	*map = state->getMapPtr();
	int			price = 0;
	int			x1, x2, y1, y2, xres, yres, j;

	for (int i = 0; i < State::mapLength; i++) {
		if (map[i]) {
			x1 = i % State::mapSize;
			y1 = i / State::mapSize;

			j = findIndexInMap(map[i], finishMap, State::mapLength);

			x2 = j % State::mapSize;
			y2 = j / State::mapSize;

			if ((xres = x1 - x2) < 0)
				xres *= -1;

			if ((yres = y1 - y2) < 0)
				yres *= -1;

			price += xres + yres;
		}
	}

	return (price);
}

int	Heuristic::linearConflicts(const State *state) {
	const int	*finishMap = state->finishState->getMapPtr();
	const int	*map = state->getMapPtr();
	int			linearConflicts = 0;

	// conflicts in rows
	for (int row = 0; row < State::mapSize; row++) {
		for (int x1 = row * State::mapSize; x1 < row + State::mapSize; x1++) {
			for (int x2 = x1 + 1; x2 < row + State::mapSize; x2++) {
				if (map[x1] != finishMap[x1] && map[x2] != finishMap[x2] &&
					(map[x1] == finishMap[x2] && map[x2] == finishMap[x1]))
							linearConflicts++;
			}
		}
	}

	// conflicts in columns
	for (int col = 0; col < State::mapSize; col++) {
		for (int y1 = col; y1 < col + State::mapSize * (State::mapSize - 1); y1 += State::mapSize) {
			for (int y2 = col + State::mapSize; y2 < col + State::mapSize * (State::mapSize - 1); y2 += State::mapSize) {
				if (map[y1] != finishMap[y1] && map[y2] != finishMap[y2] &&
					(map[y1] == finishMap[y2] && map[y2] == finishMap[y1]))
							linearConflicts++;
			}
		}
	}

	return (linearConflicts);
}

int	Heuristic::MDplusLinearConflicts(const State *state) {
	return (manhattanDistance(state) + linearConflicts(state));
}

int	Heuristic::MTplusLinearConflicts(const State *state) {
	return (misplacedTiles(state) + linearConflicts(state));
}

int	Heuristic::nMaxSwap(const State *state) {
	const int	*finishMap = state->finishState->getMapPtr();
	const int	*map = state->getMapPtr();
	int	mapCopy[State::mapLength];
	int	retVal = 0;
	int	zeroI = 0;

	for (int i = 0; i < State::mapLength; i++)
		mapCopy[i] = map[i];

	for (int i = 0; i < State::mapLength; i++) {
		if (mapCopy[i] != finishMap[i]) {
			zeroI = findIndexInMap(0, mapCopy, State::mapLength);
			if (i != zeroI) {
				retVal++;
				std::swap(mapCopy[i], mapCopy[zeroI]);
				zeroI = i;
			}
			int j = findIndexInMap(finishMap[i], mapCopy, State::mapLength);
			std::swap(mapCopy[j], mapCopy[zeroI]);
		}
	}

	return (retVal);
}
