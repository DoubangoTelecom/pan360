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
#if !defined(_PAN360_IMAGE_H_)
#define _PAN360_IMAGE_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"
#include "pan360/p360_buffer.h"

class P360_API P360Image : public P360Obj
{
private:
	P360Image(P360_IMAGE_FORMAT eImageFormat, P360_PIXEL_FORMAT ePixelFormat);
public:
	virtual ~P360Image();
	virtual P360_INLINE const char* getObjectId() { return "P360Image"; };

	P360_ERROR_CODE setBuffer(P360ObjWrapper<P360Buffer*> & buffer, size_t width, size_t height);

	P360_INLINE const P360ObjWrapper<P360Buffer*>& getData() { return m_oData; }
	P360_INLINE size_t getDataSize() { return m_oData ? m_oData->getSize() : 0; }
	P360_INLINE const void* getDataPtr() { return m_oData ? m_oData->getPtr() : NULL; }
	P360_INLINE size_t getWidth() { return m_nWidth; }
	P360_INLINE size_t getHeight() { return m_nHeight; }
	P360_INLINE P360_PIXEL_FORMAT getPixelFormat() { return m_ePixelFormat; }
	P360_INLINE P360_IMAGE_FORMAT getImageFormat() { return m_eImageFormat; }
	
	static P360ObjWrapper<P360Image*> loadImage(const char* filePath);
	static P360_ERROR_CODE getSizeForPixelFormat(P360_PIXEL_FORMAT ePixelFormat, size_t width, size_t height, size_t *size);
	static P360_ERROR_CODE newObj(P360_IMAGE_FORMAT eImageFormat, P360_PIXEL_FORMAT ePixelFormat, P360ObjWrapper<P360Image*>* image);

protected:
	P360_DISABLE_WARNINGS_BEGIN(4251 4267)
	P360ObjWrapper<P360Buffer*> m_oData;
	size_t m_nWidth;
	size_t m_nHeight;
	P360_PIXEL_FORMAT m_ePixelFormat;
	P360_IMAGE_FORMAT m_eImageFormat;
	P360_DISABLE_WARNINGS_END()
};

typedef P360_ERROR_CODE(*P360DecodeFileFuncPtr)(const char* filePath, P360ObjWrapper<P360Image*>* pImage);

class P360_API P360ImageDecoder
{
public:
	static P360_ERROR_CODE decodeFile(const char* filePath, P360ObjWrapper<P360Image*>* image);
	static P360_ERROR_CODE init();

private:
	static P360DecodeFileFuncPtr s_funcptrDecodeFileJpeg;
};

#endif /* _PAN360_IMAGE_H_ */
