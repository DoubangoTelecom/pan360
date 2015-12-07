/* Copyright (C) 2016 Doubango Telecom <https://www.doubango.org>
*
* This file is part of Open Source Panorama 360 (a.k.a pan360) project.
* Source code hosted at https://github.com/DoubangoTelecom/pan360
* Website hosted at http://pan360.org
*
* PAN360 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* PAN360 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with PAN360.
*/
#include "pan360/p360_globals.h"
#include "pan360/p360_image.h"
#include "pan360/p360_gl_headers.h"
#include "pan360/p360_debug.h"

bool P360Globals::s_bInitialized = false;

P360_ERROR_CODE P360Globals::init()
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;

    if (P360Globals::s_bInitialized) {
        return P360_ERROR_CODE_S_OK;
    }

    P360_DEBUG_INFO("Initializing engine (v %s)...", P360_VERSION_STRING);
#if defined(HAVE_GL_GLEW_H)
    GLenum glewErr = glewInit();
    if (GLEW_OK != glewErr) {
        P360_DEBUG_ERROR("Initializing GLEW failed (%d): %s", glewErr, glewGetErrorString(glewErr));
        return P360_ERROR_CODE_E_GLEW;
    }
#endif

    P360_CHECK_CODE_BAIL(err_ = P360ImageDecoder::init());

    P360_DEBUG_INFO("Engine initialized");

bail:
    P360Globals::s_bInitialized = P360_ERROR_CODE_IS_OK(err_);
    return err_;
}

bool P360Globals::isInitialized()
{
    return s_bInitialized;
}

// Private function used as extern
const char* P360GetErrorString(P360_ERROR_CODE code)
{
    return P360Globals::getErrorString(code);
}

const char* P360Globals::getErrorString(P360_ERROR_CODE code)
{
    switch (code) {
    case P360_ERROR_CODE_S_OK:
        return "Success";
    case P360_ERROR_CODE_E_NOT_INITIALIZED:
        return "Not initialized";
    case P360_ERROR_CODE_E_INVALID_PARAMETER:
        return "Invalid parameter";
    case P360_ERROR_CODE_E_INVALID_STATE:
        return "Invalid state";
    default:
        return "Unknown code";
    }
}