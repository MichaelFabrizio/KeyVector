project(keyvector-logic-test-app)

add_executable(keyvector-logic-test main.cpp)

set_target_properties(keyvector-logic-test 
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../bin")

target_sources(keyvector-logic-test PRIVATE  "./src/logic_tests.cpp")

target_include_directories(keyvector-logic-test
#	PUBLIC
	PRIVATE
		include
                "../../include/"
		src
)
