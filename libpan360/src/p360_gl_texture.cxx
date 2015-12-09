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
#include "pan360/p360_gl_texture.h"
#include "pan360/p360_gl_headers.h"
#include "pan360/p360_gl_utils.h"
#include "pan360/p360_fileutils.h"
#include "pan360/p360_globals.h"
#include "pan360/p360_image.h"
#include "pan360/p360_debug.h"

#define kModuleNameGLTexture	"GLTexture"
#define kModuleNameGLTexture2D	"GLTexture2D"

//
//	P360GLTexture
//

P360GLTexture::P360GLTexture(P360_GL_TEXTURE_TYPE eType)
	: m_eType(eType)
{

}

P360GLTexture:: ~P360GLTexture()
{

}


//
//	P360GLTexture2D
//


struct P360GLTexture2D::P360GLTexture2DPriv {
	GLuint m_uTexId;
	P360ObjWrapper<P360Image*> m_image;
	P360GLTexture2DPriv()
		: m_uTexId(0) {
	}
	virtual ~P360GLTexture2DPriv() {
		P360GLUtils::textureDelete(&m_uTexId);
		m_image = NULL;
	}
};

P360GLTexture2D::P360GLTexture2D()
	: P360GLTexture(P360_GL_TEXTURE_TYPE_2D)
	, m_pPriv(new P360GLTexture2DPriv())
{
	P360_ASSERT(m_pPriv != NULL);
}

P360GLTexture2D::~P360GLTexture2D()
{
	if (m_pPriv) {
		delete m_pPriv;
	}
}


P360_ERROR_CODE P360GLTexture2D::useBegin()
{
	if (!P360GLUtils::textureIsValid(m_pPriv->m_uTexId)) {
		P360_DEBUG_ERROR_EX(kModuleNameGLTexture2D, "Invalid texture: %u", m_pPriv->m_uTexId);
		return P360_ERROR_CODE_E_INVALID_STATE;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_pPriv->m_uTexId);
	return P360_ERROR_CODE_S_OK;
}

P360_ERROR_CODE P360GLTexture2D::useEnd()
{
	if (!P360GLUtils::progIsValid(m_pPriv->m_uTexId)) {
		P360_DEBUG_ERROR_EX(kModuleNameGLTexture2D, "Invalid texture: %u", m_pPriv->m_uTexId);
		return P360_ERROR_CODE_E_INVALID_STATE;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return P360_ERROR_CODE_S_OK;
}

P360_PIXEL_FORMAT P360GLTexture2D::getPixelFormat()
{
	return (m_pPriv->m_image ? m_pPriv->m_image->getPixelFormat() : P360GLTexture::getPixelFormat());
}

size_t P360GLTexture2D::getWidth()
{
	return (m_pPriv->m_image ? m_pPriv->m_image->getWidth() : P360GLTexture::getWidth());
}

size_t P360GLTexture2D::getHeight()
{
	return (m_pPriv->m_image ? m_pPriv->m_image->getHeight() : P360GLTexture::getHeight());
}

P360_ERROR_CODE P360GLTexture2D::newObj(const char* pcFilePath, P360ObjWrapper<P360GLTexture2D*>* texture2D)
{
	P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
	P360ObjWrapper<P360GLTexture2D*> text2D_;
	GLboolean textWasEnabled_ = GL_FALSE;
	GLint textOld_ = 0;

	P360_CHECK_CODE_BAIL(err_ = P360Globals::init());
	if (!texture2D) {
		P360_DEBUG_ERROR_EX(kModuleNameGLTexture2D, "Invalid parameter");
		return P360_ERROR_CODE_E_INVALID_PARAMETER;
	}

	text2D_ = new P360GLTexture2D();
	if (!text2D_ || !text2D_->m_pPriv) {
		P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_OUT_OF_MEMORY);
	}

	// Decode File
	P360_CHECK_CODE_BAIL(err_ = P360ImageDecoder::decodeFile(pcFilePath, &text2D_->m_pPriv->m_image));
	// Create texture
	P360_CHECK_CODE_BAIL(err_ = P360GLUtils::textureCreate(&text2D_->m_pPriv->m_uTexId));

	// Save old values
	textWasEnabled_ = glIsEnabled(GL_TEXTURE_2D);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &textOld_);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, text2D_->m_pPriv->m_uTexId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if ((text2D_->m_pPriv->m_image->getWidth() & 3)) { // multiple of 4?
		glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint)text2D_->m_pPriv->m_image->getWidth());
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)text2D_->m_pPriv->m_image->getWidth(), (GLsizei)text2D_->m_pPriv->m_image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, text2D_->m_pPriv->m_image->getDataPtr());

	// Restore old values
	if (textWasEnabled_ == GL_FALSE) {
		glDisable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, textOld_);

bail:
	if (P360_ERROR_CODE_IS_OK(err_)) {
		*texture2D = text2D_;
	}
	return err_;
}
