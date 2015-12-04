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
#if !defined(_PAN360_MEM_H_)
#define _PAN360_MEM_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"

class P360Mem
{
public:
	static void* Malloc(size_t size);
	static void* Realloc(void * ptr, size_t size);
	static void Free(void** ptr);
	static void* Calloc(size_t num, size_t size);

	static void* MallocAligned(size_t size, size_t alignment);
	static void* ReallocAligned(void * ptr, size_t size, size_t alignment);
	static void FreeAligned(void** ptr);
	static void* CallocAligned(size_t num, size_t size, size_t alignment);
};

#endif /* _PAN360_MEM_H_ */
