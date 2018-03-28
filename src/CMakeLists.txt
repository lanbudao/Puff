set(MODULE MSAV)

#include(${PROJET_SOURCE_DIR}/MSAV)

# Check ffmpeg modules
set(AVMODULES avformat avcodec swscale avutil)
foreach(av ${AVMODULES})
    string(TOUPPER ${av} AV)
    find_file(HAVE_${AV} lib${av}/${av}.h HINTS ${EXTRA_INCLUDE} PATHS ${EXTRA_INCLUDE} CMAKE_FIND_ROOT_PATH_BOTH)
    message("HAVE_${AV}: ${HAVE_${AV}}")
    if(NOT ${HAVE_${AV}} MATCHES HAVE_${AV}-NOTFOUND)
        set(HAVE_${AV} 1)
        list(APPEND EXTRA_LIBS ${av})
        list(APPEDN EXTRA_DEFS "-DMSAV_HAVE_${AV}=1")
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

list(APPEND EXTRA_DEFS -DBUILD_QTAV_LIB -D__STDC_CONSTANT_MACROS)

file(GLOB SDK_HEADERS MSAV/*.h)
file(GLOB SDK_PRIVATE_HEADERS MSAV/private/*.h)

list(APPEND SOURCES
        AVPlayer.cpp)
list(APPEND HEADERS ${SDK_HEADERS} ${SDK_PRIVATE_HEADERS})

add_library(${MODULE} SHARED ${SOURCES} ${HEADERS})