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
 * @brief Para liberar a memoria da estrutura de entrada
 *
 * @param signal
 */
void MFCC_input_free(MFCC_input *signal)
{
  free(signal->buffer);
  free(signal);
}

/**
 * @brief função interna para normalização de um sinal
 *
 * @param signal
 */
void MFCC_normilize(MFCC_input *signal)
{
  double maxValue = 0, aux = 0;
  for (size_t i = 0; i < signal->sizeBuffer; i++)
  {
    aux = absDouble((signal->buffer[i]));
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
MFCC_frames *MFCC_frames_init(MFCC_input *signal)
{
  MFCC_array_pad(signal, "reflect");

  MFCC_frames *aux = (MFCC_frames *)malloc(sizeof(MFCC_frames));

  aux->len =
      (uint16_t)myRound((double)(signal->sampleRate * signal->hopSize) / 1000);
  aux->num =
      (uint16_t)myRound(((double)(signal->sizeBuffer - signal->fftSize) / aux->len) + 1);

  aux->frameMatrix = (double **)malloc(sizeof(double *) * aux->num);
  for (size_t i = 0; i < aux->num; i++)
    aux->frameMatrix[i] =
        myBufferCP(signal->buffer + (signal->fftSize * i), signal->fftSize);

  return aux;
}

/**
 * @brief Função para extração de MFCCs
 *
 * @param signal estrutura de input que pode ser inicializada com valores padrões
 * por meio da função "MFCC_initDefaultValues"
 * @return MFCC_coef
 */
MFCC_coef MFCC_execute(MFCC_input *signal)
{
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

  // TODO ultima função implementada
  // TODO testar os valores de saida
  MFCC_frames *frame = MFCC_frames_init(signal);
}

/**
 * @brief Retorna o valor positivo
 *
 * @param num
 * @return double
 */
double myDoubleABS(double num)
{
  return num < 0 ? (num * -1) : num;
}

/**
 * @brief Arredonda o número
 *
 * @param num
 * @return int
 */
int myRound(double num)
{
  return (num - ((int)num)) > .5 ? num + 1 : num;
}

/**
 * @brief Instancia um novo vetor
 *
 * @param buffer vetor que tera os valores copiados
 * @param sizeBuffer tamanho do vetor
 * @return double* novo vetor com os valores copiados
 */
double *myBufferCP(double *buffer, int sizeBuffer)
{
  double *aux = (double *)malloc(sizeof(double) * sizeBuffer);
  for (size_t i = 0; i < sizeBuffer; i++)
    aux[i] = buffer[i];

  return aux;
}

/**
 * @brief Exibe os valores das estrururas pertinentes dessa lib
 *
 * @param MFCC_struct ponteiro generico para a estrutura
 * @param MFCC_type nome do tipo da estrutura: "MFCC_coef", "MFCC_input" ou "MFCC_frames"
 */
void MFCC_structs_view(void *MFCC_struct, char *MFCC_type)
{
  if (strcmp(MFCC_type, "MFCC_coef") == 0)
  {
  }
}
