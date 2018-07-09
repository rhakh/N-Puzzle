#include <csignal>
#include <iostream>

#include <boost/thread/thread.hpp>

#define THREAD_TEST

#ifdef THREAD_TEST
volatile int i = 0;

using th = boost::thread;

void 	signal_handler(int a) {
	std::cout << "From thred " << boost::this_thread::get_id()
				<< ": i = " << i << std::endl;
}

void	work(int i) {
	boost::thread::id	id = boost::this_thread::get_id();

	switch (i) {
		case 0: {
			std::cout << "counter: Thread " << id << "  starts." << std::endl;
			std::signal(SIGINT, signal_handler);
			for (i = 0; i < 2147483647; )
				i += 1;
			std::cout << "Thread " << id << "  end." << std::endl;
		}
		break;
		case 1: {
			std::cout << "raiser: Thread " << id << " starts." << std::endl;
			sleep(1);
			std::raise(SIGINT);
			std::cout << "Thread " << id << "  end." << std::endl;
		}
		break;
	}
}

int main()
{
  // Установка обработчика сигнала

	boost::thread	*t[2];

	t[0] = new boost::thread(work, 0);
	t[1] = new boost::thread(work, 1);

	t[0]->join();
	t[1]->join();

	delete t[0];
	delete t[1];
}
#endif // THREAD_TEST
