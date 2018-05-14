/******************************************************************************
    PUFF:  A lightweight player based on ffmpeg and sdl2
    Copyright (C) 2012-2018 lance <theartedly@gmail.com>

*   This file is part of PUFF

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
******************************************************************************/
#ifndef PUFF_GLOBAL_H
#define PUFF_GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <functional>
#include <list>

using namespace std;

#ifdef BUILD_PUFF_STATIC
#define PU_AV_EXPORT
#else
#ifdef BUILD_PUFF_LIB
#  undef PU_AV_EXPORT
#  define PU_AV_EXPORT __declspec(dllexport)
#else
#  undef PU_AV_EXPORT
#  define PU_AV_EXPORT __declspec(dllimport)
#endif
#endif
#define PU_AV_PRIVATE_EXPORT PU_AV_EXPORT
#define PU_DECL_DEPRECATED __declspec(deprecated)

#define PU_NO_COPY final
#define PU_DECL_OVERRIDE override
#define PU_SIGNALS public
#define PU_EMIT

#ifndef NULL
#define NULL nullptr
#endif

#ifndef DISABLE_COPY
#define DISABLE_COPY(Class) \
    Class(const Class &) = delete; \
    Class &operator = (const Class &) = delete;
#endif

#define ONLY_RUN_ONES static bool run = false; if (run) return; run = true;

#define puMin(a, b) (a > b ? b : a)
#define puMax(a, b) (a > b ? a : b)

#define PU_UNUSED(X) (void)(X);

#define PUFF_MAJOR 1
#define PUFF_MINOR 0
#define PUFF_PATCH 0


#define PUFF_VERSION_MAJOR(V) ((V & 0xff0000) >> 16)
#define PUFF_VERSION_MINOR(V) ((V & 0xff00) >> 8)
#define PUFF_VERSION_PATCH(V) (V & 0xff)

#define PUFF_VERSION_CHK(major, minor, patch) \
    (((major & 0xff) << 16) | ((minor & 0xff) << 8) | (patch & 0xff))

#define PUFF_VERSION PUFF_VERSION_CHK(PUFF_MAJOR, PUFF_MINOR, PUFF_PATCH)

typedef unsigned char uchar;
typedef std::list<std::string> StringList;

namespace Puff {

enum LogLevel {
    LogOff = 0,
    LogFatal,
    LogError,
    LogWarning,
    LogDebug,
    LogAll
};

enum ColorSpace {
    ColorSpace_Unknown,
    ColorSpace_RGB,
    ColorSpace_GBR, // for planar gbr format(e.g. video from x264) used in glsl
    ColorSpace_BT601,
    ColorSpace_BT709,
    ColorSpace_XYZ
};

/**
 * @brief The ColorRange enum
 * YUV or RGB color range
 */
enum ColorRange {
    ColorRange_Unknown,
    ColorRange_Limited, // TV, MPEG
    ColorRange_Full     // PC, JPEG
};

PU_AV_EXPORT inline std::string GetVersion() {return std::string("");}

}
#endif //PUFF_GLOBAL_H
