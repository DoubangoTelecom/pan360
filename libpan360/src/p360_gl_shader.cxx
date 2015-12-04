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

struct P360GlShaderProg::P360GlShaderProgPriv {
    P360GlShaderProgPriv() {

    }
    virtual ~P360GlShaderProgPriv() {

    }
};

P360GlShaderProg::P360GlShaderProg()
    : m_pPriv(new P360GlShaderProgPriv())
{
    P360_ASSERT(m_pPriv != NULL);
}

P360GlShaderProg::~P360GlShaderProg()
{
    if (m_pPriv) {
        delete m_pPriv;
    }
}

