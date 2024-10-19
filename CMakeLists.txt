cmake_minimum_required(VERSION 3.22.1)
project(stl-compatible-container)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Werror -Wall -pedantic -fsanitize=address)
add_link_options(-fsanitize=address)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(lib)

add_executable(${PROJECT_NAME} bin/main.cc)
target_link_libraries(${PROJECT_NAME} set)

