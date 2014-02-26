//
//  PATCoefficients.h
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#ifndef __PAT__PATCoefficients__
#define __PAT__PATCoefficients__

#include <iostream>
#include "PATImage.h"
#include "PATWavelet.h"
#include "PATConvolution.h"

class PATCoefficients {
    float magnitudeThreshold;
    
    int nOrientations;
    PATWavelet * kernels;
    PATConvolution convolution;
    PATImage input;
    PATImage * outputs;
    
    int halfWindowSize;
    int nSelectedRows;
    int nSelectedCols;
    int * selectedRows;
    int * selectedCols;
    
    bool dataStructureIsList;
    bool thresholdingIsLocal;
    
    PATImage M;
    PATImage N; // for local thresholding
    PATImage A;
    PATImage X;
    PATImage Y;
    PATImage IX;
    PATImage IY;
    int * rowsL;
    int * rowsR;
    int * colsU;
    int * colsB;
public:
    int nCoefficients;
    int * indices;
    float * m;
    float * a;
    float * x;
    float * y;
    void set_up(int width,
                int height,
                float scale,
                int nOrient,
                int hopSize,
                int halfWS,
                float magThreshold,
                bool dataStructureList,
                bool thresholdingLocal);
    void set_input(PATImage inputImage);
    void perform_convolutions(void);
    void find_coefficients(void);
    void save_png_to_path(const char * path);
    void clean_up();
};

#endif /* defined(__PAT__PATCoefficients__) */

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