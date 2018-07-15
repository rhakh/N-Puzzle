#include "messageProtocol.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream> //del this
#include <ctime>

void	MessageProtocol::constructTaskResponse(size_t openNodes, size_t closedNodes,
								size_t usedMemory, double elapsedTime,
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

void	MessageProtocol::constructErrorResponse(std::exception &e, std::string &resultStr) {
	namespace pt = boost::property_tree;

	pt::ptree		taskJsonRes;
	pt::ptree		dataNode;

	taskJsonRes.put("messageType", NP_ERROR);

	dataNode.put("message", e.what());

	taskJsonRes.add_child("data", dataNode);

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, taskJsonRes, false);
	resultStr = ss.str();
}

void	MessageProtocol::taskHandler(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	std::tuple<size_t, size_t, size_t>	retVal;
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

	try {
		start = clock();
		retVal = this->solver.solve(dataNode.get<int>("heuristicFunction"),
							dataNode.get<int>("algorithm"),
							map, mapNode.size(),
							result);
		start = clock() - start;
		constructTaskResponse(std::get<0>(retVal), std::get<1>(retVal), std::get<2>(retVal),
								(double)start / CLOCKS_PER_SEC, result, resultStr);
	}
	catch (std::exception &e) {
		std::cout << "Error:" << __func__ << ":" << __LINE__ << ":"
					<< e.what() << std::endl;
		constructErrorResponse(e, resultStr);
	}

	std::cout << "Server resp json:" << resultStr << std::endl;
	std::cout << std::flush;
}

void	MessageProtocol::stopHandler(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

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
