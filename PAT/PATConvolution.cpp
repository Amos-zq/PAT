//
//  PATConvolution.cpp
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
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
    
    vImageConvolve_PlanarF(&vImageBufferInput, &vImageBufferR, NULL, 0, 0, wavelet->kernelR, wavelet->kernelWidth, wavelet->kernelHeight, 0.0, kvImageBackgroundColorFill);
    vImageConvolve_PlanarF(&vImageBufferInput, &vImageBufferI, NULL, 0, 0, wavelet->kernelI, wavelet->kernelWidth, wavelet->kernelHeight, 0.0, kvImageBackgroundColorFill);
    
    vDSP_vdist(bufferR->data, 1, bufferI->data, 1, output->data, 1, output->width*output->height);
    output->normalize();
}

void PATConvolution::clean_up(void)
{
    bufferR->clean_up();
    bufferI->clean_up();
    delete bufferR;
    delete bufferI;
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