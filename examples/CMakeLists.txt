project(player)

# Use C++11
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(BOOST_DIR "")
set(FFMPEG_DIR "")
function(CHECK_ENV arg)
    if (${ARGC} EQUAL 1)
        set(ENV_DIR $ENV{${ARGV0}})
        if (EXISTS ${ENV_DIR})
            message(STATUS "Check env exist--> ${ENV_DIR}")
            include_directories(${ENV_DIR})
            set(${arg} ${ENV_DIR} PARENT_SCOPE)
        else()
            message(FATAL_ERROR "No ${ARGV0} exist.")
        endif()
    endif()
endfunction ()
CHECK_ENV(BOOST_DIR)
CHECK_ENV(FFMPEG_DIR)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(${BOOST_DIR}/boost)
include_directories(${FFMPEG_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/MSAV)

aux_source_directory(. DIR_SRCS)

add_executable(player ${DIR_SRCS})

target_link_libraries(player MSAV)