cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lockfree-ring-queue CXX)

option(LOCKFREE_RING_QUEUE_BUILD_TESTS "build test cases" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lockfree-ring-queue INTERFACE)
target_include_directories(lockfree-ring-queue INTERFACE include/)

if (${LOCKFREE_RING_QUEUE_BUILD_TESTS})
    file(GLOB test-sources tests/*.cpp)
    foreach(test-source ${test-sources})
        cmake_path(GET test-source STEM test-name)
        add_executable(test-${test-name} ${test-source})
        target_link_libraries(test-${test-name} PRIVATE lockfree-ring-queue)
    endforeach()
endif()
