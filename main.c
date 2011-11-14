#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGHT 100
#define MAX_STRING 3 * MAX_LENGHT
#define MAX_LINE 100

//Open arquivo para leitura
void fileOpenRead(FILE **pArquivo, char *name) {
	if (!(*pArquivo = fopen(name, "r"))) {
		printf("Erro ao abrir o arquivo %s \n", name);
	}
}

//Open arquivo para escrita
void fileOpenWrite(FILE **pArquivo, char *name) {
	if (!(*pArquivo = fopen(name, "a+"))) {
		printf("Erro ao abrir o arquivo %s \n", name);
	}
}

//Write no arquivo
void writeFile(FILE **pArquivo, char *dados) {
	fprintf(*pArquivo, "%s", dados);
}

//Read no arquivo
int readFile(FILE **pArquivo, char stringOutput[MAX_LINE][MAX_STRING]) {
	int contador = 0;
	while (!feof(*pArquivo)) {
		fscanf(*pArquivo, "%s", stringOutput[contador]);
		contador++;
	}
	return contador;
}

//Close no Arquivo
void closeFile(FILE **pArquivo) {
	fclose(*pArquivo);
}

//Mount arquivo CSV
void mountInput(char *stringInput, fileCSV registro) {
	int len;
	strcat(stringInput, registro.fNome);
	len = strlen(stringInput);
	stringInput[len - 1] = ',';
	//stringInput[len] = '{FONTE}';
	strcat(stringInput, registro.mNome);
	len = strlen(stringInput);
	stringInput[len - 1] = ',';
	//stringInput[len] = '{FONTE}';
	strcat(stringInput, registro.lNome);
}

int main(int argc, char **argv) {
	fileCSV registro;
	int contador = 0, auxiliar;
	char stringInput[MAX_STRING];
	char stringOutput[MAX_LINE][MAX_STRING];
	FILE *pArquivo;

	readInputs(&registro);
	mountInput(stringInput, registro);
	fileOpenWrite(&pArquivo);
	writeFile(&pArquivo, stringInput);
	closeFile(&pArquivo);
	fileOpenRead(&pArquivo);
	auxiliar = readFile(&pArquivo, stringOutput);
	for (; contador <= auxiliar; contador++) {
		printf("Registros : %s\n", stringOutput[contador]);
	}
	closeFile(&pArquivo);
	return 0;
}
