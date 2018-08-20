#include "CSCP.hpp"
#include "main.hpp"

#define BOOST_SPIRIT_THREADSAFE

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream> //del this
#include <ctime>
#include <vector>

void	CSCP::constructTaskResponse(double elapsedTime, NP_retVal &result, std::string &resultStr)
{
	namespace pt = boost::property_tree;

	pt::ptree		taskJsonRes;
	pt::ptree		dataNode;
	pt::ptree		movesNode;

	taskJsonRes.put("messageType", NP_SOLUTION);

	for (auto const &move: result.path) {
		pt::ptree	moveElem;

		moveElem.put("", move);
		movesNode.push_back(std::make_pair("", moveElem));
	}
	dataNode.add_child("movements", movesNode);

	dataNode.put("openNodes", result.maxOpen);

	dataNode.put("closedNodes", result.closedNodes);

	dataNode.put("usedMemory", result.usedMemory);

	dataNode.put("elapsedTime", elapsedTime);

	taskJsonRes.add_child("data", dataNode);

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, taskJsonRes, false);
	resultStr = ss.str();
}

void	CSCP::constructErrorResponse(std::exception &e, std::string &resultStr) {
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

void	CSCP::taskHandler(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	pt::ptree		mapNode = json.get_child("data.map");
	pt::ptree		dataNode = json.get_child("data");
	int				map[mapNode.size()];
	int				i;
	clock_t			start;
	NP_retVal		result;

	result.maxOpen = 0;
	result.closedNodes = 0;
	result.usedMemory = 0;
	pt::ptree::iterator		it = mapNode.begin();
	for (i = 0; it != mapNode.end(); it++, i++)
		map[i] = it->second.get<int>("");

	try {
		start = clock();
		// TODO:
		// optimisationByTime = dataNode.get<int>("optimisation");
		this->solver.solve(dataNode.get<int>("heuristicFunction"),
							dataNode.get<int>("solutionType"),
							map, mapNode.size(),
							result);
		start = clock() - start;
		constructTaskResponse((double)start / CLOCKS_PER_SEC, result, resultStr);
	}
	catch (std::exception &e) {
		constructErrorResponse(e, resultStr);
	}

	if (verboseLevel & SERVER)
		std::cout << "Server send response: " << resultStr
					<< std::endl << std::flush;
}

void	CSCP::processMessage(boost::property_tree::ptree &json, std::string &resultStr) {
	namespace pt = boost::property_tree;

	int		messageType = json.get<int>("messageType");

	if (verboseLevel & SERVER) {
		std::stringstream	ss;
		boost::property_tree::json_parser::write_json(ss, json, false);
		std::cout << "Server receive request: " << ss.str() << std::endl;
	}

	switch (messageType) {
		case NP_TASK:
			try {taskHandler(json, resultStr);}
			catch (std::exception &e) {}
			break;
		default:
			CSCP_InvalidMessageType	e;
			constructErrorResponse(e, resultStr);
			break;
	}
}

void	CSCP::serverInit() {
	using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

	server.config.port = 8080;

	server.resource["^/message$"]["POST"] = [this](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		try {
			namespace pt = boost::property_tree;
			std::string	result;
			pt::ptree	json;

			read_json(request->content, json);
			processMessage(json, result);

			*response << "HTTP/1.1 200 OK\r\n"
						<< "Content-Length: " << result.length() << "\r\n\r\n"
						<< result;
		}
		catch(const std::exception &e) {
			// TODO: construct json error here
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n"
						<< e.what();
		}
	};

	server.default_resource["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		try {
			auto web_root_path = boost::filesystem::canonical("webPages");
			auto path = boost::filesystem::canonical(web_root_path / request->path);
			// Check if path is within web_root_path
			if(std::distance(web_root_path.begin(), web_root_path.end()) > std::distance(path.begin(), path.end()) ||
				!std::equal(web_root_path.begin(), web_root_path.end(), path.begin()))
				throw std::invalid_argument("path must be within root path");
			if(boost::filesystem::is_directory(path))
				path /= "index.html";

			SimpleWeb::CaseInsensitiveMultimap header;

			auto ifs = std::make_shared<std::ifstream>();
			ifs->open(path.string(), std::ifstream::in | std::ios::binary | std::ios::ate);

			if(*ifs) {
				auto length = ifs->tellg();
				ifs->seekg(0, std::ios::beg);

				header.emplace("Content-Length", to_string(length));
				response->write(header);

				// Trick to define a recursive function within this scope (for example purposes)
				class FileServer {
					public:
					static void read_and_send(const std::shared_ptr<HttpServer::Response> &response, const std::shared_ptr<std::ifstream> &ifs) {
						// Read and send 128 KB at a time
						static std::vector<char> buffer(131072); // Safe when server is running on one thread
						std::streamsize read_length;
						if((read_length = ifs->read(&buffer[0], static_cast<std::streamsize>(buffer.size())).gcount()) > 0) {
							response->write(&buffer[0], read_length);
							if(read_length == static_cast<std::streamsize>(buffer.size())) {
								response->send([response, ifs](const SimpleWeb::error_code &ec) {
									if(!ec)
										read_and_send(response, ifs);
									else
										std::cerr << "Connection interrupted" << std::endl;
								});
							}
						}
					}
				};
				FileServer::read_and_send(response, ifs);
			}
			else
				throw std::invalid_argument("could not read file");
		}
		catch(const std::exception &e) {
			response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
		}
	};

	server.on_error = [](std::shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
		// Handle errors here
		// Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
	};
}

boost::thread	*CSCP::serverStart() {
	boost::thread	*server_thread;

	server_thread = new boost::thread([this]{
		server.start();
	});

	return (server_thread);
}

CSCP::CSCP() {
	try {
		serverInit();
	}
	catch (std::exception &e) {
		std::cerr << "Unhandled exception: " << e.what() << std::endl;
	}
}

CSCP::~CSCP() {

}
