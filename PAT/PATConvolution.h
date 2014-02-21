//
//  PATConvolution.h
//  PAT
//
//  Created by Marcelo Cicconet on 2/21/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#ifndef __PAT__PATConvolution__
#define __PAT__PATConvolution__

#include <iostream>
#include <Accelerate/Accelerate.h>
#include "PATImage.h"
#include "PATWavelet.h"

class PATConvolution {
    int width;
    int height;
    PATImage * bufferR;
    PATImage * bufferI;
    vImage_Buffer vImageBufferR;
    vImage_Buffer vImageBufferI;
public:
    PATConvolution();
    void set_up(int imageWidth, int imageHeight);
    void convolve(PATImage * input, PATWavelet * wavelet, PATImage * output);
    void clean_up(void);
};

#endif /* defined(__PAT__PATConvolution__) */
