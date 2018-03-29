cmake_minimum_required(VERSION 3.9)

project(MSAV)

# Set Version
set(MSAV_MAJOR 1)
set(MSAV_MINOR 0)
set(MSAV_PATCH 0)
set(PROJECT_VERSION ${MSAV_MAJOR}.${MSAV_MINOR}.${MSAV_PATCH})

#MESSAGE(STATUS "BINARY dir:" ${MSAV_BINARY_DIR})
#MESSAGE(STATUS "SOURCE dir:" ${MSAV_SOURCE_DIR})

option(BUILD_EXAMPLES "Build Examples")
option(BUILD_PLAYER "Build Player")

add_subdirectory(src)
add_subdirectory(examples)