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
#ifndef _PAN360_CONFIG_H_
#define _PAN360_CONFIG_H_

// Platforms info http://sourceforge.net/p/predef/wiki/Home/

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN16) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#	define P360_UNDER_WINDOWS	1
#	if defined(_WIN32_WCE) || defined(UNDER_CE)
#		define P360_UNDER_WINDOWS_CE	1
#		define P360_STDCALL			__cdecl
#	else
#		define P360_STDCALL __stdcall
#	endif
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define P360_UNDER_WINDOWS_RT		1
#	endif
#else
#	define P360_STDCALL
#endif
// OSX or iOS
#if defined(__APPLE__)
#	define P360_UNDER_APPLE				1
#endif
#if TARGET_OS_MAC || (defined (macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__)))
#	define P360_UNDER_OSX				1
#endif
#if TARGET_OS_IPHONE
#	define P360_UNDER_IPHONE			1
#endif
#if TARGET_IPHONE_SIMULATOR
#	define P360_UNDER_IPHONE_SIMULATOR	1
#endif
// Android
#if defined(__ANDROID__)
#	define P360_UNDER_ANDROID			1
#endif
// BSD
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#	define P360_UNDER_BSD				1
#endif
// Cygwin
#if defined(__CYGWIN__)
#	define P360_UNDER_CYGWIN			1
#endif
// GNU Linix
#if defined(__gnu_linux__)
#	define P360_UNDER_GNU_LINUX			1
#endif
// Linux
#if defined(__linux__)
#	define P360_UNDER_LINUX				1
#endif
// MS DOS
#if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#	define P360_UNDER_MSDOS				1
#endif
// NetBSD
#if defined(__NetBSD__)
#	define P360_UNDER_NETBSD			1
#endif
// OpenBSD
#if defined(__OpenBSD__)
#	define P360_UNDER_OPENBSD			1
#endif
// OS/2
#if defined(OS2) || defined(_OS2) || defined(__OS2__) || defined(__TOS_OS2__)
#	define P360_UNDER_OS2				1
#endif
// Solaris
#if defined(sun) || defined(__sun) 
#	define P360_UNDER_SOLARIS			1
#endif
// Unix
#if defined(__unix__) || defined(__unix)
#	define P360_UNDER_UNIX				1
#endif
// VxWorks
#if defined(__VXWORKS__) || defined(__vxworks)
#	define P360_UNDER_VXWORKS			1
#endif

// Disable some well-known warnings
#if defined(_MSC_VER)
#if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#	endif /* _CRT_SECURE_NO_WARNINGS */
#	define P360_INLINE	_inline
#else
#	define P360_INLINE	inline
#endif

// Disable some well-known warnings
#if defined(_MSC_VER)
#	define P360_DISABLE_WARNINGS_BEGIN(nn) \
		__pragma(warning( push )) \
		__pragma(warning( disable : ##nn ))
#	define P360_DISABLE_WARNINGS_END() \
		__pragma(warning( pop ))
#else
#	define P360_DISABLE_WARNINGS_BEGIN(nn)
#	define P360_DISABLE_WARNINGS_END()
#endif /* _MSC_VER */


#if P360_UNDER_WINDOWS && defined(PAN360_EXPORTS)
# 	define P360_API		__declspec(dllexport)
# 	define P360_GEXTERN	__declspec(dllexport)
#elif P360_UNDER_WINDOWS /*&& defined(P360_IMPORTS)*/
# 	define P360_API		__declspec(dllimport)
# 	define P360_GEXTERN	__declspec(dllimport)
#else
#	define P360_API
#	define P360_GEXTERN	extern
#endif

#ifdef __GNUC__
#	define p360_atomic_inc(_ptr_) __sync_fetch_and_add((_ptr_), 1)
#	define p360_atomic_dec(_ptr_) __sync_fetch_and_sub((_ptr_), 1)
#elif defined (_MSC_VER)
#	define p360_atomic_inc(_ptr_) InterlockedIncrement((_ptr_))
#	define p360_atomic_dec(_ptr_) InterlockedDecrement((_ptr_))
#else
#	define p360_atomic_inc(_ptr_) ++(*(_ptr_))
#	define p360_atomic_dec(_ptr_) --(*(_ptr_))
#endif


/* define "TNET_DEPRECATED(func)" macro */
#if defined(__GNUC__)
#	define P360_DEPRECATED(func) __attribute__ ((deprecated)) func
#elif defined(_MSC_VER)
#	define P360_DEPRECATED(func) __declspec(deprecated) func
#else
#	pragma message("WARNING: Deprecated not supported for this compiler")
#	define P360_DEPRECATED(func) func
#endif

#if P360_UNDER_WINDOWS
#	define _WINSOCKAPI_
#	include <windows.h>
#elif P360_UNDER_LINUX
#elif P360_UNDER_OSX
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#endif /* _PAN360_CONFIG_H_ */
