#include "messageProtocol.hpp"

#include "main.hpp"
#include <client_http.hpp>
#include <server_http.hpp>
#include "NPuzzleSolver.hpp"
#include <array>

// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>

void	MessageProtocol::constructTaskResponse(unsigned int openNodes, unsigned int closedNodes,
								unsigned int usedMemory, double elapsedTime,
								std::list<uint8_t> &result,
								std::string &resultStr) {
	namespace pt = boost::property_tree;

	pt::ptree		taskJsonRes;
	pt::ptree		dataNode;
	pt::ptree		movesNode;

	taskJsonRes.put("messageType", NP_SOLUTION);

	for (auto const &move: result) {
		pt::ptree	moveElem;

		moveElem.put("", move);
		movesNode.push_back(std::make_pair("", moveElem));
	}
	dataNode.add_child("movements", movesNode);

	dataNode.put("openNodes", openNodes);

	dataNode.put("closedNodes", closedNodes);

	dataNode.put("usedMemory", usedMemory);

	dataNode.put("elapsedTime", elapsedTime);

	taskJsonRes.add_child("data", dataNode);

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, taskJsonRes, false);
	resultStr = ss.str();
}

void	MessageProtocol::taskHandler(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	pt::ptree		mapNode = json.get_child("data.map");
	pt::ptree		dataNode = json.get_child("data");
	unsigned char	map[mapNode.size()];
	unsigned int	openNodes = 0, closedNodes = 0, usedMemory = 0;
	int				i;
	clock_t			start;
	std::list<uint8_t>	result;

	pt::ptree::iterator		it = mapNode.begin();
	for (i = 0; it != mapNode.end(); it++, i++)
		map[i] = it->second.get<int>("");

	start = clock();
	this->solver.solve(dataNode.get<int>("heuristicFunction"),
						dataNode.get<int>("algorithm"),
						map, mapNode.size(),
						result);
	start = clock() - start;

	constructTaskResponse(openNodes, closedNodes, usedMemory,
							(double)start / CLOCKS_PER_SEC, result, resultStr);

	std::cout << "Server resp json:" << resultStr << std::endl;
	std::cout << std::flush;
}

void	MessageProtocol::stopHandler(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	resultStr = "Task was stopped";
	this->solver.stopProcess();
}

void	MessageProtocol::processMessage(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	int		messageType = json.get<int>("messageType");

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, json, false);
	std::cout << "Server recv json: " << ss.str() << std::endl;

	switch (messageType) {
		case NP_TASK:
			taskHandler(json, resultStr);
			break;
		case NP_STOP:
			stopHandler(json, resultStr);
			break;
		default: {

			}
			break;
	}
}

MessageProtocol::MessageProtocol() {

}

MessageProtocol::~MessageProtocol() {

}
