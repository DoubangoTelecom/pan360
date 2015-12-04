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
#include "pan360/p360_image.h"
#include "pan360/p360_buffer.h"
#include "pan360/p360_globals.h"
#include "pan360/p360_debug.h"


#if defined(HAVE_LIBJPEG)

#include <jpeglib.h>
#include <setjmp.h>

#define kModuleNameLibjpeg "libjpeg"

static P360_ERROR_CODE decode_jpeg(const char* filename, uint8_t** rawdata, size_t *width, size_t *height, P360_PIXEL_FORMAT* pixelFormat);

P360_ERROR_CODE libjpegDecodeFile(const char* filePath, P360ObjWrapper<P360Image*>* image)
{
    P360_ERROR_CODE err_ = P360_ERROR_CODE_S_OK;
    uint8_t* rawdata_ = NULL;
    size_t width_ = 0, height_ = 0, size_ = 0;
    P360_PIXEL_FORMAT pixelFormat_ = P360_PIXEL_FORMAT_NONE;
    P360ObjWrapper<P360Image*> image_;
    P360ObjWrapper<P360Buffer*> buffer_;

    if (!filePath || !image) {
        P360_DEBUG_ERROR_EX(kModuleNameLibjpeg, "Invalid parameter");
        P360_CHECK_CODE_BAIL(err_ = P360_ERROR_CODE_E_INVALID_PARAMETER);
    }

    P360_CHECK_CODE_BAIL(err_ = decode_jpeg(filePath, &rawdata_, &width_, &height_, &pixelFormat_));
    P360_CHECK_CODE_BAIL(err_ = P360Image::getSizeForPixelFormat(pixelFormat_, width_, height_, &size_));
    P360_CHECK_CODE_BAIL(err_ = P360Image::newObj(P360_IMAGE_FORMAT_RAW, pixelFormat_, &image_));
    P360_CHECK_CODE_BAIL(err_ = P360Buffer::newObjAndTakeData((void**)&rawdata_, size_, &buffer_));
    P360_CHECK_CODE_BAIL(err_ = image_->setBuffer(buffer_, width_, height_));

    *image = image_;

bail:
    P360Mem::Free((void**)&rawdata_);
    return err_;
}


struct my_error_mgr {
    struct jpeg_error_mgr pub;	/* "public" fields */

    jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit(j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr)cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message) (cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

static P360_ERROR_CODE decode_jpeg(const char* filename, uint8_t** rawdata, size_t *width, size_t *height, P360_PIXEL_FORMAT* pixelFormat)
{
    struct jpeg_decompress_struct cinfo = { 0 };
    /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct my_error_mgr jerr = { 0 };
    /* More stuff */
    FILE * infile = nullptr;		/* source file */
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */
    P360_ERROR_CODE err = P360_ERROR_CODE_S_OK;

    /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */

    if (!rawdata || *rawdata || !width || !height || !pixelFormat) {
        P360_DEBUG_ERROR_EX(kModuleNameLibjpeg, "Invalid parameter");
        P360_CHECK_CODE_BAIL(err = P360_ERROR_CODE_E_INVALID_PARAMETER);
    }

    if ((infile = fopen(filename, "rb")) == NULL) {
        P360_DEBUG_ERROR_EX(kModuleNameLibjpeg, "Can't open %s", filename);
        P360_CHECK_CODE_BAIL(err = P360_ERROR_CODE_E_FAILED_TO_OPEN_FILE);
    }

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        P360_DEBUG_ERROR_EX(kModuleNameLibjpeg, "setjmp failed");
        P360_CHECK_CODE_BAIL(err = P360_ERROR_CODE_E_SYSTEM);
    }
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */

    (void)jpeg_read_header(&cinfo, TRUE);

    // For now only RGB is supported
    switch (cinfo.out_color_space) {
    case JCS_RGB:
    case JCS_EXT_RGB:
        *pixelFormat = P360_PIXEL_FORMAT_R8G8B8;
        break;
    case JCS_EXT_BGR:
        *pixelFormat = P360_PIXEL_FORMAT_B8G8R8;
        break;
    case JCS_EXT_RGBA:
    case JCS_EXT_RGBX:
        *pixelFormat = P360_PIXEL_FORMAT_R8G8B8A8;
        break;
    case JCS_EXT_BGRA:
    case JCS_EXT_BGRX:
        *pixelFormat = P360_PIXEL_FORMAT_B8G8R8A8;
        break;
    case JCS_EXT_ABGR:
    case JCS_EXT_XBGR:
        *pixelFormat = P360_PIXEL_FORMAT_A8B8G8R8;
        break;
    case JCS_EXT_ARGB:
    case JCS_EXT_XRGB:
        *pixelFormat = P360_PIXEL_FORMAT_A8R8G8B8;
        break;
    default:
        P360_DEBUG_ERROR_EX(kModuleNameLibjpeg, "Invalid color space %d", cinfo.out_color_space);
        P360_CHECK_CODE_BAIL(err = P360_ERROR_CODE_E_INVALID_PIXEL_FORMAT);
        break;
    }

    /* Step 5: Start decompressor */

    (void)jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
             ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    *rawdata = (uint8_t*)P360Mem::Malloc(row_stride * cinfo.output_height);
    if (!*rawdata) {
        P360_DEBUG_ERROR_EX(kModuleNameLibjpeg, "Failed to allocate %d bytes", row_stride * cinfo.output_height);
        P360_CHECK_CODE_BAIL(err = P360_ERROR_CODE_E_OUT_OF_MEMORY);
    }
    *width = cinfo.output_width;
    *height = cinfo.output_height;

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy((*rawdata) + ((cinfo.output_scanline - 1) * row_stride), buffer[0], row_stride);
    }

bail:
    /* Step 7: Finish decompression */

    (void)jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    jpeg_destroy_decompress(&cinfo);
    if (infile) {
        fclose(infile);
    }

    if (P360_ERROR_CODE_IS_NOK(err)) {
        P360Mem::Free((void**)rawdata);
    }

    return err;
}

#endif /* HAVE_LIBJPEG */
