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
#if !defined(_PAN360_GL_SHARDER_H_)
#define _PAN360_GL_SHARDER_H_

#include "pan360/p360_config.h"
#include "pan360/p360_obj.h"

class P360_API P360GlShaderProg
{
private:
	P360GlShaderProg();
public:
	virtual ~P360GlShaderProg();

	static P360_ERROR_CODE newObj(const void* pcPtr, size_t size, P360ObjWrapper<P360GlShaderProg*>* shaderProg);

private:
	// Pimpl Idiom: http://c2.com/cgi/wiki?PimplIdiom
	struct P360GlShaderProgPriv;
	P360GlShaderProgPriv* m_pPriv;
};

#endif /* _PAN360_GL_SHARDER_H_ */
