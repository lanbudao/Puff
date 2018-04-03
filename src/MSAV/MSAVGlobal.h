/******************************************************************************
    MSAV:  A lightweight player based on ffmpeg and sdl2
    Copyright (C) 2012-2018 lance <theartedly@gmail.com>

*   This file is part of MSAV

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
******************************************************************************/
#ifndef MSAV_GLOBAL_H
#define MSAV_GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <functional>

using namespace std;

#ifdef BUILD_MSAV_STATIC
#define MS_AV_EXPORT
#else
#ifdef BUILD_MSAV_LIB
#  undef MS_AV_EXPORT
#  define MS_AV_EXPORT __declspec(dllexport)
#else
#  undef MS_AV_EXPORT
#  define MS_AV_EXPORT __declspec(dllimport)
#endif
#endif
#define MS_AV_PRIVATE_EXPORT MS_AV_EXPORT
#define MS_DECL_DEPRECATED __declspec(deprecated)

typedef std::string String;

namespace MSAV {

enum LogLevel {
    LogOff = 0,
    LogFatal,
    LogError,
    LogWarning,
    LogDebug,
    LogAll
};

MS_AV_EXPORT inline String GetVersion() {return String("");}

}
#endif //MSAV_GLOBAL_H
