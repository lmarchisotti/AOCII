#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define TAM_MEMORIA (16 * 1024) // 16KiB.
#define TAM_PALAVRA 4

typedef int Palavra;

typedef struct{
	int qntDePalavras; // Guarda o tamanho da memória (quantidade de palavras).
	Palavra *dados;	// Vetor para armazear as palavras.
} Memoria;

typedef struct Mapeamento{
	int enderecoDeBloco;
	int offsetDeBLoco;
	int conjuntoDoBlocoNaCache;
	int tag;
}Mapeamento;

// Parametros passados pelo usuário.
typedef struct{
	int algoritmos;
	int qntPalavras; 			// Tamanho da cache.
	int tamBloco;
	int qntDeBlocos;
	int numDeVias;
	int numDeConjuntos;
	int mapeamentoCache; 		// 1-Direto || 2-Associativo por conjunto || 3-Totalmente associativo.
	int politicaSubstituicao;	// 1-LRU || 2-LFU || 3-FIFO.
	int politicaEscrita; 		// 1-Write through || 2-Write back.
} Parametros;

typedef struct {
	int hitLeitura;
	int hitEscrita;
	int missLeitura;
	int missEscrita;
} Estatisticas;


typedef struct{
	_Bool V; // Bit de validade.
	_Bool mudou; // Bit mudou.
	int contador; // Para política de substituição.
	int tag;
	Palavra *palavra; // Vetor de palavras para armazenar as palavras do bloco.
}Bloco;

typedef struct{
	Parametros parametros; // Armazena os parametros da cache passados pelo usuário.
	Estatisticas estatisticas; // Armazena as estatísticas de cache hit/miss.
	Bloco **bloco; // A cache é uma matriz de blocos, onde cada linha é um conjunto.
	int **FIFO;
	int **LRU;
}Cache;

Bloco newBloco(int tamBloco);
void limpaEstatisticas(Cache *C);
void setaParametros(Cache *C, Parametros *parametros);
void inicializaCache(Cache *cache, Parametros *parametros);
Mapeamento mapeia(Cache *cache, int enderecoDaPalavra);
void grava(Cache *cache, Memoria *memoria , int enderecoDaPalavra, Palavra palavra);
_Bool invalidaBlocoNoConjunto(Cache *cache, Memoria *memoria, Mapeamento mapeamento);
Palavra acessa(Cache *cache, Memoria *memoria, int enderecoDaPalavra);
int copiaBloco(Cache *cache, Memoria *memoria, Mapeamento mapeamento);
Memoria newMemoria(int tamMemoria);
Parametros recebeArgumentos();
void bubbleSort(Memoria *memoria, Cache *cache) ;
void limpaCache(Cache *cache);
void imprimeEstatisticas(Estatisticas estatisticas);
_Bool comparaPalavras(Palavra A, Palavra B);
void selectionSort(Memoria *memoria, Cache *cache);
void quickSort(Cache *cache, Memoria *memoria, int left, int right);
void atualizaMemoria(Cache *cache, Memoria *memoria);
