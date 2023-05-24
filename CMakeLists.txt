# CMakeList.txt : CMake project for TinyRenderer, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.14)
project ("TinyRenderer")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

# FetchContent module to express dependancy on GoogleTest
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add source to this project's executable.
add_executable (TinyRenderer "src/TinyRenderer.cpp" "src/TinyRenderer.h" "src/tgaimage.h" 
								"src/tgaimage.cpp" "src/geometry.h" "src/model.cpp" "src/model.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TinyRenderer PROPERTY CXX_STANDARD 20)
endif()

enable_testing()

add_executable(
	hello_test
	"test/hello_test.cpp" )

target_link_libraries(
	hello_test
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)