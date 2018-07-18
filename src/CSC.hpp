#ifndef CLIENT_SERVER_COMMUNICATION_PROTOCOL
#define CLIENT_SERVER_COMMUNICATION_PROTOCOL

#define BOOST_SPIRIT_THREADSAFE

#include <server_http.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>
#include "NPuzzleSolver.hpp"

typedef enum MessageType_e {
	NP_TASK,
	NP_SOLUTION,
	NP_STOP,
	NP_ERROR
} MessageType_E;

class CSC {
	SimpleWeb::Server<SimpleWeb::HTTP>	server;
	NPuzzleSolver	solver;

	void	constructTaskResponse(size_t openNodes, size_t closedNodes,
									size_t usedMemory, double elapsedTime,
									std::list<uint8_t> &result,
									std::string &resultStr);
	void	constructErrorResponse(std::exception &e, std::string &resultStr);
	void	taskHandler(boost::property_tree::ptree &json, std::string &resultStr);

	void			serverInit();

public:
	CSC();
	~CSC();

	boost::thread	*serverStart();

	void	processMessage(boost::property_tree::ptree &json, std::string &result);
};

#endif // CLIENT_SERVER_COMMUNICATION_PROTOCOL
