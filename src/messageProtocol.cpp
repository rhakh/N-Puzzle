#include "messageProtocol.hpp"

#include "main.hpp"
#include "client_http.hpp"
#include "server_http.hpp"
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

void	taskHandler(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	pt::ptree		mapNode = json.get_child("data.map");
	pt::ptree		dataNode = json.get_child("data");
	NPuzzleSolver	solver;
	unsigned char	map[mapNode.size()];
	int				i;
	clock_t			start;
	std::list<uint8_t>	result;

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, json);
	std::cout << "JSON: " << ss.str() << std::endl;

	pt::ptree::iterator		it = mapNode.begin();
	for (i = 0; it != mapNode.end(); it++, i++)
		map[i] = it->second.get<int>("");

	start = clock();
	solver.solve(dataNode.get<int>("heuristicFunction"),
					dataNode.get<int>("algorithm"),
					map, mapNode.size(),
					result);
	start = clock() - start;

	std::cout << "Elapsed time: " << start / CLOCKS_PER_SEC << " in sec." << std::endl;
	for (auto const &move: result) {
		std::cout << (char)(move + '0') << ", ";
	}
}

void	stopHandler(boost::property_tree::ptree &json, std::string &result) {
	namespace pt = boost::property_tree;


}

void	processMessage(boost::property_tree::ptree &json, std::string &result) {
	namespace pt = boost::property_tree;

	int		messageType = json.get<int>("messageType");

	switch (messageType) {
		case NP_TASK:
			taskHandler(json, result);
			break;
		case NP_STOP:
			stopHandler(json, result);
			break;
		default: {

			}
			break;
	}
}
