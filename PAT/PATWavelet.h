//
//  PATWavelet.h
//  PAT
//
//  Created by Marcelo Cicconet on 2/21/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#ifndef __PAT__PATWavelet__
#define __PAT__PATWavelet__

#include <iostream>
#include <math.h>

class PATWavelet {
    int kernelWidth;
    int kernelHeight;
    float * kernelR;
    float * kernelI;
    float * kernelV;
public:
    PATWavelet();
    void set_up(int stretch, float scale, float orientation, int nPeaks);
    void prepare_to_visualize_kernel(const char * name);
    void clean_up(void);
    int width() {return kernelWidth;}
    int height() {return kernelHeight;}
    float * kR() {return kernelR;}
    float * kI() {return kernelI;}
    float * kV() {return kernelV;}
};

#endif /* defined(__PAT__PATWavelet__) */
