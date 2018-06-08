cmake_minimum_required(VERSION 2.8.4)

project(player)

# Set Version
set(PLAYER_MAJOR 1)
set(PLAYER_MINOR 0)
set(PLAYER_PATCH 0)
set(PROJECT_VERSION ${PLAYER_MAJOR}.${PLAYER_MINOR}.${PLAYER_PATCH})

# Use C++11
#if(CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
#    message(STATUS "optional:-std=c++11")
#endif(CMAKE_COMPILER_IS_GNUCXX)

set(FFMPEG_DIR "")
set(SDL_DIR "")
function(CHECK_ENV arg)
    if (${ARGC} EQUAL 1)
        set(ENV_DIR $ENV{${ARGV0}})
        if (EXISTS ${ENV_DIR})
            message(STATUS "Check env exist--> ${ENV_DIR}")
            set(${arg} ${ENV_DIR} PARENT_SCOPE)
        else()
            message(FATAL_ERROR "No ${ARGV0} exist.")
        endif()
    endif()
endfunction ()
CHECK_ENV(FFMPEG_DIR)
CHECK_ENV(SDL_DIR)
CHECK_ENV(PORTAUDIO_DIR)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

link_directories(${FFMPEG_DIR}/lib ${SDL_DIR}/lib ${PORTAUDIO_DIR}/lib)
include_directories(${FFMPEG_DIR}/include ${CMAKE_SOURCE_DIR}/src/Puff)

aux_source_directory(. DIR_SRCS)

add_executable(player ${DIR_SRCS})

target_link_libraries(player Puff)
