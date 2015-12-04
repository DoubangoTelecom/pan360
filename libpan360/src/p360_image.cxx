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
#include "pan360/p360_image.h"
#include "pan360/p360_globals.h"
#include "pan360/p360_fileutils.h"
#include "pan360/p360_debug.h"

//
//	P360Image
//

P360Image::P360Image(P360_IMAGE_FORMAT eImageFormat, P360_PIXEL_FORMAT ePixelFormat)
    :P360Obj()
    , m_nWidth(0)
    , m_nHeight(0)
    , m_ePixelFormat(ePixelFormat)
    , m_eImageFormat(eImageFormat)
{

}

P360Image::~P360Image()
{

}

P360_ERROR_CODE P360Image::setBuffer(P360ObjWrapper<P360Buffer*> & buffer, size_t width, size_t height)
{
    if (!buffer || !width || !height) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    m_oData = buffer;
    m_nWidth = width;
    m_nHeight = height;
    return P360_ERROR_CODE_S_OK;
}

P360ObjWrapper<P360Image*> P360Image::loadImage(const char* filePath)
{
    P360_DEBUG_ERROR("Not implemented");

    return NULL;
}

P360_ERROR_CODE P360Image::getSizeForPixelFormat(P360_PIXEL_FORMAT ePixelFormat, size_t width, size_t height, size_t *size)
{
    if (!size) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    switch (ePixelFormat) {
    case P360_PIXEL_FORMAT_R8G8B8:
    case P360_PIXEL_FORMAT_B8G8R8:
        *size = (width * height * 3);
        return P360_ERROR_CODE_S_OK;

    case P360_PIXEL_FORMAT_R8G8B8A8:
    case P360_PIXEL_FORMAT_B8G8R8A8:
    case P360_PIXEL_FORMAT_A8B8G8R8:
    case P360_PIXEL_FORMAT_A8R8G8B8:
        *size = (width * height * 4);
        return P360_ERROR_CODE_S_OK;
    default:
        P360_DEBUG_ERROR("Invalid pixel format");
        return P360_ERROR_CODE_E_INVALID_PIXEL_FORMAT;
    }
}

P360_ERROR_CODE P360Image::newObj(P360_IMAGE_FORMAT eImageFormat, P360_PIXEL_FORMAT ePixelFormat, P360ObjWrapper<P360Image*>* image)
{
    if (!image) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    *image = new P360Image(eImageFormat, ePixelFormat);
    if (!*image) {
        P360_DEBUG_ERROR("Failed to alloc new 'P360Image' object");
        return P360_ERROR_CODE_E_OUT_OF_MEMORY;
    }
    return P360_ERROR_CODE_S_OK;
}

//
//	P360ImageDecoder
//

P360DecodeFileFuncPtr P360ImageDecoder::s_funcptrDecodeFileJpeg = NULL;

#define kModuleNameImageDecoder "ImageDecoder"

P360_ERROR_CODE P360ImageDecoder::init()
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;

    P360_DEBUG_INFO_EX(kModuleNameImageDecoder, "Initializing image decoder...");

#if defined(HAVE_LIBJPEG)
    extern P360_ERROR_CODE libjpegDecodeFile(const char* filePath, P360ObjWrapper<P360Image*>* image);
    P360ImageDecoder::s_funcptrDecodeFileJpeg = libjpegDecodeFile;
#else
    P360_DEBUG_WARN_EX(kModuleNameImageDecoder, "No jpeg decoder found");
#endif /* HAVE_LIBJPEG */

    P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_S_OK);

bail:
    return err_;
}

P360_ERROR_CODE P360ImageDecoder::decodeFile(const char* filePath, P360ObjWrapper<P360Image*>* image)
{
    _P360_IMAGE_FORMAT format_ = P360FileUtils::getImageFormat(filePath);
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    switch (format_) {
    case P360_IMAGE_FORMAT_JPEG:
        if (P360ImageDecoder::s_funcptrDecodeFileJpeg) {
            return P360ImageDecoder::s_funcptrDecodeFileJpeg(filePath, image);
        }
        else {
            P360_DEBUG_ERROR_EX(kModuleNameImageDecoder, "Cannot find jpeg decoder for file: %s", filePath);
            P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_DECODER_NOT_FOUND);
        }
        break;
    case P360_IMAGE_FORMAT_RAW:
    case P360_IMAGE_FORMAT_BITMAP:
    case P360_IMAGE_FORMAT_PNG:
    default:
        P360_DEBUG_ERROR_EX(kModuleNameImageDecoder, "Cannot find decoder for file: %s, format: %d", filePath, format_);
        P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_DECODER_NOT_FOUND);
        break;
    }

bail:
    return P360_ERROR_CODE_E_NOT_IMPLEMENTED;
}
