cmake_minimum_required(VERSION 3.0)
project(lisp++)

set(CMAKE_CXX_FLAGS "-std=c++14")
add_definitions(-Weverything -Wno-c++98-compat -Wno-padded)	

set (src
    sexpr.cpp	
	lisp.cpp
	parser.cpp
	environment.cpp
  	main.cpp )

find_package(Boost 1.50.0 REQUIRED COMPONENTS program_options
				   log regex system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(/home/icepic/Code/spirit/include)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(lisp++ ${src})
target_link_libraries(lisp++ ${Boost_LIBRARIES})
