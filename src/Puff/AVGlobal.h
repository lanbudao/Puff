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

typedef unsigned char uchar;
typedef std::string String;
typedef std::list<String> StringList;

namespace Puff {

enum LogLevel {
    LogOff = 0,
    LogFatal,
    LogError,
    LogWarning,
    LogDebug,
    LogAll
};

PU_AV_EXPORT inline String GetVersion() {return String("");}

}
#endif //PUFF_GLOBAL_H
