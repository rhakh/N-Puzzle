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

#define sNAIL_TEST
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
