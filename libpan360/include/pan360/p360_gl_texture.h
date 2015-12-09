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
#if !defined(_PAN360_GL_TEXTURE_H_)
#define _PAN360_GL_TEXTURE_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"
#include "pan360/p360_obj.h"

class P360_API P360GLTexture : public P360Obj
{
protected:
	P360GLTexture(P360_GL_TEXTURE_TYPE eType);
public:
	virtual ~P360GLTexture();
	P360_INLINE P360_GL_TEXTURE_TYPE getType() { return m_eType; }
	virtual P360_ERROR_CODE useBegin() = 0;
	virtual P360_ERROR_CODE useEnd() = 0;
	virtual P360_PIXEL_FORMAT getPixelFormat() { return P360_PIXEL_FORMAT_R8G8B8; }
	virtual size_t getWidth() { return 0; }
	virtual size_t getHeight() { return 0; }
private:
	P360_GL_TEXTURE_TYPE m_eType;
};

class P360_API P360GLTexture2D : public P360GLTexture
{
private:
	P360GLTexture2D();
public:
	virtual ~P360GLTexture2D();
	virtual P360_INLINE const char* getObjectId() { return "P360GLTexture2D"; };

	// "P360GLTexture" override
	virtual P360_ERROR_CODE useBegin();
	virtual P360_ERROR_CODE useEnd();
	virtual P360_PIXEL_FORMAT getPixelFormat();
	virtual size_t getWidth();
	virtual size_t getHeight();

	static P360_ERROR_CODE newObj(const char* pcFilePath, P360ObjWrapper<P360GLTexture2D*>* texture2D);

private:
	// Pimpl Idiom: http://c2.com/cgi/wiki?PimplIdiom
	struct P360GLTexture2DPriv;
	P360GLTexture2DPriv* m_pPriv;
};

#endif /* _PAN360_GL_TEXTURE_H_ */
