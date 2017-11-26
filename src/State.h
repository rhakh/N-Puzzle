#ifndef N_PUZZLE_STATE_H
#define N_PUZZLE_STATE_H

#include <vector>
#include <set>

#define N_SIZE 4
#define DSIZE 16
#define BIG_VAL 68761468

class State {
private:
    int cost;
    int length;
    std::vector<int>    map;
    State   *prev;

public:
    State();
    State(std::vector<int> _map, int _const, int _length);
    virtual	~State();
    void	setCost(int _cost) { this->cost = _cost; }
    void	setLength(int _length) { this->length = _length; }
    void	incLength() { this->length++; }
    int		getCost() const { return this->cost; }
    int		getLength() const { return this->length; }
	void 	printState() const;
	void 	setMapVal(int pos, int val) { this->map[pos] = val; }
	int 	getMapVal(int pos) { return (this->map[pos]); }
	std::vector<int> getMap() { return this->map; }
	bool	operator<(const State &b) const {
		return this->map < b.map;
	}
};


struct CompareState {
public:
    bool operator()(const State &a, const State &b) {
        if (a.getCost() == b.getCost())
            return a.getLength() > b.getLength();
        else
            return a.getCost() > b.getCost();
    }
};

#endif //N_PUZZLE_STATE_H
