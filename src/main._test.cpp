#include <csignal>
#include <iostream>

#define THREAD_TEST

#ifdef THREAD_TEST
volatile int i = 0;

void 	signal_handler() {
	std::cout << "From thred " << boost::this_thread::get_id()
				<< ": i = " << i << std::endl;
}

void	work(int i, int id) {
	switch (i) {
		case 0: {
			std::cout << "Thread " << id << "  starts." << std::endl;
			std::signal(SIGINT, signal_handler);
			for (i = 0; i < 2147483647; i++)
				;
		}
		break;
		case 1: {
			std::cout << "Thread " << id << " starts." << std::endl;
			sleep(1);
			std::raise(SIGINT);
		}
		break;
	}
}

int main()
{
  // Установка обработчика сигнала



  std::cout << "Значение сигнала: " << gSignalStatus << '\n';
  std::cout << "Передаём сигнал " << SIGINT << '\n';
  std::raise(SIGINT);
  std::cout << "Значение сигнала: " << gSignalStatus << '\n';
}
#endif // THREAD_TEST
