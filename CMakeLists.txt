cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_CXX_STANDARD 17)

project(Minesweeper)
add_executable(Minesweeper)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

file(GLOB_RECURSE MINESWEEPER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/headers")
target_sources(Minesweeper PUBLIC "${MINESWEEPER_SOURCES}")