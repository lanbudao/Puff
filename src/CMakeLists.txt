cmake_minimum_required(VERSION 3.9)

set(MODULE Puff)

# Use C++11
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

list(APPEND INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Puff
        ${CMAKE_CURRENT_SOURCE_DIR}/Puff/private
        ${CMAKE_CURRENT_SOURCE_DIR}/util)

set(BOOST_DIR "")
set(FFMPEG_DIR "")
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

# Find boost library
CHECK_ENV(BOOST_DIR)
if (EXISTS ${BOOST_DIR})
    list(APPEND EXTRA_INCLUDE ${BOOST_DIR})
    list(APPEND EXTRA_INCLUDE ${BOOST_DIR}/boost)
    link_directories(${BOOST_DIR}/stage/lib)
    list(APPEND EXTRA_LIBS "boost_signals-mgw62-mt-x32-1_66")    #signals2
    list(APPEND EXTRA_LIBS "boost_timer-mgw62-mt-x32-1_66")    #timer
    list(APPEND EXTRA_LIBS "boost_system-mgw62-mt-x32-1_66")    #system
    list(APPEND EXTRA_LIBS "boost_thread-mgw62-mt-x32-1_66")    #thread
    if(WIN32)
        list(APPEND EXTRA_LIBS "ws2_32")
    endif()
endif()

# Check ffmpeg modules
CHECK_ENV(FFMPEG_DIR)
if (EXISTS ${FFMPEG_DIR})
    list(APPEND EXTRA_INCLUDE ${FFMPEG_DIR}/include)
    link_directories(${FFMPEG_DIR}/lib)
    set(AVMODULES avformat avcodec swscale avutil)
    foreach(av ${AVMODULES})
        string(TOUPPER ${av} AV)
        find_file(HAVE_${AV} lib${av}/${av}.h HINTS ${EXTRA_INCLUDE} PATHS ${EXTRA_INCLUDE} CMAKE_FIND_ROOT_PATH_BOTH)
        message("HAVE_${AV}: ${HAVE_${AV}}")
        if(NOT ${HAVE_${AV}} MATCHES HAVE_${AV}-NOTFOUND)
            set(HAVE_${AV} 1)
            list(APPEND EXTRA_LIBS ${av})
            list(APPEND EXTRA_DEFS "-DPUFF_HAVE_${AV}=1")
        else()
            message(FATAL_ERROR "${av} is required")
        endif()
    endforeach()

    foreach(av avfilter avdevice avresample swresample)
        string(TOUPPER ${av} AV)
        find_file(HAVE_${AV} lib${av}/${av}.h HINTS ${EXTRA_INCLUDE} PATHS ${EXTRA_INCLUDE} CMAKE_FIND_ROOT_PATH_BOTH)
        message("HAVE_${AV}: ${HAVE_${AV}}")
        if(NOT ${HAVE_${AV}} MATCHES HAVE_${AV}-NOTFOUND)
            set(HAVE_${AV} 1)
            list(APPEND EXTRA_LIBS ${av})
            list(APPEND EXTRA_DEFS "-DPUFF_HAVE_${AV}=1")
            list(APPEND AVMODULES ${av})
        endif()
    endforeach()

    if (NOT HAVE_SWRESAMPLE)
        if (NOT HAVE_AVRESAMPLE)
            message(FATAL_ERROR "swresample or avresample is required")
        endif()
    endif()
endif()

list(APPEND EXTRA_DEFS -DBUILD_PUFF_LIB -D__STDC_CONSTANT_MACROS)

file(GLOB SDK_HEADERS *.h *.h)
file(GLOB SDK_HEADERS *.h Puff/*.h)
file(GLOB SDK_HEADERS *.h util/*.h)
file(GLOB SDK_PRIVATE_HEADERS Puff/private/*.h)

# Load all source file without heareders
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/util SOURCES)
list(APPEND HEADERS ${SDK_HEADERS} ${SDK_PRIVATE_HEADERS})

# Include path
list(APPEND INCLUDE_DIR ${EXTRA_INCLUDE})
include_directories(${INCLUDE_DIR})

# Output path
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_library(${MODULE} SHARED ${SOURCES} ${HEADERS} AVDemuxer.cpp Puff/AVDemuxer.h AVError.cpp Puff/AVError.h)

target_compile_definitions(${MODULE} PUBLIC ${EXTRA_DEFS})

target_link_libraries(${MODULE} ${EXTRA_LIBS})

set_target_properties(${MODULE} PROPERTIES
        VERSION 1.0
        SOVERSION 1.0
        OUTPUT_NAME puff)