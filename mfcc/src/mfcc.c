/**
 * @file mfcc.c
 * @author Adriano R. de Sousa (adrianorodriguesdsousa@gmail.com)
 * @brief Definição das funções para processamento de MFCCs
 * @version 0.1
 * @date 2022-05-22
 *
 * @copyright Copyright (c) 2022
 */

#define _MFCC_debug
#define _MFCC_private
#include "../header/mfcc.h"

/**
 * @brief Função para inicializar uma estrutura de entrada com valores predeterminados
 *
 * @param buffer buffer para formar a estrutura de entrada
 * @param sizeBuffer tamanho do buffer
 * @return MFCC_input* inicialização de uma estrurura de entrada para usar como
 * entrada na função de extração de caracteristicas
 */
MFCC_input *MFCC_initDefaultValues(double *buffer, size_t sizeBuffer)
{
  MFCC_input *aux = (MFCC_input *)malloc(sizeof(MFCC_input));

  aux->hopSize = 15;
  aux->fftSize = 2048;
  aux->buffer = buffer;
  aux->dctFilterNum = 20;
  aux->melFilterNum = 10;
  aux->sampleRate = 22050;
  aux->sizeBuffer = sizeBuffer;
  aux->normilizeActivate = TRUE;
  return aux;
}

/**
 * @brief função interna para normalização de um sinal
 *
 * @param signal
 */
void MFCC_normilize(MFCC_input *signal)
{
  double maxValue = 0;
  int aux = 0;

  for (size_t i = 0; i < signal->sizeBuffer; i++)
  {
    aux = MFCC_myDoubleABS((signal->buffer[i]));
    if (aux > maxValue)
      maxValue = aux;
  }

  for (size_t i = 0; i < signal->sizeBuffer; i++)
    signal->buffer[i] /= maxValue;
}

/**
 * @brief
 *
 * @param signal estrutura de entrada
 * @param mode uma string contendo "zeros" ou "reflect"
 */
void MFCC_array_pad(MFCC_input *signal, char *mode)
{
  size_t padSize = signal->fftSize / 2,
         bufferAuxMaxSize = signal->sizeBuffer + signal->fftSize;

  double *bufferAux = (double *)malloc(bufferAuxMaxSize * sizeof(double));

  Bool inter = FALSE;

  if (strcmp(mode, "zeros") == 0)
  {
    inter = TRUE;

    for (size_t min = 0; min < padSize; min++)
      bufferAux[min] = 0;

    for (
        size_t min = padSize,
               max = signal->sizeBuffer + padSize;
        min < max;
        min++)
      bufferAux[min] = signal->buffer[min];

    for (
        uint64_t min = signal->sizeBuffer + padSize,
                 max = bufferAuxMaxSize;
        min < max;
        min++)
      bufferAux[min] = signal->buffer[min];
  }

  if (strcmp(mode, "reflect") == 0)
  {
    inter = TRUE;

    for (
        size_t i = 0,
               j = padSize,
               k = signal->sizeBuffer - padSize - 1,
               m = bufferAuxMaxSize - 1;
        i < padSize;
        i++, j--, k++, m--)
    {
      bufferAux[i] = signal->buffer[j];
      bufferAux[m] = signal->buffer[k];
    }
  }

  if (inter)
  {
    free(signal->buffer);
    signal->buffer = bufferAux;
    signal->sizeBuffer = bufferAuxMaxSize;
  }
}

/**
 * @brief Função interna para fatiar o sinal
 *
 * @param signal
 * @return MFCC_frames
 */
void MFCC_frames_init(MFCC_input *signal, MFCC_frames **output)
{
  MFCC_array_pad(signal, "reflect");

  (*output) = (MFCC_frames *)malloc(sizeof(MFCC_frames));
  (*output)->fftSize = signal->fftSize;
  (*output)->len =
      (uint16_t)MFCC_myRound((double)(signal->sampleRate * signal->hopSize) / 1000);
  (*output)->num =
      (uint16_t)MFCC_myRound(((double)(signal->sizeBuffer - signal->fftSize) / (*output)->len) + 1);
  (*output)->frameMatrix = (double **)malloc(sizeof(double *) * (*output)->num);
  for (size_t i = 0; i < (*output)->num; i++)
  {
    (*output)->frameMatrix[i] = (double *)malloc(sizeof(double) * signal->fftSize);
    MFCC_myBufferCP((*output)->frameMatrix[i], signal->buffer + (i * (*output)->len), signal->fftSize);
  }
}

/**
 * @brief Gera um janelamento de Hann
 *
 * @param fftSize tamanho da janela
 * @return HannWindow*
 */
HannWindow *MFCC_hanning(uint32_t fftSize)
{
  HannWindow *aux = (HannWindow *)malloc(sizeof(HannWindow));
  aux->window = (double *)malloc(sizeof(double) * fftSize);
  aux->fftSize = fftSize;

  for (size_t i = 0; i < fftSize; i++)
    aux->window[i] = 0.5 - 0.5 * cos((2 * M_PI * i) / fftSize);
  return aux;
}

/**
 * @brief Para multiplicar cada quadro pela janela de Hann
 *
 * @param frames
 * @param window
 */
void MFCC_multiplyFramesForHannWindow(MFCC_frames **frames, HannWindow *window)
{
  for (
      size_t i = 0, num = (*frames)->num, fftSize = window->fftSize;
      i < num; i++)
  {
    for (size_t j = 0; j < fftSize; j++)
      (*frames)->frameMatrix[i][j] *= window->window[j];
  }
}

/**
 * @brief Inicia uma matrix de numeros complexos a partir de um vetor de numeros reais
 *
 * @param frames
 * @return MatrixComplex
 */
MatrixComplex *MFCC_initDefaultComplexMatrix(MFCC_frames *frames)
{
  MatrixComplex *matrixComplex = (MatrixComplex *)malloc(sizeof(MatrixComplex));
  matrixComplex->rowNum = frames->num;

  matrixComplex->rows = (ArrayComplex *)malloc(sizeof(ArrayComplex) * frames->num);
  for (size_t i = 0; i < frames->num; i++)
  {
    matrixComplex->rows[i].columns = (double complex *)malloc(sizeof(double complex) * frames->fftSize);
    for (size_t j = 0; j < frames->fftSize; j++)
      matrixComplex->rows[i].columns[j] = CMPLX(frames->frameMatrix[i][j], 0);
  }

  return matrixComplex;
}

/**
 * @brief Calcula a transformada de Fourier para um buffer que tem a quantidade
 * de elementos igual a uma potencia de 2
 *
 * @param matrixComplex buffer
 * @return MatrixComplex*
 */
MatrixComplex *MFCC_fft_cooleyTukey(MatrixComplex *buffer)
{
  for (
      size_t i = 0,
             rowsQuant = buffer->rowNum,
             columnQuant = buffer->rows->columnNum;
      i < rowsQuant; i++)
  {
    for (size_t j = 0; j < columnQuant; j++)
    {
      // TODO implementar um "rowProcess" para calcular a matriz inteira
    }
  }
}

/**
 * @brief Função para extração de MFCCs
 *
 * @param signal estrutura de input que pode ser inicializada com um buffer e valores padrões
 * por meio da função "MFCC_initDefaultValues"
 * @return MFCC_coef
 */
MFCC_coef MFCC_execute(MFCC_input *signal)
{
  // TODO liberar toda memoria alocada
  // TODO verificar vazamentos de memoria

  // Iniciando valor de retorno
  MFCC_coef result = {
      .buffer = NULL,
      .size = signal->dctFilterNum,
      ._status = MFCC_SUCESS,
  };

  if (signal->fftSize % 2 != 0)
  {
    result._status = MFCC_FAILED_POWER_OF_TWO;
    return result;
  }

  if (signal->normilizeActivate)
    MFCC_normilize(signal);

  MFCC_frames *frames = NULL; // * Memoria liberada
  MFCC_frames_init(signal, &frames);

  HannWindow *hannWindow = MFCC_hanning(signal->fftSize); // * Memoria liberada
  MFCC_multiplyFramesForHannWindow(&frames, hannWindow);
  MFCC_free_HannWindow(hannWindow); // * liberando memoria

  MatrixComplex *matrixComplex = MFCC_initDefaultComplexMatrix(frames);
  MFCC_free_MFCC_frames(frames); // * liberando memoria

  printf("aaaa\n");
}

/**
 * @brief Construct a new mfcc free mfcc frames object
 *
 * @param frames
 */
void MFCC_free_MFCC_frames(MFCC_frames *frames)
{
  for (size_t i = 0; i < frames->num; i++)
    free(frames->frameMatrix[i]);
  free(frames->frameMatrix);
  free(frames);
}

/**
 * @brief Construct a new mfcc free hannwindow object
 *
 * @param hannWindow
 */
void MFCC_free_HannWindow(HannWindow *hannWindow)
{
  free(hannWindow->window);
  free(hannWindow);
};

/**
 * @brief Construct a new mfcc free MFCC_input object
 *
 * @param signal
 */
void MFCC_free_input(MFCC_input *signal)
{
  free(signal->buffer);
  free(signal);
}

Bool MFCC_complex_is_even(double complex num)
{
  int a = (int)creal(num);
  return creal(num) == a && a % 2 == 0 ? TRUE : FALSE;
}

/**
 * @brief Retorna o valor positivo
 *
 * @param num
 * @return double
 */
double MFCC_myDoubleABS(double num)
{
  return num < 0 ? (num * -1) : num;
}

/**
 * @brief Arredonda o número
 *
 * @param num
 * @return int
 */
int MFCC_myRound(double num)
{
  return (num - ((int)num)) > .5 ? num + 1 : num;
}

/**
 * @brief Copia os valores do buffer "b" para o buffer "a"
 *
 * @param a buffer que será alterado
 * @param b buffer que terá os valores coM_PIados
 * @param sizeBuffer tamanho dos dois buffers
 */
void MFCC_myBufferCP(double *a, double *b, int sizeBuffer)
{
  for (size_t i = 0; i < sizeBuffer; i++)
    a[i] = b[i];
}
