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
#if !defined(_PAN360_PRIV_GL_UTILS_H_)
#define _PAN360_PRIV_GL_UTILS_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"
#include "pan360/p360_gl_headers.h"
#include "pan360/p360_buffer.h"

#include <string>
#include <vector>

class P360GLUtils
{
public:
	static P360_ERROR_CODE getLastError(std::string *error);

	static P360_ERROR_CODE shaderDelete(GLuint* uShader);
	static P360_ERROR_CODE shaderDelete(std::vector<GLuint >& uShaders);
	static P360_ERROR_CODE shaderCreate(GLuint* uShader, GLenum shaderType);
	static P360_ERROR_CODE shaderCreateVert(GLuint* uShader);
	static P360_ERROR_CODE shaderCreateFrag(GLuint* uShader);
	static bool shaderIsValid(GLuint uShader);
	static P360_ERROR_CODE shaderSetSource(GLuint uShader, GLsizei count, const GLchar **string, const GLint *length);
	static P360_ERROR_CODE shaderSetSource(GLuint uShader, const P360ObjWrapper<P360Buffer*>& buff);
	static P360_ERROR_CODE shaderSetSource(GLuint uShader, const char* filePath);
	static P360_ERROR_CODE shaderCompile(GLuint uShader);
	static P360_ERROR_CODE shaderCompileGetStatus(GLuint uShader, std::string *error);
	static P360_ERROR_CODE shaderAttach(GLuint uProg, GLuint uShader);

	static P360_ERROR_CODE textureCreate(GLuint* uText);
	static P360_ERROR_CODE textureDelete(GLuint* uText);
	static bool textureIsValid(GLuint uText);
	static bool texture2DIsEnabled(GLuint uText);
	static P360_ERROR_CODE texture2DEnable();
	static P360_ERROR_CODE textureGetCurrent(GLuint* uText);
	static P360_ERROR_CODE textureSetCurrent(GLuint uText);

	static P360_ERROR_CODE progCreate(GLuint* uProg);
	static P360_ERROR_CODE progDelete(GLuint* uProg);
	static bool progIsValid(GLuint uProg);
	static bool progIsUsed(GLuint uProg);
	static bool progIsCurrent(GLuint uProg);
	static P360_ERROR_CODE progLink(GLuint uProg);
	static P360_ERROR_CODE progLinkGetStatus(GLuint uProg, std::string *error);
	static P360_ERROR_CODE progUseBegin(GLuint uProg);
	static P360_ERROR_CODE progUseEnd(GLuint uProg);
};


#endif /* _PAN360_PRIV_GL_UTILS_H_ */
