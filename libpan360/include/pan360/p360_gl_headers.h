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
#if !defined(_PAN360_GL_HEADERS_H_)
#define _PAN360_GL_HEADERS_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"

// Platforms info -> http://sourceforge.net/p/predef/wiki/Home/
// OpenGL headers setup -> http://cacs.usc.edu/education/cs596/OGL_Setup.pdf
#if defined(HAVE_GL_GLEW_H)
#	include <GL/glew.h>
#else
#	if P360_UNDER_WINDOWS
#		include <GL/GL.h>
#		include <GL/GLU.h>
#	elif P360_UNDER_LINUX
#		include <GL/gl.h>
#		include <GL/glu.h>
#	elif P360_UNDER_OSX
#		include <OpenGL/gl.h>
#		include <OpenGL/glu.h>
#	endif
#endif /* HAVE_GL_GLEW_H */

#endif /* _PAN360_GL_HEADERS_H_ */
