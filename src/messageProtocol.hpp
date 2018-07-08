#ifndef MESSAGE_PROTOCOL_HPP
#define MESSAGE_PROTOCOL_HPP

#include "server_http.hpp"
#include <boost/property_tree/ptree.hpp>


typedef enum MessageType_e {
	NP_TASK,
	NP_SOLUTION,
	NP_STOP
} MessageType_E;

void	processMessage(boost::property_tree::ptree &json, std::string &result);

#endif
