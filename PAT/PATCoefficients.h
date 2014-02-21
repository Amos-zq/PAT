//
//  PATCoefficients.h
//  PAT
//
//  Created by Marcelo Cicconet on 2/21/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
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
    PATWavelet ** kernels;
    PATConvolution * convolution;
    PATImage * input;
    PATImage ** outputs;
    
    int halfWindowSize;
    int nSelectedRows;
    int nSelectedCols;
    int * selectedRows;
    int * selectedCols;
    
    bool dataStructureIsList;
    bool thresholdingIsLocal;
    
    int nCoefficients;
    int * indices;
    float * m;
    float * a;
    float * x;
    float * y;
    
    PATImage * M;
    PATImage * N; // for local thresholding
    PATImage * A;
    PATImage * X;
    PATImage * Y;
    PATImage * IX;
    PATImage * IY;
    int * rowsL;
    int * rowsR;
    int * colsU;
    int * colsB;
public:
    void set_up(int width,
                int height,
                float scale,
                int nOrient,
                int hopSize,
                int halfWS,
                float magThreshold,
                bool dataStructureList,
                bool thresholdingLocal);
    void set_input(PATImage * inputImage);
    void perform_convolutions(void);
    void find_coefficients(void);
    void save_png_to_path(const char * path);
    void clean_up();
};

#endif /* defined(__PAT__PATCoefficients__) */
