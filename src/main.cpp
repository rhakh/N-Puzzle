#include "main.hpp"
#include <client_http.hpp>
#include <server_http.hpp>
#include "NPuzzleSolver.hpp"
#include <array>

// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

#include "messageProtocol.hpp"

using namespace std;
// Added for the json-example:
using namespace boost::property_tree;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
MessageProtocol		mp;

void	serverInit(HttpServer &server) {
	server.config.port = 8080;

	server.resource["^/message$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		try {
			namespace pt = boost::property_tree;
			std::string	result;
			pt::ptree	json;

			read_json(request->content, json);
			mp.processMessage(json, result);
			*response << "HTTP/1.1 200 OK\r\n"
						<< "Content-Length: " << result.length() << "\r\n\r\n"
						<< result;
		}
		catch(const exception &e) {
			*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n"
						<< e.what();
		}
	};

	server.default_resource["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
	  try {
  		auto web_root_path = boost::filesystem::canonical("webPages");
  		auto path = boost::filesystem::canonical(web_root_path / request->path);
  		// Check if path is within web_root_path
  		if(distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
  		   !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
  		  throw invalid_argument("path must be within root path");
  		if(boost::filesystem::is_directory(path))
  		  path /= "index.html";

  		SimpleWeb::CaseInsensitiveMultimap header;

  		auto ifs = make_shared<ifstream>();
  		ifs->open(path.string(), ifstream::in | ios::binary | ios::ate);

  		if(*ifs) {
  		  auto length = ifs->tellg();
  		  ifs->seekg(0, ios::beg);

  		  header.emplace("Content-Length", to_string(length));
  		  response->write(header);

  		  // Trick to define a recursive function within this scope (for example purposes)
  		  class FileServer {
  		  public:
  			static void read_and_send(const shared_ptr<HttpServer::Response> &response, const shared_ptr<ifstream> &ifs) {
  			  // Read and send 128 KB at a time
  			  static vector<char> buffer(131072); // Safe when server is running on one thread
  			  streamsize read_length;
  			  if((read_length = ifs->read(&buffer[0], static_cast<streamsize>(buffer.size())).gcount()) > 0) {
  				response->write(&buffer[0], read_length);
  				if(read_length == static_cast<streamsize>(buffer.size())) {
  				  response->send([response, ifs](const SimpleWeb::error_code &ec) {
  					if(!ec)
  					  read_and_send(response, ifs);
  					else
  					  cerr << "Connection interrupted" << endl;
  				  });
  				}
  			  }
  			}
  		  };
  		  FileServer::read_and_send(response, ifs);
  		}
  		else
  		  throw invalid_argument("could not read file");
	  }
  	  catch(const exception &e) {
  		response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
	  }
	};

	server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
	  // Handle errors here
	  // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
	};
}

void	constructTaskRequest(std::string &requestStr) {
	namespace pt = boost::property_tree;
	// std::array<int, 9>		map = {{0, 3, 5, 6, 7, 1, 4, 2, 8}};
	std::array<int, 16>		map = {{11, 0, 9, 4, 2, 15, 7, 1, 13, 3, 12, 5, 8, 6, 10, 14}};
	// std::array<int, 36>		map = {{1, 14, 2, 4, 6, 18,
	// 								9, 13, 3, 17, 11, 33,
	// 								19, 7, 16, 10, 5, 12,
	// 								8, 26, 20, 15, 22, 24,
	// 								21, 31, 27, 29, 23, 30,
	// 								25, 0, 32, 28, 34, 35}};
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

	dataNode.put("algorithm", ASTAR);

	dataNode.put("heuristicFunction", HAMMILTON_DISTANCE);

	taskJson.add_child("data", dataNode);

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, taskJson, false);
	requestStr = ss.str();
}

void	constructStopRequest(std::string &requestStr) {
	namespace pt = boost::property_tree;

	pt::ptree		taskJson;
	pt::ptree		dataNode;

	taskJson.put("messageType", NP_STOP);

	taskJson.add_child("data", dataNode);

	std::stringstream	ss;
	boost::property_tree::json_parser::write_json(ss, taskJson, false);
	requestStr = ss.str();
}

#include <mutex>
std::mutex		mt;
void	clientCode() {
	HttpClient client("localhost:8080");

	// Synchronous request examples
	try {
		std::string	requestStr;

		constructTaskRequest(requestStr);
		std::cout << "Client send to server json: " << requestStr << std::endl;

		auto r2 = client.request("POST", "/message", requestStr);
		cout << "Client recv json: " << r2->content.rdbuf() << endl;
	}
	catch(const SimpleWeb::system_error &e) {
		cerr << "Client request error: " << e.what() << endl;
	}

	//Async
	// thread	stop_thread([&client]() {
	// 	mt.lock();
	// 	std::cout << "********* Sleep" << std::endl;
	// 	mt.unlock();
    //
	// 	this_thread::sleep_for(chrono::seconds(2));
    //
	// 	mt.lock();
	// 	std::cout << "********* Send stop" << std::endl;
	// 	mt.unlock();
	// 	{
	// 		std::string	requestStr;
    //
	// 		constructStopRequest(requestStr);
	// 		mt.lock();
	// 		std::cout << "Client send to server json: " << requestStr << std::endl;
	// 		mt.unlock();
	// 		client.request("POST", "/message", requestStr, [](shared_ptr<HttpClient::Response> response, const SimpleWeb::error_code &ec) {
	// 			if(!ec) {
	// 				mt.lock();
	// 				cout << "Client recv json: " << response->content.rdbuf() << endl;
	// 				mt.unlock();
	// 			}
	// 		});
	// 		client.io_service->run();
	// 	}
	// });
    //
    //
	// mt.lock();
	// std::cout << "********* Send task" << std::endl;
	// mt.unlock();
	// {
	// 	std::string	requestStr;
    //
	// 	constructTaskRequest(requestStr);
	// 	mt.lock();
	// 	std::cout << "Client send to server json: " << requestStr << std::endl;
	// 	mt.unlock();
	// 	client.request("POST", "/message", requestStr, [](shared_ptr<HttpClient::Response> response, const SimpleWeb::error_code &ec) {
	// 		if(!ec) {
	// 			mt.lock();
	// 			cout << "Client recv json: " << response->content.rdbuf() << endl;
	// 			mt.unlock();
	// 		}
	// 	});
	// 	client.io_service->run();
	// }
    //
	// stop_thread.join();
}

int		main(int argc, char ** argv) {
    HttpServer server;


    serverInit(server);

    thread server_thread([&server]() {
      // Start server
      server.start();
    });

    // Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    // Client examples
	thread	client_thread([]() {
		clientCode();
	});

	client_thread.join();
	server_thread.join();
}

/*
	Code conventions:
		1. Everything in camelCase
		2. 'using' / 'using namespace' can be applied in scope of function
		3. Short alias for this namespace -> namespace pt = boost::property_tree;
		4. Names of enumes and defines starts with 'NP_'

	TODO:
		1. Makefile for macos
		2. -std=c++11 in Makefile ?
		3. Start new thread to solve puzzle, and send signal if client
			was send a stop message
		4. Use unsigned int in everywhere in NPuzzleSolver
		5. Make an API of NPuzzleSolver, means in header files must be
			everything what needs for NPuzzleSolver
		6. Think about using enum as argument in NPuzzleSolver class
		7. Use incline function in NPuzzleSolver.cpp instead of SWAP
		8. Leaks in NPsolver
		9. Add -O2 flag to Makefile
		10. Includes must be in cpp files, not in hpp
		*11. Use multithreads to find solution in NPuzzleSolver.cpp
		12. Put to NPuzzleSolver opennodes, closednodes, momory used
		13. rvalue reference, use it to return value from function
		14. Add mutex to NPuzzleSolver: stop task only when task is running
*/
