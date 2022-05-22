
#define _MFCC_private
#include "../header/mfcc.h"

MFCC_input *MFCC_initDefaultValues(double *buffer, size_t sizeBuffer)
{
  MFCC_input *aux = (MFCC_input *)malloc(sizeof(MFCC_coef));
  aux->hopSize = 15;
  aux->fftSize = 2048;
  aux->buffer = buffer;
  aux->dctFilterNum = 20;
  aux->melFilterNum = 10;
  aux->sizeBuffer = sizeBuffer;
  aux->normilizeActivate = TRUE;

  return aux;
}

void MFCC_input_free(MFCC_input *signal)
{
  free(signal->buffer);
  free(signal);
}

double *MFCC_cp_buffer(double *buffer, int sizeBuffer)
{
  double *aux = (double *)malloc(sizeof(double) * sizeBuffer);
  for (size_t i = 0; i < sizeBuffer; i++)
    aux[i] = buffer[i];

  return aux;
}

void MFCC_normilize(MFCC_input *signal)
{
  int maxValue = 0;
  for (size_t i = 0; i < signal->sizeBuffer; i++)
  {
    if (abs((int)signal->buffer[i]) > maxValue)
      maxValue = signal->buffer[i];
  }

  for (size_t i = 0; i < signal->sizeBuffer; i++)
    signal->buffer[i] /= maxValue;
}

MFCC_coef MFCC_execute(MFCC_input *signal)
{
  // TODO implementar
}
