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

int main(int argc, const char * argv[])
{
//    PATImage * image = new PATImage;
//    const char * pathIn = "/Users/Cicconet/Desktop/Image.png";
//    const char * pathOut = "/Users/Cicconet/Desktop/Output.png";
//    image->set_up_with_path(pathIn);
////    image->set_up_with_data(NULL, 400, 300);
//    image->save_png_to_path(pathOut);
//    image->clean_up();
//    delete image;
    
    PATWavelet * wavelet = new PATWavelet;
    wavelet->set_up(1, 10.0, 60.0, 1);
    wavelet->prepare_to_visualize_kernel("real");
    PATImage * image = new PATImage;
    image->set_up_with_data(wavelet->kV(), wavelet->width(), wavelet->height());
    image->save_png_to_path("/Users/Cicconet/Desktop/Kernel.png");
    image->clean_up();
    delete image;

    return 0;
}

