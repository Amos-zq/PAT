//
//  PATImage.h
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#ifndef __PAT__PATImage__
#define __PAT__PATImage__

#include <iostream>
#include <Accelerate/Accelerate.h>
#include <ImageIO/ImageIO.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

class PATImage {
private:
    int width;
    int height;
    float * data;
    unsigned char * ucImage;
    CGImageRef cgImageRef;
    CFDataRef data8;
    CGDataProviderRef provider;
public:
//    PATImage();
    void set_up_with_path(const char * path);
    void set_up_with_data(float * d, int w, int h);
    void copy_from_image(PATImage * image);
    void prepare_image_ref(void);
    void save_png_to_path(const char * path);
    vImage_Buffer v_image_buffer_structure(void);
    void normalize(void);
    void set_zero(void);
    void clean_up(void);
    inline float * d() {return data;}
    inline int w() {return width;}
    inline int h() {return height;}
};

#endif /* defined(__PAT__PATImage__) */

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