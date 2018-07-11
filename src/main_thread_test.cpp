#include <csignal>
#include <iostream>

#include <boost/thread/thread.hpp>

#define THREAD_TEST

#ifdef THREAD_TEST
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
