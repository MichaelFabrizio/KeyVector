cmake_minimum_required(VERSION 3.20)

project(keyvector-logic-test-app)

add_executable(keyvector-logic-test main.cpp)

target_sources(keyvector-logic-test PRIVATE  "./src/logic_tests.cpp")

target_include_directories(keyvector-logic-test
#	PUBLIC
	PRIVATE
		include
                "../include/"
		src
)
