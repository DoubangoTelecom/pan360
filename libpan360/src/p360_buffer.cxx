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
#include "pan360/p360_buffer.h"
#include "pan360/p360_mem.h"
#include "pan360/p360_globals.h"
#include "pan360/p360_debug.h"

P360Buffer::P360Buffer(const void* pcPtr /*= NULL*/, size_t size /*= 0*/)
    : P360Obj()
    , m_pPtr(NULL)
    , m_nSize(0)
{
    if (pcPtr && size) {
        P360_ASSERT(P360_ERROR_CODE_IS_OK(copyData(pcPtr, size)));
    }
}

P360_ERROR_CODE P360Buffer::copyData(const void* pcPtr, size_t size)
{
    if (!pcPtr || !size) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    P360_ASSERT((m_pPtr = P360Mem::Realloc(m_pPtr, size)) != NULL);
    if (m_pPtr) {
        m_nSize = size;
        memcpy(m_pPtr, pcPtr, size);
    }
    else {
        P360_DEBUG_FATAL("Failed to allocate buffer with size = %lu", size);
        return P360_ERROR_CODE_E_OUT_OF_MEMORY;
    }
    return P360_ERROR_CODE_S_OK;
}

P360_ERROR_CODE P360Buffer::takeData(void** ppPtr, size_t size)
{
    if (!ppPtr || !*ppPtr || !size) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    P360Mem::Free(&m_pPtr);
    m_pPtr = *ppPtr, *ppPtr = NULL;
    m_nSize = size;
    return P360_ERROR_CODE_S_OK;
}

P360Buffer::~P360Buffer()
{
    P360Mem::Free(&m_pPtr);
}

P360_ERROR_CODE P360Buffer::newObj(const void* pcPtr, size_t size, P360ObjWrapper<P360Buffer*>* buffer)
{
    if (!buffer) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }
    *buffer = new P360Buffer(pcPtr, size);
    if (!*buffer) {
        P360_DEBUG_ERROR("Failed to alloc new 'P360Buffer' object");
        return P360_ERROR_CODE_E_OUT_OF_MEMORY;
    }
    return P360_ERROR_CODE_S_OK;
}

P360_ERROR_CODE P360Buffer::newObjAndNullData(P360ObjWrapper<P360Buffer*>* buffer)
{
    return P360Buffer::newObj(NULL, 0, buffer);
}

P360_ERROR_CODE P360Buffer::newObjAndTakeData(void** ppPtr, size_t size, P360ObjWrapper<P360Buffer*>* buffer)
{
    P360ObjWrapper<P360Buffer*> buffer_;
    P360_ERROR_CODE err = P360_ERROR_CODE_S_OK;

    if (!buffer) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }

    P360_CHECK_CODE_BAIL(err = P360Buffer::newObjAndNullData(&buffer_));
    P360_CHECK_CODE_BAIL(err = buffer_->takeData(ppPtr, size));
    *buffer = buffer_;

bail:
    return err;
}

P360_ERROR_CODE P360Buffer::newObjAndCopyData(const void* pcPtr, size_t size, P360ObjWrapper<P360Buffer*>* buffer)
{
    P360ObjWrapper<P360Buffer*> buffer_;
    P360_ERROR_CODE err = P360_ERROR_CODE_S_OK;

    if (!buffer) {
        P360_DEBUG_ERROR("Invalid parameter");
        return P360_ERROR_CODE_E_INVALID_PARAMETER;
    }

    P360_CHECK_CODE_BAIL(err = P360Buffer::newObjAndNullData(&buffer_));
    P360_CHECK_CODE_BAIL(err = buffer_->copyData(pcPtr, size));
    *buffer = buffer_;

bail:
    return err;
}
