#ifndef MESSAGE_PROTOCOL_HPP
#define MESSAGE_PROTOCOL_HPP

#include "server_http.hpp"
#include <boost/property_tree/ptree.hpp>


typedef enum MessageType_e {
	TASK,
	SOLUTION,
	STOP
} MessageType_E;

void	taskHandler(boost::property_tree::ptree &json, std::string &result);

#endif
