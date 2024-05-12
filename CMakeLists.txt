cmake_minimum_required(VERSION 3.10)
project(BoschTestQueue)

set(CMAKE_CXX_STANDARD 11)

# Add executable for the main code
include_directories(include)
add_executable(main_code src/main.cpp)


# Unit tests example for GTests: https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(code_tests tests/main_tests.cpp)
target_link_libraries(code_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(code_tests)