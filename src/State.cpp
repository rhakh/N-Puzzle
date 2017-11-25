#include "State.h"

State::State(const st::vector<int> _map, const int _const, const int _length) {
    this->map = _map;
    this->cost = _const;
    this->length = _length;
}