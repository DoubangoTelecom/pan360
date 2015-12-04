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
#if !defined(_PAN360_BUFFER_H_)
#define _PAN360_BUFFER_H_

#include "pan360/p360_config.h"
#include "pan360/p360_common.h"
#include "pan360/p360_obj.h"

class P360_API P360Buffer : public P360Obj
{
private:
	P360Buffer(const void* pcPtr = NULL, size_t size = 0);
public:
	virtual ~P360Buffer();
	virtual P360_INLINE const char* getObjectId() { return "P360Buffer"; };
	P360_ERROR_CODE copyData(const void* pcPtr, size_t size);
	P360_ERROR_CODE takeData(void** ppPtr, size_t size);
	P360_INLINE const void* getPtr() { return m_pPtr; }
	P360_INLINE size_t getSize(){ return m_nSize; }
	static P360_ERROR_CODE newObj(const void* pcPtr, size_t size, P360ObjWrapper<P360Buffer*>* buffer);
	static P360_ERROR_CODE newObjAndNullData(P360ObjWrapper<P360Buffer*>* buffer);
	static P360_ERROR_CODE newObjAndTakeData(void** ppPtr, size_t size, P360ObjWrapper<P360Buffer*>* buffer);
	static P360_ERROR_CODE newObjAndCopyData(const void* pcPtr, size_t size, P360ObjWrapper<P360Buffer*>* buffer);

private:
	void* m_pPtr;
	size_t m_nSize;
};

#endif /* _PAN360_BUFFER_H_ */
