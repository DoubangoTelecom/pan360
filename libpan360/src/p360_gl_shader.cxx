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
#include "pan360/p360_gl_shader.h"
#include "pan360/p360_gl_headers.h"
#include "pan360/p360_gl_utils.h"
#include "pan360/p360_fileutils.h"
#include "pan360/p360_globals.h"
#include "pan360/p360_debug.h"

#include <vector>

#define kModuleNameGLShader "GLShader"

struct P360GLShaderProg::P360GLShaderProgPriv {
    std::vector<GLuint > m_uVertShaders;
    std::vector<GLuint > m_uFragShaders;
    GLuint m_uProg;
    bool m_bLinked;
    bool m_bUsed;

    P360GLShaderProgPriv()
        : m_uProg(0)
        , m_bLinked(false)
        , m_bUsed(false) {
    }
    virtual ~P360GLShaderProgPriv() {

    }
};

P360GLShaderProg::P360GLShaderProg()
    : m_pPriv(new P360GLShaderProgPriv())
{
    P360_ASSERT(m_pPriv != NULL);
}

P360GLShaderProg::~P360GLShaderProg()
{
    if (m_pPriv) {
        P360GLUtils::shaderDelete(m_pPriv->m_uFragShaders);
        P360GLUtils::shaderDelete(m_pPriv->m_uVertShaders);
        P360GLUtils::progDelete(&m_pPriv->m_uProg);
        delete m_pPriv;
    }
}

P360_ERROR_CODE P360GLShaderProg::shaderAttach(const char* pcFilePath, bool vertType)
{
    GLuint shader_ = 0;
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    if (P360FileUtils::empty(pcFilePath)) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    if (vertType) {
        // Vertex shader
        if (m_pPriv->m_uVertShaders.size() != m_pPriv->m_uFragShaders.size()) {
            P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Number of vertex shaders must match the number of fragment shaders");
            return P360_ERROR_CODE_E_INVALID_PARAMETER;
        }
    }
    else {
        // Fragment shader
        if (m_pPriv->m_uVertShaders.size() - 1 != m_pPriv->m_uFragShaders.size()) {
            P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Each fragment shader must match a vertex shader");
            return P360_ERROR_CODE_E_INVALID_PARAMETER;
        }
    }
    P360_CHECK_CODE_BAIL(err_ = vertType ? P360GLUtils::shaderCreateVert(&shader_) : P360GLUtils::shaderCreateFrag(&shader_));
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::shaderSetSource(shader_, pcFilePath));
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::shaderCompile(shader_));
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::shaderAttach(m_pPriv->m_uProg, shader_));

bail:
    if (P360_ERROR_CODE_IS_OK(err_)) {
        std::vector<GLuint >* shaders_ = vertType ? &m_pPriv->m_uVertShaders : &m_pPriv->m_uFragShaders;
        shaders_->push_back(shader_);
    }
    else {
        P360GLUtils::shaderDelete(&shader_);
    }
    return err_;
}

P360_ERROR_CODE P360GLShaderProg::shaderAttachVertex(const char* pcFilePath)
{
    return shaderAttach(pcFilePath, true);
}

P360_ERROR_CODE P360GLShaderProg::shaderAttachFragment(const char* pcFilePath)
{
    return shaderAttach(pcFilePath, false);
}

P360_ERROR_CODE P360GLShaderProg::link()
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    if (m_pPriv->m_bUsed || m_pPriv->m_bLinked) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Program in use or already linked");
        return P360_ERROR_CODE_E_INVALID_STATE;
    }
    if (m_pPriv->m_uVertShaders.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "No vertex shaders");
        return P360_ERROR_CODE_E_INVALID_STATE;
    }
    if (m_pPriv->m_uFragShaders.empty()) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "No fragment shaders");
        return P360_ERROR_CODE_E_INVALID_STATE;
    }
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::progLink(m_pPriv->m_uProg));
    m_pPriv->m_bLinked = true;
bail:
    return err_;
}

P360_ERROR_CODE P360GLShaderProg::useBegin()
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    if (!m_pPriv->m_bLinked) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Program not linked");
        return P360_ERROR_CODE_E_INVALID_STATE;
    }
    // do not check "m_pPriv->m_bUsed", program could be used from different threads several times to make it current
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::progUseBegin(m_pPriv->m_uProg));
    m_pPriv->m_bUsed = true;
bail:
    return err_;
}

P360_ERROR_CODE P360GLShaderProg::useEnd()
{
    if (!m_pPriv->m_bLinked) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Program not linked");
        return P360_ERROR_CODE_E_INVALID_STATE;
    }
    // do not check "m_pPriv->m_bUsed", program could be used from different threads several times to make it current
    return P360GLUtils::progUseEnd(m_pPriv->m_uProg);
}

P360_ERROR_CODE P360GLShaderProg::newObj(P360ObjWrapper<P360GLShaderProg*>* shaderProg)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    P360ObjWrapper<P360GLShaderProg*> shaderProg_;
    P360_CHECK_CODE_BAIL(err_ = P360Globals::init());
    if (!shaderProg) {
        P360_DEBUG_ERROR_EX(kModuleNameGLShader, "Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }

    shaderProg_ = new P360GLShaderProg();
    if (!shaderProg_ || !shaderProg_->m_pPriv) {
        P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_OUT_OF_MEMORY);
    }

    // Create program
    P360_CHECK_CODE_BAIL(err_ = P360GLUtils::progCreate(&shaderProg_->m_pPriv->m_uProg));

bail:
    if (P360_ERROR_CODE_IS_OK(err_)) {
        *shaderProg = shaderProg_;
    }
    return err_;
}
