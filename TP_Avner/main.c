#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char **argv){

Cache cache; // Declara a cache.

	Parametros parametros = recebeArgumentos();	// Recebe os argumentos do usuário.

	inicializaCache(&cache, &parametros); // Inicializa a cache de acordo com os parâmetros recebidos pelo usuário.

	Memoria memoria = newMemoria(TAM_MEMORIA); // Declara e aloca a memória.

	if(parametros.algoritmos == 1){
		bubbleSort(&memoria, &cache);
	}
	else if (parametros.algoritmos == 2){
		selectionSort(&memoria, &cache);
	}
	else if(parametros.algoritmos == 3){
		quickSort(&cache, &memoria, 0,memoria.qntDePalavras);
	}

	atualizaMemoria(&cache, &memoria); // Caso seja Write-Back, copia os blocos na cache que foram alterados para memória.

	imprimeEstatisticas(cache.estatisticas);

	return 0;
}
