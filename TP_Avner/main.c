#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

int main(int argc, char **argv){

	Cache cache; // Declara a cache.

	int algoritmos, tamBloco, numDeVias;
	int qntPalavras; // Tamanho da cache.
	int mapeamento;  // 1-Direto || 2-Associativo por conjunto || 3-Totalmente associativo.
	int politicaSub; // 1-LRU || 2-LFU || 3-FIFO.
	int politicaEsc; // 1-Write through || 2-Write back.

	Memoria memoria = newMemoria(TAM_MEMORIA); // Declara e aloca a memória.

	printf("De acordo com as opções, escolha a configuração da cache.\n\n");
	printf("Algoritmo de ordenação: 1-BubbleSort, 2-SelectionSort, 3-QuickSort\n");
	printf("Tamanho da cache: 16, 32, 64, 128, 256 ou 512 palavras: \n");
	printf("Tamanho de bloco: 1, 2, 4, 8, 16 ou 32 palavras por bloco: \n");
	printf("Mapeamento: 1-Mapeamento Direto, 2-Associativo por conjunto, 3-Totalmente Associativo: \n");
	printf("Política de Substituição: 1-LRU, 2-LFU, 3-FIFO: \n");
	printf("Política de Escrita: 1-Write Through, 2-Write-Back: \n");
	printf("Ex: 3 64 4 1 3 1\n");
	printf(":: ");
	scanf("%d %d %d %d %d %d", &algoritmos, &qntPalavras, &tamBloco, &mapeamento, &politicaSub, &politicaEsc);

	if(mapeamento == 1){
		numDeVias = 1;
	} else {
		if(mapeamento == 2){
			printf("Associatividade: conjunto de 2 vias, 4 vias, 8 vias: \n");
			printf(":: ");
			scanf("%d", &numDeVias);
		} else {
			if(mapeamento == 3){
				numDeVias = (int)qntPalavras/tamBloco;
			}
		}
	}

	// Inicializa a cache de acordo com os parâmetros recebidos pelo usuário.
	inicializaCache(&cache, qntPalavras, tamBloco, mapeamento, numDeVias, politicaSub, politicaEsc);

	if(algoritmos == 1){
		bubbleSort(&memoria, &cache);
	} else {
		if(algoritmos == 2){
			selectionSort(&memoria, &cache);
		} else {
			if(algoritmos == 3){
				quickSort(&cache, &memoria, 0, memoria.qntDePalavras);
			}
		}
	}

	if(politicaEsc == 2){
		atualizaMemoria(&cache, &memoria); // Caso seja Write-Back, copia os blocos na cache que foram alterados para memória.
	}

	printf("\n");
	imprimeEstatisticas(cache.estatisticas);

	return 0;
}
