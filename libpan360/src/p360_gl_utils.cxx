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
#include "pan360/p360_gl_utils.h"
#include "pan360/p360_mem.h"
#include "pan360/p360_fileutils.h"
#include "pan360/p360_globals.h"
#include "pan360/p360_debug.h"

#define kModuleNameGLUtils "GLUtils"

#define __glu_h__
#define __GLU_H__

P360_ERROR_CODE P360GLUtils::getLastError(std::string *error)
{
    GLenum err;
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    char buff_[33] = { 0 };
    while ((err = glGetError()) != GL_NO_ERROR) {
        if (error) {
#if defined(HAVE_GL_GLU_H) || defined(HAVE_GLU_H) || defined(__glu_h__) || defined(__GLU_H__)
            *error += "\n" + std::string((const char*)gluErrorString(err));
#else
            *error += "\n" + std::string((const char*)itoa((int)err, buff_, 10));
#endif
        }
        err_ = P360_ERROR_CODE_E_GL;
    }
    return err_;
}

P360_ERROR_CODE P360GLUtils::shaderDelete(GLuint* uShader)
{
    if (uShader && P360GLUtils::shaderIsValid(*uShader)) {
        glDeleteShader(*uShader);
        *uShader = 0;
        return P360_ERROR_CODE_S_OK;
    }
    return P360_ERROR_CODE_E_INVALID_PARAMETER;
}

P360_ERROR_CODE P360GLUtils::shaderDelete(std::vector<GLuint >& uShaders)
{
    std::vector<GLuint >::iterator it = uShaders.begin();
    GLuint shader_;
    for (; it != uShaders.end(); ++it) {
        shader_ = *it;
        P360GLUtils::shaderDelete(&shader_);
    }
    uShaders.clear();
    return P360_ERROR_CODE_S_OK;
}

P360_ERROR_CODE P360GLUtils::shaderCreate(GLuint* uShader, GLenum shaderType)
{
    if (uShader) {
        P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
        std::string errString_;
        GLuint shader_;
        *uShader = 0;
        shader_ = glCreateShader(shaderType);
        if (!P360GLUtils::shaderIsValid(shader_)) {
            P360_CHECK_CODE_BAIL(err_ = P360GLUtils::getLastError(&errString_));
            P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_GL);
        }
        *uShader = shader_;
bail:
        if (!errString_.empty()) {
            P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "glCreateShader failed: %s", errString_.c_str());
        }
        return err_;
    }
    P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
    return P360_ERROR_CODE_E_INVALID_PARAMETER;
}

P360_ERROR_CODE P360GLUtils::shaderCreateVert(GLuint* uShader)
{
    return P360GLUtils::shaderCreate(uShader, GL_VERTEX_SHADER);
}

P360_ERROR_CODE P360GLUtils::shaderCreateFrag(GLuint* uShader)
{
    return P360GLUtils::shaderCreate(uShader, GL_FRAGMENT_SHADER);
}

bool P360GLUtils::shaderIsValid(GLuint uShader)
{
    return uShader != 0 && glIsShader(uShader) == GL_TRUE;
}

/*
Replaces the source code in a shader object
@param uShader Specifies the handle of the shader object whose source code is to be replaced.
@param count Specifies the number of elements in the string and length arrays.
@param string Specifies an array of pointers to strings containing the source code to be loaded into the shader.
@param length Specifies an array of string lengths
*/
P360_ERROR_CODE P360GLUtils::shaderSetSource(GLuint uShader, GLsizei count, const GLchar **string, const GLint *length)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    std::string errString_;
    if (!uShader || count <= 0 || !string || !*string) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    glShaderSource(uShader, count, string, length);
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::getLastError(&errString_));

bail:
    if (!errString_.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "glShaderSource failed: %s", errString_.c_str());
    }
    return err_;
}

P360_ERROR_CODE P360GLUtils::shaderSetSource(GLuint uShader, const P360ObjWrapper<P360Buffer*>& buff)
{
    if (!P360GLUtils::shaderIsValid(uShader) || !buff || buff->isEmpty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    const char* source_ = (const char*)buff->getPtr();
    const GLint length_ = (GLint)buff->getSize();

    return P360GLUtils::shaderSetSource(uShader, 1, &source_, &length_);
}

P360_ERROR_CODE P360GLUtils::shaderSetSource(GLuint uShader, const char* filePath)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    P360ObjWrapper<P360Buffer*> buff_;
    if (P360FileUtils::empty(filePath)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    P360_CHECK_CODE_BAIL(err_ = P360FileUtils::read(filePath, &buff_));
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::shaderSetSource(uShader, buff_));

bail:
    return err_;
}

P360_ERROR_CODE P360GLUtils::shaderCompile(GLuint uShader)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    std::string errString_;
    if (!P360GLUtils::shaderIsValid(uShader)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    glCompileShader(uShader);
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::shaderCompileGetStatus(uShader, &errString_));

bail:
    if (!errString_.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Shader compile info log: %s", errString_.c_str());
    }
    return err_;
}

P360_ERROR_CODE P360GLUtils::shaderCompileGetStatus(GLuint uShader, std::string *error)
{
    if (!P360GLUtils::shaderIsValid(uShader)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    GLint ok_ = 0;
    glGetShaderiv(uShader, GL_COMPILE_STATUS, &ok_);
    if (ok_ == GL_FALSE) {
        if (error) {
            *error = "";
            GLint maxLength = 0;
            glGetShaderiv(uShader, GL_INFO_LOG_LENGTH, &maxLength);
            if (maxLength > 0) {
                GLchar* infoLog = (GLchar*)P360Mem::Malloc(maxLength + 1);
                if (infoLog) {
                    infoLog[maxLength] = '\0';
                    glGetShaderInfoLog(uShader, maxLength, &maxLength, infoLog);
                    *error = std::string((const char*)infoLog, maxLength);
                }
            }
        }
        return P360_ERROR_CODE_E_GL;
    }
    return P360_ERROR_CODE_S_OK;
}

P360_ERROR_CODE P360GLUtils::shaderAttach(GLuint uProg, GLuint uShader)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    std::string errString_;
    if (!P360GLUtils::progIsValid(uProg) || !P360GLUtils::shaderIsValid(uShader)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    glAttachShader(uProg, uShader);
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::getLastError(&errString_));

bail:
    if (!errString_.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "glAttachShader failed: %s", errString_.c_str());
    }
    return err_;
}


P360_ERROR_CODE P360GLUtils::progCreate(GLuint* uProg)
{
    if (uProg) {
        P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
        std::string errString_;
        GLuint prog_;
        *uProg = 0;
        prog_ = glCreateProgram();
        if (!P360GLUtils::progIsValid(prog_)) {
            P360_CHECK_CODE_BAIL(err_ = P360GLUtils::getLastError(&errString_));
            P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_GL);
        }
        *uProg = prog_;
bail:
        if (!errString_.empty()) {
            P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "glCreateProgram failed: %s", errString_.c_str());
        }
        return err_;
    }
    P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
    return P360_ERROR_CODE_E_INVALID_PARAMETER;
}

P360_ERROR_CODE P360GLUtils::progDelete(GLuint* uProg)
{
    if (uProg && P360GLUtils::progIsValid(*uProg)) {
        glDeleteProgram(*uProg);
        *uProg = 0;
        return P360_ERROR_CODE_S_OK;
    }
    return P360_ERROR_CODE_E_INVALID_PARAMETER;
}

bool P360GLUtils::progIsValid(GLuint uProg)
{
    return uProg != 0 && glIsProgram(uProg) == GL_TRUE;
}

bool P360GLUtils::progIsUsed(GLuint uProg)
{
    if (P360GLUtils::progIsValid(uProg)) {
        GLuint prog_ = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&prog_);
        return (uProg == prog_);
    }
    return false;
}

bool P360GLUtils::progIsCurrent(GLuint uProg)
{
    return P360GLUtils::progIsUsed(uProg);
}

P360_ERROR_CODE P360GLUtils::progLink(GLuint uProg)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    std::string errString_;
    if (!P360GLUtils::progIsValid(uProg)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    glLinkProgram(uProg);
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::progLinkGetStatus(uProg, &errString_));

bail:
    if (!errString_.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Program link info log: %s", errString_.c_str());
    }
    return err_;
}

P360_ERROR_CODE P360GLUtils::progLinkGetStatus(GLuint uProg, std::string *error)
{
    if (!P360GLUtils::progIsValid(uProg)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    GLint ok_ = 0;
    glGetProgramiv(uProg, GL_LINK_STATUS, (int *)&ok_);
    if (ok_ == GL_FALSE) {
        if (error) {
            *error = "";
            GLint maxLength = 0;
            glGetProgramiv(uProg, GL_INFO_LOG_LENGTH, &maxLength);
            if (maxLength > 0) {
                GLchar* infoLog = (GLchar*)P360Mem::Malloc(maxLength + 1);
                if (infoLog) {
                    infoLog[maxLength] = '\0';
                    glGetProgramInfoLog(uProg, maxLength, &maxLength, infoLog);
                    *error = std::string((const char*)infoLog, maxLength);
                }
            }
        }
        return P360_ERROR_CODE_E_GL;
    }
    return P360_ERROR_CODE_S_OK;
}

P360_ERROR_CODE P360GLUtils::progUseBegin(GLuint uProg)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    std::string errString_;
    if (!P360GLUtils::progIsValid(uProg)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    glUseProgram(uProg);
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::getLastError(&errString_));

bail:
    if (!errString_.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "glUseProgram failed: %s", errString_.c_str());
    }
    return err_;
}

P360_ERROR_CODE P360GLUtils::progUseEnd(GLuint uProg)
{
    if (!P360GLUtils::progIsValid(uProg)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    GLuint prog_ = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&prog_);
    if (prog_ != uProg) {
        P360_DEBUG_ERROR_EX(kModuleNameGLUtils, "Program %u not in use", uProg);
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    glUseProgram(0);
    return P360_ERROR_CODE_S_OK;
}


