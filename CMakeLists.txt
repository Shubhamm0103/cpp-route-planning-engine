cmake_minimum_required(VERSION 3.20)
project(RoutePlanningEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(route_planner
    src/main.cpp
    src/Graph.cpp
)

target_include_directories(route_planner PRIVATE include)

add_executable(run_tests
    tests/test_graph.cpp
    src/Graph.cpp
)
target_include_directories(run_tests PRIVATE include)

add_executable(run_benchmark
    benchmarks/benchmark.cpp
    src/Graph.cpp
)
target_include_directories(run_benchmark PRIVATE include)