cmake_minimum_required(VERSION 3.9)
project(N_Puzzle)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/boost/1.72.0_3/include)
include_directories(/usr/local/Cellar/tbb/2020_U2/include)
include_directories(/Users/rhakh/N-Puzzle/Simple-Web-Server)
link_directories(/usr/local/Cellar/boost/1.72.0_3/lib)
link_directories(/usr/local/Cellar/tbb/2020_U2/lib)

add_executable(N_Puzzle
        Simple-Web-Server/client_http.hpp
        Simple-Web-Server/client_https.hpp
        Simple-Web-Server/crypto.hpp
        Simple-Web-Server/server_http.hpp
        Simple-Web-Server/server_https.hpp
        Simple-Web-Server/status_code.hpp
        Simple-Web-Server/utility.hpp
        src/CSCP.cpp
        src/CSCP.hpp
        src/Heuristic.cpp
        src/Heuristic.hpp
        src/main.cpp
        src/main.hpp
        src/NPuzzleSolver.cpp
        src/NPuzzleSolver.hpp
        src/State.cpp
        src/State.hpp
        src/CLI.cpp
        src/CLI.hpp)

target_link_libraries(N_Puzzle boost_filesystem)
target_link_libraries(N_Puzzle boost_system-mt)
target_link_libraries(N_Puzzle boost_system)
target_link_libraries(N_Puzzle boost_thread-mt)
target_link_libraries(N_Puzzle boost_program_options)
#target_link_libraries(N_Puzzle tbb)


