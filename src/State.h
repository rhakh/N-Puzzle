#ifndef N_PUZZLE_STATE_H
#define N_PUZZLE_STATE_H

#include <vector>

class State {
private:
    int cost;
    int length;
    std::vector<int>    map;
    State   *prev;

public:
    State(const st::vector<int> _map, const int _const, const int _length);
    ~State();
    void setCost(int _cost) { this->cost = _cost; }
    void setLength(int _length) { this->length = _length; }
    void incLength() { this->length++; }
    int getCost() { return this->cost; }
    int getLength() { return this->length; }
    std::vector getMap() { return map; }
    bool operator()(State *a, State *b) {
        if (a->getCost() == b->getCost())
            return a->getLength() < b->getLength();
        else
            return a->getCost() < b->getCost();
    }
};

#endif //N_PUZZLE_STATE_H
