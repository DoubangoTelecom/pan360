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
#include "pan360/p360_mem.h"
#include "pan360/p360_debug.h"

/*
* Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
* The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
* @param size Size of the memory block, in bytes.
* @retval On success, a pointer to the memory block allocated by the function.
* It is up to you to free the returned pointer.
*/
void* P360Mem::Malloc(size_t size)
{
    void *pMem = malloc(size);
    if (!pMem) {
        P360_DEBUG_ERROR("Memory allocation failed");
    }
    return pMem;
}


/*
* Reallocate memory block.
* In case that ptr is NULL, the function behaves exactly as @a tsk_malloc, assigning a new block of size bytes and returning a pointer to the beginning of it.
* The function may move the memory block to a new location, in which case the new location is returned. The content of the memory block is preserved up to the lesser of the
* new and old sizes, even if the block is moved. If the new size is larger, the value of the newly allocated portion is indeterminate.
* In case that the size is 0, the memory previously allocated in ptr is deallocated as if a call to free was made, and a NULL pointer is returned.
* @param ptr Pointer to a memory block previously allocated with malloc, calloc or realloc to be reallocated.
* If this is NULL, a new block is allocated and a pointer to it is returned by the function.
* @param size New size for the memory block, in bytes.
* If it is 0 and ptr points to an existing block of memory, the memory block pointed by ptr is deallocated and a NULL pointer is returned.
* @retval A pointer to the reallocated memory block, which may be either the same as the ptr argument or a new location.
* The type of this pointer is void*, which can be cast to the desired type of data pointer in order to be dereferenceable.
* If the function failed to allocate the requested block of memory, a NULL pointer is returned.
* It is up to you to free the returned pointer.
*/
void* P360Mem::Realloc(void* ptr, size_t size)
{
    void *pMem = NULL;

    if (size) {
        if (ptr) {
            if (!(pMem = realloc(ptr, size))) {
                P360_DEBUG_ERROR("Memory reallocation failed");
            }
        }
        else {
            if (!(pMem = calloc(size, 1))) {
                P360_DEBUG_ERROR("Memory allocation (%u) failed", (unsigned)size);
            }
        }
    }
    else if (ptr) {
        free(ptr);
    }

    return pMem;
}


/*
* Deallocate space in memory.
* @param ptr Pointer to a memory block previously allocated with @a tsk_malloc, @a tsk_calloc or @a tsk_realloc to be deallocated.
* If a null pointer is passed as argument, no action occurs.
*/
void P360Mem::Free(void** ptr)
{
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}


/*
* Allocates a block of memory for an array of num elements, each of them size bytes long, and initializes all its bits to zero.
* The effective result is the allocation of an zero-initialized memory block of (num * size) bytes.
* @param num Number of elements to be allocated
* @param size Size of elements
* @retval A pointer to the memory block allocated by the function. The type of this pointer is always void*, which can be cast to the desired type of data pointer in order to be dereferenceable.
* If the function failed to allocate the requested block of memory, a NULL pointer is returned.
* It is up to you to free the returned pointer.
*/
void* P360Mem::Calloc(size_t num, size_t size)
{
    void* pMem = NULL;
    if (num && size) {
        pMem = calloc(num, size);
        if (!pMem) {
            P360_DEBUG_ERROR("Memory allocation failed. num=%u and size=%u", (unsigned)num, (unsigned)size);
        }
    }

    return pMem;
}

void* P360Mem::MallocAligned(size_t size, size_t alignment)
{
#if P360_UNDER_WINDOWS && !P360_UNDER_WINDOWS_CE && !P360_UNDER_WINDOWS_RT
    return _aligned_malloc(size, alignment);
#else
    void* ret = malloc(size + alignment);
    if (ret) {
        long pad = ((~(long)ret) % alignment) + 1;
        ret = ((uint8_t*)ret) + pad; // pad
        ((uint8_t*)ret)[-1] = (uint8_t)pad; // store the pad for later use
    }
    return ret;
#endif
}

void* P360Mem::ReallocAligned(void * ptr, size_t size, size_t alignment)
{
#if P360_UNDER_WINDOWS && !P360_UNDER_WINDOWS_CE && !P360_UNDER_WINDOWS_RT
    return _aligned_realloc(ptr, size, alignment);
#else
    tsk_free_aligned(ptr);
    return tsk_malloc_aligned(size, alignment);
#endif
}

void P360Mem::FreeAligned(void** ptr)
{
    if (ptr && *ptr) {
        void* ptr_ = *ptr;
#if P360_UNDER_WINDOWS && !P360_UNDER_WINDOWS_CE && !P360_UNDER_WINDOWS_RT
        _aligned_free(ptr_);
#else
        free((((uint8_t*)ptr_) - ((uint8_t*)ptr_)[-1]));
#endif
        *ptr = NULL;
    }
}

void* P360Mem::CallocAligned(size_t num, size_t size, size_t alignment)
{
    void* pMem = P360Mem::MallocAligned((size * num), alignment);
    if (pMem) {
        memset(pMem, 0, (size * num));
    }
    return pMem;
}
