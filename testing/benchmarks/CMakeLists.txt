project(keyvector-benchmark-test-app)

add_executable(keyvector-benchmark-test main.cpp)

set_target_properties(keyvector-benchmark-test 
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../bin")

target_include_directories(keyvector-benchmark-test
#	PUBLIC
	PRIVATE
		include
                "../../include/"
		src
)
