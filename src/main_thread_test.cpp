#include <csignal>
#include <iostream>

// #define THREAD_TEST
#ifdef THREAD_TEST
#include <boost/thread/thread.hpp>

volatile int iter = 0;

using th = boost::thread;

void 	signal_handler(int a) {
	std::cout << "From thred " << boost::this_thread::get_id()
				<< ": iter = " << iter << std::endl << std::flush;
}

void	work(int a) {
	boost::thread::id	id = boost::this_thread::get_id();

	switch (a) {
		case 0: {
			std::cout << "counter: Thread " << id << "  starts." << std::endl << std::flush;
			usleep(100);
			std::signal(SIGINT, signal_handler);
			for (iter = 0; iter < 2147483647; )
				iter += 1;
			std::cout << "Thread " << id << "  end." << std::endl << std::flush;
		}
		break;
		case 1: {
			usleep(200);
			std::cout << "raiser: Thread " << id << " starts." << std::endl << std::flush;
			usleep(100);
			std::raise(SIGINT);
			std::cout << "Thread " << id << "  end." << std::endl << std::flush;
		}
		break;
	}
}

int main()
{
	boost::thread	*t[2];

	t[0] = new boost::thread(work, 0);
	t[1] = new boost::thread(work, 1);

	t[0]->join();
	t[1]->join();

	delete t[0];
	delete t[1];
}
#endif // THREAD_TEST

//#define RVALUE_TEST
#ifdef RVALUE_TEST

#include <utility>
#include <iostream>

struct Tclass {
	Tclass(int _value, int _n) {
		value = _value;
		n = _n;
	};

	Tclass(Tclass &&rhs) {
		std::cout << "move constructor was called" << std::endl;
		this->value = std::move(rhs.value);
		this->n = std::move(rhs.n);
	};

	Tclass &operator=(Tclass &&rhs) {
		std::cout << "operator 'move' was called" << std::endl;
		this->value = std::move(rhs.value);
		this->n = std::move(rhs.n);
		return *this;
	};

	~Tclass() {};
	int value;
	int n;

private:
	// to disable copy constr. and copy operator
	Tclass &operator=(const Tclass &src) {};
	Tclass(const Tclass &src) {};
};

Tclass	&&foo() {
	Tclass	a(42, 21);

	std::cout << __func__ << ": &a = " << &a
				<< " &value = " << &a.value
				<< " &n = " << &a.n
				<< ", n = " << a.n
				<< ", value = " << a.value << std::endl;

	return (std::move(a));
}

int		main() {
	Tclass &&a = foo();

	std::cout << __func__ << ": &a = " << &a
				<< " &value = " << &a.value
				<< " &n = " << &a.n
				<< ", n = " << a.n
				<< ", value = " << a.value << std::endl;

	return (0);
}

#endif // RVALUE_TEST

//#define sNAIL_TEST
#ifdef sNAIL_TEST

#include <utility>
#include <iostream>
#include <array>
#include <functional>
#include <cmath>

#define msize	16

int		main() {

	int		arr[(int)std::sqrt(msize)][(int)std::sqrt(msize)];
	int		i;

	auto	fillArr = [&arr](int c) {
		for (int i = 0; i < std::sqrt(msize); i++)
			for (int j = 0; j < std::sqrt(msize); j++)
				arr[i][j] = c;
	};

	auto	printArr = [&arr]() {
		for (int i = 0; i < std::sqrt(msize); i++)
			for (int j = 0; j < std::sqrt(msize); j++)
				(j + 1 == std::sqrt(msize)) ? std::cout << arr[i][j] << std::endl : std::cout << arr[i][j] << " ";
	};

	auto	genArr = [&arr]() {
		int	row = 0;
		int	col = 0;
		int	dx = 1;
		int	dy = 0;
		int	dirCh = 0;
		int	vis = std::sqrt(msize);

		int matr[(int)std::sqrt(msize)][(int)std::sqrt(msize)];

		for (int i = 0; i < msize; i++) {
			matr[row][col] = i + 1;
			if (i + 1 == msize)
				matr[row][col] = 0;
			vis--;
			if (vis == 0) {
				vis = (int)std::sqrt(msize) * (dirCh % 2) + (int)std::sqrt(msize) * ((dirCh + 1) % 2) - (dirCh / 2 - 1) - 2;
				int tmp = dx;
				dx = -dy;
				dy = tmp;
				dirCh++;
			}
			col += dx;
			row += dy;
		}
		for (int i = 0; i < std::sqrt(msize); i++)
			for (int j = 0; j < std::sqrt(msize); j++)
				arr[i][j] = matr[i][j];
	};

	fillArr(0);
	genArr();
	printArr();

	return (0);
}

#endif // sNAIL_TEST

#define SOLVABLE_CHECK
#ifdef SOLVABLE_CHECK

// C++ program to check if a given instance of N*N-1
// puzzle is solvable or not
#include <iostream>
#define N 3
using namespace std;

// A utility function to count inversions in given
// array 'arr[]'. Note that this function can be
// optimized to work in O(n Log n) time. The idea
// here is to keep code small and simple.
int getInvCount(int arr[])
{
    int inv_count = 0;
    for (int i = 0; i < N * N - 1; i++)
    {
        for (int j = i + 1; j < N * N; j++)
        {
            // count pairs(i, j) such that i appears
            // before j, but i > j.
            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}

// find Position of blank from bottom
int findXPosition(int puzzle[N][N])
{
    // start from bottom-right corner of matrix
    for (int i = N - 1; i >= 0; i--)
        for (int j = N - 1; j >= 0; j--)
            if (puzzle[i][j] == 0)
                return N - i;
}

// This function returns true if given
// instance of N*N - 1 puzzle is solvable
bool isSolvable(int puzzle[N][N])
{
    // Count inversions in given puzzle
    int invCount = getInvCount((int*)puzzle);

    // If grid is odd, return true if inversion
    // count is even.
    if (N & 1)
        return !(invCount & 1);

    else     // grid is even
    {
        int pos = findXPosition(puzzle);
        if (pos & 1)
            return !(invCount & 1);
        else
            return invCount & 1;
    }
}

/* Driver program to test above functions */
int main()
{

    int puzzle[N][N] =
    // {
    //     {12, 1, 10, 2},
    //     {7, 11, 4, 14},
    //     {5, 0, 9, 15}, // Value 0 is used for empty space
    //     {8, 13, 6, 3},
    // };
	// {{11, 0, 9, 4}, {2, 15, 7, 1}, {13, 3, 12, 5}, {8, 6, 10, 14}};
	// {{0, 10, 5, 7}, {11, 14, 4, 8}, {1, 2, 6, 13}, {12, 3, 15, 9}};
	// {{3, 2, 6},{1, 4, 0}, {8, 7, 5}};

    /*
    int puzzle[N][N] = {{1, 8, 2},
                    {0, 4, 3},
                    {7, 6, 5}};

    int puzzle[N][N] = {
                    {13, 2, 10, 3},
                    {1, 12, 8, 4},
                    {5, 0, 9, 6},
                    {15, 14, 11, 7},
                };

    int puzzle[N][N] = {
                    {6, 13, 7, 10},
                    {8, 9, 11, 0},
                    {15, 2, 12, 5},
                    {14, 3, 1, 4},
                };


    int puzzle[N][N] = {
                    {3, 9, 1, 15},
                    {14, 11, 4, 6},
                    {13, 0, 10, 12},
                    {2, 7, 8, 5},
                };
    */

    isSolvable(puzzle)? cout << "Solvable":
                        cout << "Not Solvable";
    return 0;
}

#endif // SOLVABLE_CHECK
