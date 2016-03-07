cmake_minimum_required(VERSION 2.8) 
project(Infinite)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -g")

find_package(Boost 1.55.0 REQUIRED COMPONENTS system thread filesystem iostreams log log_setup)
if(Boost_FOUND)
	message(STATUS "Boost found")
	message(STATUS "BOOST_LIBRARIES: " ${Boost_LIBRARIES})
    add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

add_library (Cache RuleList.cpp Node.cpp DepGraph.cpp BucketSet.cpp TcamCache.cpp)
add_executable(CodeB CacheCode.cpp)

if(UNIX)
	target_link_libraries(
		CodeB
		Cache
		${Boost_LIBRARIES}
		pthread
		)
elseif(WIN32)
elseif(APPLE)
endif(UNIX)

message(STATUS "c++ flag : " ${CMAKE_CXX_FLAGS}) 
message(STATUS "this is source dir " ${PROJECT_SOURCE_DIR})
message(STATUS "this is binary dir " ${PROJECT_BINARY_DIR})
