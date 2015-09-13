cmake_minimum_required(VERSION 3.0)
project(lisp++)

set(CMAKE_CXX_FLAGS "-std=c++14")
#add_definitions(-Weverything -Wno-c++98-compat -Wno-padded)	

set (src
    sexpr.cpp	
	lisp.cpp
	parser.cpp
	environment.cpp
	operators.cpp)

find_package(Boost 1.50.0 REQUIRED COMPONENTS program_options
				   log regex system unit_test_framework )

include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(lisp ${src})
add_executable(lisp++ main.cpp)
target_link_libraries(lisp++ lisp ${Boost_LIBRARIES})

add_definitions(-DBOOST_TEST_DYN_LINK) 
add_executable(testlisp test.cpp)
target_link_libraries(testlisp lisp ${Boost_LIBRARIES} ) 

enable_testing()
add_test (NAME TestLisp COMMAND testlisp)


