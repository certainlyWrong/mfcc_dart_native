#include <stdio.h>
#include <stdlib.h>
#include "../header/mfcc.h"

void loadBuffer(double **, size_t, char *);
// 110250

int main(int argc, char const *argv[])
{
	double *buffer;
	size_t bufferSize = 110250;
	char *path = "/home/adriano/Documents/SaveTheWorld/mfcc_dart_native/mfcc/buffer1_esp";

	// Carregando buffer de teste
	loadBuffer(&buffer, bufferSize, path);

	// Iniciando a estrutura do sinal com os valores padrões usados no projeto
	MFCC_input *signal = MFCC_initDefaultValues(buffer, bufferSize);

	// Iniciando o processamento dos MFCCs
	MFCC_coef result = MFCC_execute(signal);

	return 0;
}

void loadBuffer(double **buffer, size_t sizeBuffer, char *path)
{
	FILE *aux = fopen(path, "r");
	*buffer = (double *)malloc(sizeof(double) * sizeBuffer);

	for (size_t i = 0; i < sizeBuffer; i++)
		fscanf(aux, "%lf", &(*buffer)[i]);
	fclose(aux);
}