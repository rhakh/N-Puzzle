#include "State.h"

State::State() {
	this->map = std::vector<int> (0);
	this->cost = 0;
	this->length = 0;
};

State::State(std::vector<int> _map, int _const, int _length) {
	this->map = _map;
	this->cost = _const;
	this->length = _length;
}

void	State::setCost(int _cost) {
	this->cost = _cost;
}

void	State::setLength(int _length) {
	this->length = _length;
}

void	State::incLength() {
	this->length++;
}

int		State::getCost() const {
	return this->cost;
}

int		State::getLength() const {
	return this->length;
}

void	State::printState() const {
	for (int i = 0; i < this->map.size(); i++) {
		if (i % (int)sqrt(this->map.size()) == 0)
			printf("\n");
		printf("%2d ", this->map[i]);
	}
	printf("\n");
};

void	State::setMapVal(int pos, int val) {
	this->map[pos] = val;
}

int		State::getMapVal(int pos) const {
	return (this->map[pos]);
}

std::vector<int>	State::getMap() const {
	return this->map;
}

std::vector<int>	*State::getptrMap() {
	return &map;
}

bool	State::operator<(const State &b) const {
	return this->map > b.map;
}
