//
//  main.cpp
//  PAT
//
//  Created by Marcelo Cicconet on 2/20/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
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

int main(int argc, const char * argv[])
{
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
    PATWavelet wav;
    wav.set_up(1, 10.0, 60.0, 1);
    wav.prepare_to_visualize_kernel("real");
    PATImage image;
    image.set_up_with_data(wav.kV(), wav.width(), wav.height());
    image.save_png_to_path("/Users/Cicconet/Desktop/Kernel.png");
    image.clean_up();
    wav.clean_up();
}

void convolution(void)
{
    PATWavelet wavelet;
    wavelet.set_up(1, 0.5, 90.0, 1);
    PATImage image;
    image.set_up_with_path("/Users/Cicconet/Desktop/Image.png");
    PATConvolution conv;
    conv.set_up(image.w(), image.h());
    PATImage output;
    output.set_up_with_data(NULL, image.w(), image.h());
    conv.convolve(&image, &wavelet, &output);
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
    float scale = 1.0;
    int nOrient = 16;
    int hopSize = 5;
    int halfWS = 1;
    float magThreshold = 0.01;
    bool dataStructureList = true;
    bool thresholdingLocal = true;
    coef.set_up(image.w(), image.h(), scale, nOrient, hopSize, halfWS, magThreshold, dataStructureList, thresholdingLocal);
    coef.set_input(&image);
    coef.perform_convolutions();
    coef.find_coefficients();
    coef.save_png_to_path("/Users/Cicconet/Desktop/Output.png");
    coef.clean_up();
    image.clean_up();
}
