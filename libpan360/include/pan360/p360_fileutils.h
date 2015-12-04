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
#if !defined(_PAN360_FILEUTILS_H_)
#define _PAN360_FILEUTILS_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"
#include "pan360/p360_buffer.h"
#include <string>

class P360FileUtils
{
private:
	P360FileUtils();
public:
	virtual ~P360FileUtils();
	static bool exists(const char* pcPath);
	static size_t getSize(const char* pcPath);
	static std::string getExt(const char* pcPath);
	static _P360_IMAGE_FORMAT getImageFormat(const char* pcPath);
	static P360ObjWrapper<P360Buffer*> read(const char* pcPath);
};

#endif /* _PAN360_FILEUTILS_H_ */
