//
//  PATCoefficients.cpp
//  PAT
//
//  Created by Marcelo Cicconet on 2/21/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#include "PATCoefficients.h"

void PATCoefficients::set_up(int width,
                             int height,
                             float scale,
                             int nOrient,
                             int hopSize,
                             int halfWS,
                             float magThreshold,
                             bool dataStructureIsList,
                             bool thresholdingIsLocal)
{
    magnitudeThreshold = magThreshold;
    nOrientations = nOrient;
    kernels = (PATWavelet **)malloc(nOrientations*sizeof(PATWavelet *));
    for (int i = 0; i < nOrientations; i++) {
        float orientation = (float)i*180.0/(float)nOrientations;
        kernels[i] = new PATWavelet; kernels[i]->set_up(1, scale, orientation, 1);
    }
    convolution = new PATConvolution;
    convolution->set_up(width, height);
    input = new PATImage; input->set_up_with_data(NULL, width, height);
    outputs = (PATImage **)malloc(nOrientations*sizeof(PATImage *));
    for (int i = 0; i < nOrientations; i++) {
        outputs[i] = new PATImage; outputs[i]->set_up_with_data(NULL, width, height);
    }
    halfWindowSize = theHalfWindowSize;
    int windowSize = halfWS;
    nSelectedRows = floorf((input.height-windowSize)/(float)theHopSize)+1;
    nSelectedCols = floorf((input.width-windowSize)/(float)theHopSize)+1;
    selectedRows = (int *)malloc(nSelectedRows*sizeof(int));
    selectedCols = (int *)malloc(nSelectedCols*sizeof(int));
    
    float firstRow = halfWindowSize;
    float lastRow = input.height-1-halfWindowSize;
    float firstCol = firstRow;
    float lastCol = input.width-1-halfWindowSize;
    float rowStep = (lastRow-firstRow)/(float)(nSelectedRows-1);
    float colStep = (lastCol-firstCol)/(float)(nSelectedCols-1);
    for (int i = 0; i < nSelectedRows; i++) {
        selectedRows[i] = roundf(firstRow+i*rowStep);
    }
    for (int j = 0; j < nSelectedCols; j++) {
        selectedCols[j] = roundf(firstCol+j*colStep);
    }
    
    dataStructureIsList = theDataStructureIsList;
    thresholdingIsLocal = theThresholdingIsLocal;
    
    // matrix data structure
    M = [[OCVFloatImage alloc] initWithData:NULL width:nSelectedCols height:nSelectedRows];
    N = [[OCVFloatImage alloc] initWithData:NULL width:nSelectedCols height:nSelectedRows];
    A = [[OCVFloatImage alloc] initWithData:NULL width:nSelectedCols height:nSelectedRows];
    X = [[OCVFloatImage alloc] initWithData:NULL width:nSelectedCols height:nSelectedRows];
    Y = [[OCVFloatImage alloc] initWithData:NULL width:nSelectedCols height:nSelectedRows];
    IX = [[OCVFloatImage alloc] initWithData:NULL width:input.width height:input.height];
    IY = [[OCVFloatImage alloc] initWithData:NULL width:input.width height:input.height];
    rowsL = (int *)malloc(nSelectedRows*sizeof(int));
    rowsR = (int *)malloc(nSelectedRows*sizeof(int));
    colsU = (int *)malloc(nSelectedCols*sizeof(int));
    colsB = (int *)malloc(nSelectedCols*sizeof(int));
    for (int i = 1; i < nSelectedRows-1; i++) {
        rowsL[i] = ceilf(0.5*((float)selectedRows[i-1]+(float)selectedRows[i]));
        rowsR[i] = ceilf(0.5*((float)selectedRows[i]+(float)selectedRows[i+1]))-1;
    }
    rowsL[0] = 0;
    rowsR[0] = ceilf(0.5*((float)selectedRows[0]+(float)selectedRows[1]))-1;
    rowsL[nSelectedRows-1] = ceilf(0.5*((float)selectedRows[nSelectedRows-2]+(float)selectedRows[nSelectedRows-1]));
    rowsR[nSelectedRows-1] = input.height-1;
    for (int i = 1; i < nSelectedCols-1; i++) {
        colsU[i] = ceilf(0.5*((float)selectedCols[i-1]+(float)selectedCols[i]));
        colsB[i] = ceilf(0.5*((float)selectedCols[i]+(float)selectedCols[i+1]))-1;
    }
    colsU[0] = 0;
    colsB[0] = ceilf(0.5*((float)selectedCols[0]+(float)selectedCols[1]))-1;
    colsU[nSelectedCols-1] = ceilf(0.5*((float)selectedCols[nSelectedCols-2]+(float)selectedCols[nSelectedCols-1]));
    colsB[nSelectedCols-1] = input.width-1;
    
    if (dataStructureIsList) {
        int maxNCoefficients = nSelectedRows*nSelectedCols;
        m = (float *)malloc(maxNCoefficients*sizeof(float));
        a = (float *)malloc(maxNCoefficients*sizeof(float));
        x = (float *)malloc(maxNCoefficients*sizeof(float));
        y = (float *)malloc(maxNCoefficients*sizeof(float));
        indices = (int *)malloc(maxNCoefficients*sizeof(int));
    }
}
