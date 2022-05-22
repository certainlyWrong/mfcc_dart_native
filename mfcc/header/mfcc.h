/**
 * @file mfcc.h
 * @author Adriano R. de Sousa (adrianorodriguesdsousa@gmail.com)
 * @date 2022-05-20
 * @version 0.0.1
 */

// The MIT License (MIT)

// Copyright (c) 2022 Adriano R. de Sousa

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#ifndef _MFCC_
#include <stdint.h>

#ifdef _MFCC_private
#include <math.h>
#include <stdlib.h>
#include <complex.h>

// TODO arquitetar e implementar estrutura para representar um quadro

typedef struct
{
	double **frameMatrix;
	uint16_t fftSize, qtddFrames;
} frames;

double *MFCC_cp_buffer(double *Buffer, int sizeBuffer);
void MFCC_normilize(MFCC_input *signal);
#endif // _MFCC_private

typedef enum
{
	FALSE,
	TRUE
} Bool;

typedef struct
{
	double *buffer;
	uint16_t size;
} MFCC_coef;

typedef struct
{
	double *buffer;
	size_t sizeBuffer;
	uint16_t hopSize,
		fftSize,
		dctFilterNum,
		melFilterNum;
	Bool normilizeActivate;
} MFCC_input;

MFCC_input *MFCC_initDefaultValues(double *buffer, size_t sizeBuffer);

MFCC_coef MFCC_execute(MFCC_input *signal);

void MFCC_input_free(MFCC_input *signal);

#endif // _MFCC_
