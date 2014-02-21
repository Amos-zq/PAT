//
//  PATImage.h
//  PAT
//
//  Created by Marcelo Cicconet on 2/21/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#ifndef __PAT__PATImage__
#define __PAT__PATImage__

#include <iostream>
#include <Accelerate/Accelerate.h>
#include <ImageIO/ImageIO.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

class PATImage {
    int width;
    int height;
    float * data;
    unsigned char * ucImage;
    CGImageRef cgImageRef;
    CFDataRef data8;
    CGDataProviderRef provider;
public:
    PATImage();
    void set_up_with_path(const char * path);
    void set_up_with_data(float * d, int w, int h);
    void prepare_image_ref(void);
    void save_png_to_path(const char * path);
    vImage_Buffer v_image_buffer_structure(void);
    void normalize(void);
    void clean_up(void);
    float * d() {return data;}
    float w() {return width;}
    float h() {return height;}
};


#endif /* defined(__PAT__PATImage__) */
