set(MODULE MSAV)

# Use C++11
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

list(APPEND INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MSAV)
list(APPEND INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MSAV/private)

# Check ffmpeg modules
list(APPEND EXTRA_INCLUDE ${CMAKE_SOURCE_DIR}/external/include)
list(APPEND EXTRA_LIBS ${CMAKE_LIBRARY_PATH_FLAG}${CMAKE_SOURCE_DIR}/external/lib)
set(AVMODULES avformat avcodec swscale avutil)
foreach(av ${AVMODULES})
    string(TOUPPER ${av} AV)
    find_file(HAVE_${AV} lib${av}/${av}.h HINTS ${EXTRA_INCLUDE} PATHS ${EXTRA_INCLUDE} CMAKE_FIND_ROOT_PATH_BOTH)
    message("HAVE_${AV}: ${HAVE_${AV}}")
    if(NOT ${HAVE_${AV}} MATCHES HAVE_${AV}-NOTFOUND)
        set(HAVE_${AV} 1)
        list(APPEND EXTRA_LIBS ${av})
        list(APPEND EXTRA_DEFS "-DMSAV_HAVE_${AV}=1")
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
        list(APPEND EXTRA_DEFS "-DMSAV_HAVE_${AV}=1")
        list(APPEND AVMODULES ${av})
    endif()
endforeach()

if (NOT HAVE_SWRESAMPLE)
    if (NOT HAVE_AVRESAMPLE)
        message(FATAL_ERROR "swresample or avresample is required")
    endif()
endif()

list(APPEND EXTRA_DEFS -DBUILD_MSAV_LIB -D__STDC_CONSTANT_MACROS)

file(GLOB SDK_HEADERS *.h MSAV/*.h)
file(GLOB SDK_PRIVATE_HEADERS MSAV/private/*.h)

# Load all source file without heareders
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)
list(APPEND HEADERS ${SDK_HEADERS} ${SDK_PRIVATE_HEADERS})

# Find boost library
set(BOOST_DIR $ENV{BOOST_PATH})
if (EXISTS BOOST_DIR)
    message(STATUS "Boost library path exist--> $ENV{BOOST_PATH}")
    list(APPEND INCLUDE_DIR ${BOOST_DIR})
else()
    message(FATAL_ERROR "No boost directory specified.\nPlease set environment variables of boost named 'BOOST_PATH'.")
endif()
# Include path
list(APPEND INCLUDE_DIR ${EXTRA_INCLUDE})
include_directories(${INCLUDE_DIR})
# 3rd party libaries
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/boost)

# Output path
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_library(${MODULE} SHARED ${SOURCES} ${HEADERS} AVDemuxer.cpp MSAV/AVDemuxer.h)

target_link_libraries(${MODULE} ${EXTRA_LIBS})

target_compile_definitions(${MODULE} PUBLIC ${EXTRA_DEFS})

set_target_properties(${MODULE} PROPERTIES VERSION 1.0 SOVERSION 1.0)