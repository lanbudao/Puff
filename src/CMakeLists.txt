cmake_minimum_required(VERSION 2.8.4)

set(MODULE Puff)

# Use C++11
#if(CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
#    message(STATUS "optional:-std=c++11")
#endif(CMAKE_COMPILER_IS_GNUCXX)

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

function(FIND_BOOST_LIBRARY arg)
    message(${ARGV0} " " ${ARGV1})
    find_library(LIB_XX_LIBRARIES NAMES ${ARGV1} timer HINTS ${ARGV0})
    message(STATUS "find boost library: " ${LIB_XX_LIBRARIES})
    list(APPEND EXTRA_LIBS ${LIB_XX_LIBRARIES})
endfunction()

# Find boost library
#CHECK_ENV(BOOST_DIR)
#if (EXISTS ${BOOST_DIR})
#    set(BOOST_ROOT ${BOOST_DIR})
##    IF (DEBUG_mode)
##        set(Boost_DEBUG OFF)
##    ENDIF()
#    set(Boost_USE_MULTITHREADED ON)
#    if(WIN32)
#        set(Boost_USE_STATIC_LIBS ON)
#    endif(WIN32)
#    find_package(Boost)
#    message(STATUS "version: " ${Boost_VERSION})
#    if(NOT ${Boost_VERSION} LESS 106600)
#        list(APPEND EXTRA_INCLUDE ${BOOST_ROOT})
#        set(BOOST_LIBRARIES_DIR ${BOOST_ROOT}/stage/lib)
#        link_directories(${BOOST_LIBRARIES_DIR})
#        list(APPEND EXTRA_LIBS "boost_signals-mgw62-mt-x32-1_66")    #signals2
#        list(APPEND EXTRA_LIBS "boost_timer-mgw62-mt-x32-1_66")    #timer
#        list(APPEND EXTRA_LIBS "boost_system-mgw62-mt-x32-1_66")    #system
#        list(APPEND EXTRA_LIBS "boost_filesystem-mgw62-mt-x32-1_66")    #system
#        list(APPEND EXTRA_LIBS "boost_thread-mgw62-mt-x32-1_66")    #thread
#    else()
#        find_package(Boost COMPONENTS filesystem system thread timer signals REQUIRED)
#        if(NOT Boost_FOUND)
#            message(FATAL_ERROR "Boost is not found." ${Boost_INCLUDE_DIRS} ${Boost_VERSION})
#        endif()
#        list(APPEND EXTRA_INCLUDE ${Boost_INCLUDE_DIRS})
#        link_directories(${Boost_LIBRARY_DIRS})
#        list(APPEND EXTRA_LIBS ${Boost_LIBRARIES})
#    endif()
#    if(WIN32)
#        list(APPEND EXTRA_LIBS "ws2_32" "wsock32")
#    endif()
#endif()

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

# Check SDL2 modules
CHECK_ENV(SDL_DIR)
if (EXISTS ${SDL_DIR})
    list(APPEND EXTRA_INCLUDE ${SDL_DIR}/include ${SDL_DIR}/include/SDL2)
    link_directories(${SDL_DIR}/lib)
    list(APPEND EXTRA_LIBS "SDL2" "SDL2main")
endif()

# Check PortAudio modules
CHECK_ENV(PORTAUDIO_DIR)
if (EXISTS ${PORTAUDIO_DIR})
    list(APPEND EXTRA_INCLUDE ${PORTAUDIO_DIR}/include)
    link_directories(${PORTAUDIO_DIR}/lib)
    list(APPEND EXTRA_LIBS "portaudio")
endif()

list(APPEND EXTRA_DEFS -DBUILD_PUFF_LIB -D__STDC_CONSTANT_MACROS)

file(GLOB SDK_HEADERS *.h)
file(GLOB SDK_HEADERS_PUFF Puff/*.h)
file(GLOB SDK_HEADERS_RENDERER Puff/renderer/*.h)
file(GLOB SDK_HEADERS_UTIL util/*.h)
file(GLOB SDK_PRIVATE_HEADERS Puff/private/*.h)

# Load all source file without heareders
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/util SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/decoder SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/decoder/video SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/decoder/audio SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/decoder/subtitle SOURCES)
list(APPEND HEADERS ${SDK_HEADERS} ${SDK_PRIVATE_HEADERS} ${SDK_HEADERS_PUFF} ${SDK_HEADERS_UTIL} ${SDK_HEADERS_RENDERER})

# Include path
list(APPEND INCLUDE_DIR ${EXTRA_INCLUDE})
include_directories(${INCLUDE_DIR})

# Output path
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_library(${MODULE} SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(${MODULE} PUBLIC ${EXTRA_DEFS})

target_link_libraries(${MODULE} ${EXTRA_LIBS})

set_target_properties(${MODULE} PROPERTIES
        VERSION 1.0
        SOVERSION 1.0
        OUTPUT_NAME Puff)
