project(player)

set(EXECUTABLE_OUTPUT_PATH ./)

include_directories(${CMAKE_SOURCE_DIR}/src/MSAV)

aux_source_directory(. DIR_SRCS)

add_executable(player ${DIR_SRCS})

target_link_libraries(player MSAV)