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
                             bool dataStructureList,
                             bool thresholdingLocal)
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
    halfWindowSize = halfWS;
    nSelectedRows = floorf((height-2*halfWindowSize)/(float)hopSize)+1;
    nSelectedCols = floorf((width-2*halfWindowSize)/(float)hopSize)+1;
    selectedRows = (int *)malloc(nSelectedRows*sizeof(int));
    selectedCols = (int *)malloc(nSelectedCols*sizeof(int));
    
    float firstRow = halfWindowSize;
    float lastRow = height-1-halfWindowSize;
    float firstCol = firstRow;
    float lastCol = width-1-halfWindowSize;
    float rowStep = (lastRow-firstRow)/(float)(nSelectedRows-1);
    float colStep = (lastCol-firstCol)/(float)(nSelectedCols-1);
    for (int i = 0; i < nSelectedRows; i++) {
        selectedRows[i] = roundf(firstRow+i*rowStep);
    }
    for (int j = 0; j < nSelectedCols; j++) {
        selectedCols[j] = roundf(firstCol+j*colStep);
    }
    
    dataStructureIsList = dataStructureList;
    thresholdingIsLocal = thresholdingLocal;
    
    // matrix data structure
    M = new PATImage;   M->set_up_with_data(NULL, nSelectedCols, nSelectedRows);
    N = new PATImage;   N->set_up_with_data(NULL, nSelectedCols, nSelectedRows);
    A = new PATImage;   A->set_up_with_data(NULL, nSelectedCols, nSelectedRows);
    X = new PATImage;   X->set_up_with_data(NULL, nSelectedCols, nSelectedRows);
    Y = new PATImage;   Y->set_up_with_data(NULL, nSelectedCols, nSelectedRows);
    IX = new PATImage;  IX->set_up_with_data(NULL, width, height);
    IY = new PATImage;  IY->set_up_with_data(NULL, width, height);
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
    rowsR[nSelectedRows-1] = height-1;
    for (int i = 1; i < nSelectedCols-1; i++) {
        colsU[i] = ceilf(0.5*((float)selectedCols[i-1]+(float)selectedCols[i]));
        colsB[i] = ceilf(0.5*((float)selectedCols[i]+(float)selectedCols[i+1]))-1;
    }
    colsU[0] = 0;
    colsB[0] = ceilf(0.5*((float)selectedCols[0]+(float)selectedCols[1]))-1;
    colsU[nSelectedCols-1] = ceilf(0.5*((float)selectedCols[nSelectedCols-2]+(float)selectedCols[nSelectedCols-1]));
    colsB[nSelectedCols-1] = width-1;
    
    if (dataStructureIsList) {
        int maxNCoefficients = nSelectedRows*nSelectedCols;
        m = (float *)malloc(maxNCoefficients*sizeof(float));
        a = (float *)malloc(maxNCoefficients*sizeof(float));
        x = (float *)malloc(maxNCoefficients*sizeof(float));
        y = (float *)malloc(maxNCoefficients*sizeof(float));
        indices = (int *)malloc(maxNCoefficients*sizeof(int));
    }
}

void PATCoefficients::set_input(PATImage * inputImage)
{
    input->copy_from_image(inputImage);
}

void PATCoefficients::perform_convolutions(void)
{
    for (int i = 0; i < nOrientations; i++) {
        convolution->convolve(input, kernels[i], outputs[i]);
    }
}

void PATCoefficients::find_coefficients(void)
{
    int index;
    float magnitude;
    int nCols = input->w();
    float globalMaxMag = -INFINITY;
    for (int i = 0; i < nSelectedRows; i++) {
        for (int j = 0; j < nSelectedCols; j++) {
            int row1 = selectedRows[i]-halfWindowSize;
            int row2 = selectedRows[i]+halfWindowSize;
            int col1 = selectedCols[j]-halfWindowSize;
            int col2 = selectedCols[j]+halfWindowSize;
            float blockMaxMag = -INFINITY;
            int maxK, maxRow, maxCol;
            for (int row = row1; row <= row2; row++) {
                for (int col = col1; col <= col2; col++) {
                    for (int k = 0; k < nOrientations; k++) {
                        magnitude = outputs[k]->d()[row*nCols+col];
                        if (magnitude > blockMaxMag) {
                            blockMaxMag = magnitude;
                            maxK = k;
                            maxRow = row;
                            maxCol = col;
                        }
                    }
                }
            }
            index = i*nSelectedCols+j;
            M->d()[index] = blockMaxMag;
            A->d()[index] = maxK*M_PI/(float)nOrientations+M_PI_2;
            X->d()[index] = maxRow; // X0: selectedRows[i];
            Y->d()[index] = maxCol; // Y0: selectedCols[j];
            if (blockMaxMag > globalMaxMag) globalMaxMag = blockMaxMag;
            for (int row = rowsL[i]; row <= rowsR[i]; row++) {
                for (int col = colsU[j]; col <= colsB[j]; col++) {
                    IX->d()[row*nCols+col] = i;
                    IY->d()[row*nCols+col] = j;
                }
            }
        }
    }
    if (thresholdingIsLocal) {
        N->set_zero();
        float value;
        float threshold = magnitudeThreshold*globalMaxMag;
        for (int i = 1; i < nSelectedRows-1; i++) {
            for (int j = 1; j < nSelectedCols-1; j++) {
                float locMax = -INFINITY;
                float locMin = INFINITY;
                for (int ii = -1; ii < 2; ii++) {
                    for (int jj = -1; jj < 2; jj++) {
                        value = M->d()[(i+ii)*nSelectedCols+(j+jj)];
                        if (value > locMax) locMax = value;
                        if (value < locMin) locMin = value;
                    }
                }
                int entryIndex = i*nSelectedCols+j;
                value = M->d()[entryIndex];
                if (value > 0.75*locMax && locMin > threshold) {
                    N->d()[entryIndex] = value;
                }
            }
        }
        M->copy_from_image(N);
        for (int i = 1; i < nSelectedRows-1; i++) {
            for (int j = 1; j < nSelectedCols-1; j++) {
                M->d()[i*nSelectedCols+j] = (M->d()[i*nSelectedCols+j] > 0 ? 1.0 : 0.0);
            }
        }
    }
    if (dataStructureIsList) {
        int memsize = (index+1)*sizeof(float);
        memcpy(m, M->d(), memsize);
        memcpy(a, A->d(), memsize);
        memcpy(x, X->d(), memsize);
        memcpy(y, Y->d(), memsize);
        
        nCoefficients = 0;
        float threshold = magnitudeThreshold*globalMaxMag;
        for (int i = 0; i < index; i++) {
            if (m[i] > threshold) {
                indices[nCoefficients] = i;
                nCoefficients += 1;
            }
        }
        
        // permutate coefficient indices
        srand((unsigned)time(NULL));
        for (int i = nCoefficients; i > 0; i--) {
            int randIndex = rand() % i; // rand number in {0,...,i-1}
            int memIndex = indices[i-1];
            indices[i-1] = indices[randIndex];
            indices[randIndex] = memIndex;
        }
    }
}

void PATCoefficients::save_png_to_path(const char * path)
{
    int factor = 2;
    PATImage image;
    image.set_up_with_data(NULL,factor*input->w(),factor*input->h());
    if (dataStructureIsList) {
        for (int i = 0; i < nCoefficients; i++) {
            int row0 = factor*x[indices[i]];
            int col0 = factor*y[indices[i]];
            for (int j = -factor; j < factor; j++) {
                int row = row0+roundf(j*cosf(a[indices[i]]));
                int col = col0+roundf(j*sinf(a[indices[i]]));
                image.d()[row*image.w()+col] = m[indices[i]];
            }
        }
    } else {
        for (int i = 0; i < nSelectedRows; i++) {
            for (int j = 0; j < nSelectedCols; j++) {
                int index = i*X->w()+j;
                int row0 = factor*X->d()[index];
                int col0 = factor*Y->d()[index];
                for (int k = -factor; k < factor; k++) {
                    int row = row0+roundf(k*cosf(A->d()[index]));
                    int col = col0+roundf(k*sinf(A->d()[index]));
                    image.d()[row*image.w()+col] = M->d()[index];
                }
            }
        }
    }
    image.save_png_to_path(path);
    image.clean_up();
}

void PATCoefficients::clean_up()
{
    if (dataStructureIsList) {
        free(indices);
        free(m);
        free(a);
        free(x);
        free(y);
    }
    
    M->clean_up(); delete M;
    N->clean_up(); delete N;
    A->clean_up(); delete A;
    X->clean_up(); delete X;
    Y->clean_up(); delete Y;
    IX->clean_up(); delete IX;
    IY->clean_up(); delete IY;
    free(rowsL);
    free(rowsR);
    free(colsU);
    free(colsB);
    
    free(selectedRows);
    free(selectedCols);
    input->clean_up(); delete input;
    for (int i = 0; i < nOrientations; i++) {
        outputs[i]->clean_up(); delete outputs[i];
    }
    free(outputs);
    for (int i = 0; i < nOrientations; i++) {
        kernels[i]->clean_up(); delete kernels[i];
    }
    free(kernels);
}
