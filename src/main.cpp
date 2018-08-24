#include "main.hpp"
#include <client_http.hpp>
#include <server_http.hpp>
#include "NPuzzleSolver.hpp"
#include <array>
#include <boost/program_options.hpp>

// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <vector>

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

// *** delete this
void	constructTaskRequest(std::string &requestStr) {
	namespace pt = boost::property_tree;
	//unsolvable for snail
	// std::array<int, 9>		map = {{0, 3, 5, 6, 7, 1, 4, 2, 8}};

	// solvable for snail
//	std::array<int, 16>		map = {{11, 0, 9, 4, 2, 15, 7, 1, 13, 3, 12, 5, 8, 6, 10, 14}};

	// solvable for snail
	 std::array<int, 16>		map = {{5, 6, 11, 14, 1, 15, 3, 4, 8, 2, 10, 12, 0, 9, 7, 13}};

	// unsolvable snail, solvable for norm
	// std::array<int, 36>		map = {{1, 14, 2, 4, 6, 18,
	// 								9, 13, 3, 17, 11, 33,
	// 								19, 7, 16, 10, 5, 12,
	// 								8, 26, 20, 15, 22, 24,
	// 								21, 31, 27, 29, 23, 30,
	// 								25, 0, 32, 28, 34, 35}};
	// /* 1,14,2,4,6,18,9,13,3,17,11,33,19,7,16,10,5,12,8,26,20,15,22,24,21,31,27,29,23,30,25,0,32,28,34,35 */

	// snail ok and regular solution
	// std::array<int, 16>		map = {{12, 1, 11, 0,
	// 								3, 2, 14, 10,
	// 								15, 6, 4, 5,
	// 								9, 13, 8, 7}};

	//hard
	// std::array<int, 16>		map = {{10, 2, 14,  9,
	// 								3, 12, 11, 13,
	// 								15, 6, 7, 1,
	// 								4, 5, 0, 8}};

	//pdf
	// unsolvable 1
	// std::array<int, 16>		map = {{0, 10, 5, 7,
	// 								11, 14, 4, 8,
	// 								1, 2, 6, 13,
	// 								12, 3, 15, 9}};

	// /* 0,10,5,7,11,14,4,8,1,2,6,13,12,3,15,9 */

	// pdf 3;3
	// solved for snail, unsolvable for normal
	// std::array<int, 9>		map = {{3, 2, 6,
	// 								1, 4, 0,
	// 								8, 7, 5}};

	// /* 3,2,6,1,4,0,8,7,5 */

	// solvable for snail, but prog tells unsolvable
	// std::array<int, 9>		map = {{0, 4, 1,
	// 								5, 2, 8,
	// 								6, 3, 7}};

	// /* 0,4,1,5,2,8,6,3,7 */

	pt::ptree		taskJson;
	pt::ptree		dataNode;
	pt::ptree		mapNode;

	taskJson.put("messageType", NP_TASK);

	for (size_t i = 0; i < map.size(); i++) {
		pt::ptree	mapElem;

		mapElem.put("", map[i]);
		mapNode.push_back(std::make_pair("", mapElem));
	}
	dataNode.add_child("map", mapNode);

	dataNode.put("heuristicFunction", MANHATTAN_DISTANCE);

	dataNode.put("solutionType", SNAIL_SOLUTION);

	dataNode.put("optimisation", 1);

	taskJson.add_child("data", dataNode);

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, taskJson, false);
	requestStr = ss.str();
}

void	clientCode() {
	HttpClient client("localhost:8080");

	// Synchronous request examples
	try {
		std::string	requestStr;

		constructTaskRequest(requestStr);
		if (verboseLevel & SERVER)
			std::cout << "Client send request: " << requestStr << std::endl;

		auto r2 = client.request("POST", "/message", requestStr);
		if (verboseLevel & SERVER)
			std::cout << "Client receive response: " << r2->content.rdbuf() << endl;
	}
	catch(const SimpleWeb::system_error &e) {
		cerr << "Client request error: " << e.what() << endl;
	}
}
// *** delete this

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

		// *** this code for tests
		this_thread::sleep_for(chrono::seconds(1));
		thread	client_thread([]() {
			clientCode();
		});
		client_thread.join();
		// *** delete this

		server_thread->join();
		delete server_thread;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
