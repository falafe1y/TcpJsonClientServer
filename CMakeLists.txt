cmake_minimum_required(VERSION 3.10)
project(KeyValueStoreTCPServer)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/json/single_include)

add_executable(server server.cpp)
target_link_libraries(server ${Boost_LIBRARIES} pthread)

add_executable(client client.cpp)
target_link_libraries(client ${Boost_LIBRARIES} pthread)
