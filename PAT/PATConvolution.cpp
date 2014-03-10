//
//  PATConvolution.cpp
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "PATConvolution.h"

void PATConvolution::set_up(int imageWidth, int imageHeight)
{
    width = imageWidth;
    height = imageHeight;
    bufferR.set_up_with_data(NULL, width, height);
    bufferI.set_up_with_data(NULL, width, height);
}

void PATConvolution::convolve(PATImage input, PATWavelet wavelet, PATImage output)
{
    bufferR.set_zero();
    bufferI.set_zero();
    
    int khw = floorf(wavelet.width/2.0); // kernel half width
    int khh = floorf(wavelet.height/2.0); // kernel half height
    
    
    float accR, accI;
    int indexInputRow, indexInput, indexKernelRow, indexKernel;
    for (int i = khh; i < input.height-khh; i++) {
        for (int j = khw; j < input.width-khw; j++) {
            accR = 0.0;
            accI = 0.0;
            for (int ii = -khh; ii <= khh ; ii++) {
                indexInputRow = (i+ii)*input.width+j;
                indexKernelRow = (khh+ii)*wavelet.width+khw;
                for (int jj = -khw; jj <= khw; jj++) {
                    indexInput = indexInputRow+jj;
                    indexKernel = indexKernelRow+jj;
                    accR += input.data[indexInput]*wavelet.kernelR[indexKernel];
                    accI += input.data[indexInput]*wavelet.kernelI[indexKernel];
                }
            }
            output.data[i*output.width+j] = sqrtf(accR*accR+accI*accI);
        }
    }
    
    output.normalize();
}

void PATConvolution::clean_up(void)
{
    bufferR.clean_up();
    bufferI.clean_up();
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