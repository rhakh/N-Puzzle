#include "main.hpp"
#include <client_http.hpp>
#include <server_http.hpp>
#include "NPuzzleSolver.hpp"
#include <array>
#include <boost/program_options.hpp>

// for SIGFAULT
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <streambuf>
#include <fstream>
#include <string>

#include "CSCP.hpp"
#include "CLI.hpp"

std::string	fileName;
int	verboseLevel = 0;
int	optimisationByTime = 1;

using namespace std;
// Added for the json-example:
using namespace boost::property_tree;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

void sigFaultHandler(int sig) {
	void *array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

int		main(int argc, char **argv) {
	try {
		boost::thread	*server_thread;
		CLI				cli(argc, argv);
		CSCP			mp;

		signal(SIGSEGV, sigFaultHandler);

		if (cli.isFlagSet("help"))
			return (0);
		if (cli.isFlagSet("file")) {
			cli.startLogic();
			return (0);
		}

		server_thread = mp.serverStart();
		std::cout << "Open browser page at address http://localhost:8080" << std::endl;

		server_thread->join();
		delete server_thread;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
