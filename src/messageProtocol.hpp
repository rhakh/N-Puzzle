#ifndef MESSAGE_PROTOCOL_HPP
#define MESSAGE_PROTOCOL_HPP

//#include <server_http.hpp>
#include <boost/property_tree/ptree.hpp>
#include "NPuzzleSolver.hpp"

typedef enum MessageType_e {
	NP_TASK,
	NP_SOLUTION,
	NP_STOP,
	NP_ERROR
} MessageType_E;

class MessageProtocol {
	NPuzzleSolver	solver;

	void	constructTaskResponse(size_t openNodes, size_t closedNodes,
									size_t usedMemory, double elapsedTime,
									std::list<uint8_t> &result,
									std::string &resultStr);
	void	constructErrorResponse(std::exception &e, std::string &resultStr);
	void	taskHandler(boost::property_tree::ptree &json, std::string &resultStr);
	void	stopHandler(boost::property_tree::ptree &json, std::string &resultStr);

public:
	MessageProtocol();
	~MessageProtocol();

	void	processMessage(boost::property_tree::ptree &json, std::string &result);
};

#endif
