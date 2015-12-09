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
#if !defined(_PAN360_COMMON_H_)
#define _PAN360_COMMON_H_

#include "pan360/p360_config.h"

#define P360_CAT_(A, B) A ## B
#define P360_CAT(A, B) P360_CAT_(A, B)
#define P360_STRING_(A) #A
#define P360_STRING(A) P360_STRING_(A)

#define P360_VERSION_MAJOR 1
#define P360_VERSION_MINOR 0
#define P360_VERSION_MICRO 0
#if !defined(P360_VERSION_STRING)
#	define P360_VERSION_STRING P360_STRING(P360_CAT(P360_VERSION_MAJOR, .)) P360_STRING(P360_CAT(P360_VERSION_MINOR, .)) P360_STRING(P360_VERSION_MICRO)
#endif

#if !defined(P360_SAFE_DELETE_CPP)
#	define P360_SAFE_DELETE_CPP(cpp_obj) if(cpp_obj) delete (cpp_obj), (cpp_obj) = NULL;
#endif /* P360_SAFE_DELETE_CPP */

#define P360_ASSERT(x) { \
	bool __b_ret = (x); \
	assert(__b_ret); \
}

typedef enum _P360_DEBUG_LEVEL {
	P360_DEBUG_LEVEL_INFO = 4,
	P360_DEBUG_LEVEL_WARN = 3,
	P360_DEBUG_LEVEL_ERROR = 2,
	P360_DEBUG_LEVEL_FATAL = 1,
}
P360_DEBUG_LEVEL;

#define kErrorCodeSuccessStart		0
#define kErrorCodeWarnStart			10000
#define kErrorCodeErrorStart		(kErrorCodeWarnStart << 1)
#define kErrorCodeFatalStart		(kErrorCodeErrorStart << 1)

// TODO(dmi) complete P360GetErrorString(code) with all the newly added codes
typedef enum _P360_ERROR_CODE {
	P360_ERROR_CODE_S_OK = kErrorCodeSuccessStart,

	P360_ERROR_CODE_W = kErrorCodeWarnStart,

	P360_ERROR_CODE_E = kErrorCodeErrorStart,
	P360_ERROR_CODE_E_NOT_IMPLEMENTED,
	P360_ERROR_CODE_E_NOT_INITIALIZED,
	P360_ERROR_CODE_E_INVALID_STATE,
	P360_ERROR_CODE_E_INVALID_PARAMETER,
	P360_ERROR_CODE_E_INVALID_PIXEL_FORMAT,
	P360_ERROR_CODE_E_INVALID_IMAGE_FORMAT,
	P360_ERROR_CODE_E_FAILED_TO_OPEN_FILE,
	P360_ERROR_CODE_E_FAILED_TO_READ_FILE,
	P360_ERROR_CODE_E_OUT_OF_MEMORY,
	P360_ERROR_CODE_E_DECODER_NOT_FOUND,
	P360_ERROR_CODE_E_FILE_NOT_FOUND,
	P360_ERROR_CODE_E_SYSTEM,
	P360_ERROR_CODE_E_GL,
	P360_ERROR_CODE_E_GLEW,

	P360_ERROR_CODE_F = kErrorCodeFatalStart,
}
P360_ERROR_CODE;

#define P360_ERROR_CODE_IS_SUCCESS(code_) ((code_) < kErrorCodeWarnStart)
#define P360_ERROR_CODE_IS_OK(code_) P360_ERROR_CODE_IS_SUCCESS((code_))
#define P360_ERROR_CODE_IS_FAILURE(code_) (!P360_ERROR_CODE_IS_SUCCESS((code_)))
#define P360_ERROR_CODE_IS_NOK(code_) P360_ERROR_CODE_IS_FAILURE((code_))
#define P360_ERROR_CODE_IS_WARN(code_) ((code_) >= kErrorCodeWarnStart && (code_) < kErrorCodeErrorStart)
#define P360_ERROR_CODE_IS_ERROR(code_) ((code_) >= kErrorCodeErrorStart && (code_) < kErrorCodeFatalStart)
#define P360_ERROR_CODE_IS_FATAL(code_) ((code_) >= kErrorCodeFatalStart)

// In P360_CHECK_HR(x) When (x) is a function it will be executed twice when used in "P360_DEBUG_ERROR(x)" and "If(x)"
extern const char* P360GetErrorString(P360_ERROR_CODE code);
#define P360_CHECK_CODE_BAIL(x) { P360_ERROR_CODE __code__ = (x); if (P360_ERROR_CODE_IS_NOK(__code__)) { P360_DEBUG_ERROR("Operation Failed (%s)", P360GetErrorString(__code__)); goto bail; } }
#define P360_CHECK_CODE_RETURN(x) { P360_ERROR_CODE __code__ = (x); if (P360_ERROR_CODE_IS_NOK(__code__)) { P360_DEBUG_ERROR("Operation Failed (%s)", P360GetErrorString(__code__)); return __code__; } }

typedef enum _P360_PIXEL_FORMAT {
	P360_PIXEL_FORMAT_NONE,
	P360_PIXEL_FORMAT_R8G8B8, // RGB24
	P360_PIXEL_FORMAT_B8G8R8, // BGR8
	P360_PIXEL_FORMAT_R8G8B8A8, // RGB32
	P360_PIXEL_FORMAT_B8G8R8A8, // BGRA32
	P360_PIXEL_FORMAT_A8B8G8R8, // ABGR32
	P360_PIXEL_FORMAT_A8R8G8B8, // ARGB32
}
P360_PIXEL_FORMAT;

typedef enum _P360_IMAGE_FORMAT {
	P360_IMAGE_FORMAT_NONE,
	P360_IMAGE_FORMAT_RAW,
	P360_IMAGE_FORMAT_JPEG,
	P360_IMAGE_FORMAT_JPG = P360_IMAGE_FORMAT_JPEG,
	P360_IMAGE_FORMAT_BMP,
	P360_IMAGE_FORMAT_BITMAP = P360_IMAGE_FORMAT_BMP,
	P360_IMAGE_FORMAT_PNG
}
P360_IMAGE_FORMAT;

typedef enum _P360_GL_TEXTURE_TYPE {
	P360_GL_TEXTURE_TYPE_NONE,
	P360_GL_TEXTURE_TYPE_2D,
	P360_GL_TEXTURE_TYPE_3D
}
P360_GL_TEXTURE_TYPE;

#endif /* _PAN360_COMMON_H_ */
