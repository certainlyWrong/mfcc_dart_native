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
#define _MFCC_

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Facilita a leitura de valores booleano
 */
typedef enum
{
	FALSE,
	TRUE
} Bool;

/**
 * @brief Para determinar o tipo de retorno da função "MFCC_execute"
 *
 */
typedef enum
{
	MFCC_SUCESS,
	MFCC_FAILED_POWER_OF_TWO,
} MFCC_status;

/**
 * @brief Estrutura com o resultado da "MFCC_execute"
 *
 */
typedef struct
{
	double *buffer;
	uint16_t size;
	MFCC_status _status;
} MFCC_coef;

/**
 * @brief  Estrutura com os valores de entrada da "MFCC_execute"
 *
 */
typedef struct
{
	double *buffer;
	size_t sizeBuffer;
	uint32_t hopSize,
			fftSize,
			dctFilterNum,
			melFilterNum;
	uint64_t sampleRate;
	Bool normilizeActivate;
} MFCC_input;

MFCC_input *MFCC_initDefaultValues(double *buffer, size_t sizeBuffer);
MFCC_coef MFCC_execute(MFCC_input *signal);
void MFCC_free_input(MFCC_input *signal);

#ifdef _MFCC_private

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>

/**
 * @brief Estrutura interna para lidar com o fatiamento da áudio
 *
 */
typedef struct
{
	double **frameMatrix;
	uint16_t len, num, fftSize;
} MFCC_frames;

typedef struct
{
	double *window;
	uint32_t fftSize;
} HannWindow;

// TODO implementar free
typedef struct
{
	double complex *columns;
	uint32_t columnNum;
} ArrayComplex;

// TODO implementar free
typedef struct
{
	ArrayComplex *rows;
	uint32_t rowNum;
} MatrixComplex;

void MFCC_frames_init(MFCC_input *signal, MFCC_frames **output);
void MFCC_normilize(MFCC_input *signal);
void MFCC_array_pad(MFCC_input *signal, char *mode);
int MFCC_myRound(double num);
double MFCC_myDoubleABS(double num);
void MFCC_myBufferCP(double *a, double *b, int sizeBuffer);
HannWindow *MFCC_hanning(uint32_t fftSize);
void MFCC_multiplyFramesForHannWindow(MFCC_frames **frames, HannWindow *window);
MatrixComplex *MFCC_initDefaultComplexMatrix(MFCC_frames *frames);
MatrixComplex *MFCC_fft_cooleyTukey(MatrixComplex *buffer);
Bool MFCC_complex_is_even(double complex num);

void MFCC_free_HannWindow(HannWindow *hannWindow);
void MFCC_free_MFCC_frames(MFCC_frames *frames);
#ifdef _MFCC_debug
#include <stdio.h>
#endif // _MFCC_debug
#endif // _MFCC_private

#endif // _MFCC_
