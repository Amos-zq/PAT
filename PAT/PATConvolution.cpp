//
//  PATConvolution.cpp
//  PAT
//
//  Created by Marcelo Cicconet on 2/21/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#include "PATConvolution.h"

//PATConvolution::PATConvolution()
//{
//    width = 0;
//    height = 0;
//    bufferR = NULL;
//    bufferI = NULL;
//}

void PATConvolution::set_up(int imageWidth, int imageHeight)
{
    width = imageWidth;
    height = imageHeight;
    bufferR = new PATImage;
    bufferR->set_up_with_data(NULL, width, height);
    bufferI = new PATImage;
    bufferI->set_up_with_data(NULL, width, height);
    vImageBufferR = bufferR->v_image_buffer_structure();
    vImageBufferI = bufferI->v_image_buffer_structure();
}

void PATConvolution::convolve(PATImage * input, PATWavelet * wavelet, PATImage * output)
{
    vImage_Buffer vImageBufferInput = input->v_image_buffer_structure();
    
    vImageConvolve_PlanarF(&vImageBufferInput, &vImageBufferR, NULL, 0, 0, wavelet->kR(), wavelet->width(), wavelet->height(), 0.0, kvImageBackgroundColorFill);
    vImageConvolve_PlanarF(&vImageBufferInput, &vImageBufferI, NULL, 0, 0, wavelet->kI(), wavelet->width(), wavelet->height(), 0.0, kvImageBackgroundColorFill);
    
    vDSP_vdist(bufferR->d(), 1, bufferI->d(), 1, output->d(), 1, output->w()*output->h());
    output->normalize();
}

void PATConvolution::clean_up(void)
{
    bufferR->clean_up();
    bufferI->clean_up();
    delete bufferR;
    delete bufferI;
}
