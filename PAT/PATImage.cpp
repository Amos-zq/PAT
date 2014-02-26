//
//  PATImage.cpp
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "PATImage.h"

//PATImage::PATImage()
//{
//    width = 0;
//    height = 0;
//    data = NULL;
//    ucImage = NULL;
//    cgImageRef = NULL;
//    data8 = NULL;
//    provider = NULL;
//}

void PATImage::set_up_with_path(const char * path)
{
    CFStringRef filePath = CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, filePath, kCFURLPOSIXPathStyle, false);
    CGImageSourceRef image_source = CGImageSourceCreateWithURL(url, NULL);
    CGImageRef image = CGImageSourceCreateImageAtIndex(image_source, 0, NULL);
    CFRelease(image_source);
    
    width = (int)CGImageGetWidth(image);
    height = (int)CGImageGetHeight(image);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
    unsigned char * rawData = (unsigned char *)malloc(width*height*sizeof(unsigned char));
    unsigned long bytesPerPixel = 1;
    unsigned long bytesPerRow = bytesPerPixel*width;
    unsigned long bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGBitmapByteOrderDefault);
    CGColorSpaceRelease(colorSpace);
    
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
    
    data = (float *)calloc(width*height, sizeof(float));
    
    for (int i = 0; i < width*height; i++) {
        data[i] = (float)rawData[i]/255.0;
    }
    
    CGContextRelease(context);
    free(rawData);
    
    cgImageRef = NULL;
}

void PATImage::set_up_with_data(float * d, int w, int h)
{
    width = w;
    height = h;
    data = (float *)calloc(width*height, sizeof(float));
    if (d) {
        memcpy(data, d, width*height*sizeof(float));
    }
    
    cgImageRef = NULL;
}

void PATImage::copy_from_image(PATImage * image)
{
    // images should be of same size
    memcpy(data, image->data, image->width*image->height*sizeof(float));
}

void PATImage::prepare_image_ref(void)
{
    if (cgImageRef) {
        CGImageRelease(cgImageRef);
        CGDataProviderRelease(provider);
        CFRelease(data8);
        free(ucImage);
    }
    ucImage = (unsigned char *)malloc(width*height*sizeof(unsigned char));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ucImage[i*width+j] = 255*data[i*width+j];
        }
    }
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceGray();
    data8 = CFDataCreate(NULL, ucImage, width*height);
    provider = CGDataProviderCreateWithCFData(data8);
    cgImageRef = CGImageCreate(width, height, 8, 8, width, colorspace, kCGBitmapByteOrderDefault, provider, NULL, true, kCGRenderingIntentDefault);
    CGColorSpaceRelease(colorspace);
}

void PATImage::save_png_to_path(const char * path)
{
    prepare_image_ref();
    
    CFStringRef filePath = CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, filePath, kCFURLPOSIXPathStyle, false);
    CFStringRef type = CFStringCreateWithCString(NULL, "public.png", kCFStringEncodingUTF8);
    CGImageDestinationRef myImageDest = CGImageDestinationCreateWithURL(url, type, 1, NULL);
    CGImageDestinationAddImage(myImageDest, cgImageRef, NULL);
    CGImageDestinationFinalize(myImageDest);
    CFRelease(myImageDest);
}

vImage_Buffer PATImage::v_image_buffer_structure(void)
{
    vImage_Buffer bf;
    bf.data = data;
    bf.height = height;
    bf.width = width;
    bf.rowBytes = width*sizeof(float);
    return bf;
}

void PATImage::normalize(void)
{
    float min = INFINITY;
    float max = -INFINITY;
    for (int i = 0; i < width*height; i++) {
        float v = data[i];
        if (v < min) min = v;
        if (v > max) max = v;
    }
    float range = max-min;
    if (range > 0) {
        for (int i = 0; i < width*height; i++) {
            data[i] = (data[i]-min)/range;
        }
    } else {
        for (int i = 0; i < width*height; i++) {
            data[i] -= min;
        }
    }
}

void PATImage::set_zero(void)
{
    for (int i = 0; i < width*height; i++) {
        data[i] = 0.0;
    }
}

void PATImage::clean_up(void)
{
    if (cgImageRef) {
        CGImageRelease(cgImageRef);
        CGDataProviderRelease(provider);
        CFRelease(data8);
        free(ucImage);
    }
    free(data);
}

//
// Copyright (c) 2014 Marcelo Cicconet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//