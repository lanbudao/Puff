cmake_minimum_required(VERSION 3.9)

project(Puff)

# Set Version
set(PUFF_MAJOR 1)
set(PUFF_MINOR 0)
set(PUFF_PATCH 0)
set(PROJECT_VERSION ${PUFF_MAJOR}.${PUFF_MINOR}.${PUFF_PATCH})

#MESSAGE(STATUS "BINARY dir:" ${PUFF_BINARY_DIR})
#MESSAGE(STATUS "SOURCE dir:" ${PUFF_SOURCE_DIR})

option(BUILD_EXAMPLES "Build Examples")
option(BUILD_PLAYER "Build Player")

add_subdirectory(src)
add_subdirectory(examples)
