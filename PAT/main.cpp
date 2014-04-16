//
//  main.cpp
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include <iostream>
#include "PATImage.h"
#include "PATWavelet.h"
#include "PATConvolution.h"
#include "PATCoefficients.h"

void image(void);
void wavelet(void);
void convolution(void);
void coefficients(void);

// to compile on Terminal, type
// g++ *.cpp -o pat -I/usr/local/include -L/opt/local/lib -lpng

int main(int argc, const char * argv[])
{
//    image();
//    wavelet();
//    convolution();
    coefficients();

    return 0;
}

void image(void)
{
    PATImage im;
    im.set_up_with_path("/Users/Cicconet/Desktop/Image.png");
//    im.set_up_with_data(NULL, 400, 300);
    im.save_png_to_path("/Users/Cicconet/Desktop/Output.png");
    im.clean_up();
}

void wavelet(void)
{
    int nOrient = 16;
    for (int i = 0; i < nOrient; i++) {
        PATWavelet wav;
        wav.set_up(1, 10.0, (float)i*360.0/(float)nOrient, 1);
        wav.prepare_to_visualize_kernel("imaginary");
        PATImage image;
        image.set_up_with_data(wav.kernelV, wav.width, wav.height);
        char path[100];
        sprintf(path, "/Users/Cicconet/Desktop/Wavelets/Kernel%02d.png", i);
        image.save_png_to_path(path);
        image.clean_up();
        wav.clean_up();
    }
}

void convolution(void)
{
    PATWavelet wavelet;
    wavelet.set_up(1, 1.0, 0.0, 1);
    PATImage image;
    image.set_up_with_path("/Users/Cicconet/Desktop/Image.png");
    PATConvolution conv;
    conv.set_up(image.width, image.height, wavelet.width, wavelet.height);
    PATImage output;
    output.set_up_with_data(NULL, image.width, image.height);
    bool ignoreDirection = false;
    conv.convolve(image, wavelet, output, ignoreDirection);
    output.normalize();
    output.save_png_to_path("/Users/Cicconet/Desktop/Output.png");
    conv.clean_up();
    image.clean_up();
    wavelet.clean_up();
}

void coefficients(void)
{
    PATImage image;
    image.set_up_with_path("/Users/Cicconet/Desktop/Image.png");
    PATCoefficients coef;
    float scale = 2.0;
    int nOrient = 16;
    int hopSize = 5;
    int halfWS = 1;
    float magThreshold = 0.01;
    bool dataStructureList = true;
    bool thresholdingLocal = true;
    bool ignoreDirection = false;
    coef.set_up(image.width, image.height, scale, nOrient, hopSize, halfWS, magThreshold, dataStructureList, thresholdingLocal, ignoreDirection);
    coef.set_input(image);
    coef.perform_convolutions();
    coef.find_coefficients();
    coef.save_png_to_path("/Users/Cicconet/Desktop/Output.png");
    coef.clean_up();
    image.clean_up();
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