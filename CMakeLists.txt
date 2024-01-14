cmake_minimum_required(VERSION 3.14)
project(astar)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES src/main.cpp
        src/Simulator/Simulator.cpp
        src/Simulator/Simulator.h
        src/Algorithm/Algorithm.h
        src/Algorithm/AStar/AStar.cpp
        src/Algorithm/AStar/AStar.h
)
add_executable(astar ${SOURCE_FILES})

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(astar sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

file(COPY assets DESTINATION .)
