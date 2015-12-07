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
#include "pan360/p360_fileutils.h"
#include "pan360/p360_mem.h"
#include "pan360/p360_debug.h"

#if P360_UNDER_WINDOWS
#include <Shlwapi.h>
#endif /* P360_UNDER_WINDOWS */

#include <algorithm> // std::transform(), ...
#include <sys/stat.h>

#define kModuleNameFileUtils "FileUtils"

P360FileUtils::P360FileUtils()
{

}

P360FileUtils::~P360FileUtils()
{

}

bool P360FileUtils::exists(const char* pcPath)
{
    if (!pcPath) {
        P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Invalid parameter");
        return false;
    }
    struct stat st_;
    return (stat(pcPath, &st_) == 0);
}

bool P360FileUtils::empty(const char* pcPath)
{
    return (P360FileUtils::getSize(pcPath) == 0);
}

size_t P360FileUtils::getSize(const char* pcPath)
{
    if (!pcPath) {
        P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Invalid parameter");
        return 0;
    }
    struct stat st_;
    if (stat(pcPath, &st_) != 0) {
        return 0;
    }
    return st_.st_size;
}

/*
Returns extension in uppercase format
*/
std::string P360FileUtils::getExt(const char* pcPath)
{
    std::string ext_;
    if (!pcPath) {
        P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Invalid parameter");
    }
    else {
#if P360_UNDER_WINDOWS
        LPSTR strExt_ = PathFindExtensionA(pcPath);
        if (strExt_ && strlen(strExt_) > 1) {
            ext_ = std::string(strExt_ + 1);
        }
#else
        std::string path_ = std::string(pcPath);
        size_t index_ = path_.find_last_of(".");
        if (index_ != std::string::npos) {
            ext_ = path_.substr(index_ + 1);
        }
#endif
    }
    if (ext_.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Connot find extension for file path = %s", pcPath);
    }
    else {
        std::transform(ext_.begin(), ext_.end(), ext_.begin(), ::toupper); // UpperCase
    }
    return ext_;
}

_P360_IMAGE_FORMAT P360FileUtils::getImageFormat(const char* pcPath)
{
    _P360_IMAGE_FORMAT format_ = P360_IMAGE_FORMAT_NONE;
    std::string ext_ = P360FileUtils::getExt(pcPath); // UpperCase
    if (ext_ == "JPEG" || ext_ == "JPG" || ext_ == "JPE" || ext_ == "JFIF" || ext_ == "JIF") {
        return P360_IMAGE_FORMAT_JPEG;
    }
    else if (ext_ == "PNG") {
        return P360_IMAGE_FORMAT_PNG;
    }
    else if (ext_ == "BMP") {
        return P360_IMAGE_FORMAT_BMP;
    }
    else {
        P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Connot image format from file path = %s", pcPath);
        return P360_IMAGE_FORMAT_NONE;
    }
}

P360_ERROR_CODE P360FileUtils::read(const char* pcPath, P360ObjWrapper<P360Buffer*> *buffer)
{
    P360ObjWrapper<P360Buffer*> buffer_ = NULL;
    if (!pcPath || !buffer) {
        P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    size_t size_ = P360FileUtils::getSize(pcPath);
    if (size_ > 0) {
        FILE* file_ = NULL;
        void* mem_ = NULL;
        if ((file_ = fopen(pcPath, "rb")) == NULL) {
            P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Can't open %s", pcPath);
            return P360_ERROR_CODE_E_FILE_NOT_FOUND;
        }
        mem_ = P360Mem::Malloc(size_);
        if (!mem_) {
            P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Failed to alloc mem with size = %lu", size_);
            fclose(file_);
            return P360_ERROR_CODE_E_OUT_OF_MEMORY;
        }
        size_t read_;
        if (size_ != (read_ = fread(mem_, 1, size_, file_))) {
            P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "fread(%s) returned %lu instead of %lu", pcPath, read_, size_);
            fclose(file_);
            P360Mem::Free(&mem_);
            return P360_ERROR_CODE_E_FAILED_TO_READ_FILE;
        }
        if (P360_ERROR_CODE_IS_NOK(P360Buffer::newObjAndTakeData(&mem_, size_, &buffer_))) {
            P360_DEBUG_ERROR_EX(kModuleNameFileUtils, "Failed to create new P360Buffer object");
        }
        fclose(file_);
        P360Mem::Free(&mem_);
    }
    *buffer = buffer_;
    return P360_ERROR_CODE_S_OK;
}

