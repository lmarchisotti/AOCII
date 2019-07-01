#define FALSE 0
#define TRUE 1
#define TAM_PALAVRA 4
#define TAM_MEMORIA (16 * 1024) // 16KiB.

typedef int Palavra;

typedef struct T_Memoria{
	int qntDePalavras; // Guarda o tamanho da memória (quantidade de palavras).
	Palavra *dados;	// Vetor para armazear as palavras.
}Memoria;

typedef struct T_Mapeamento{
	int enderecoDeBloco;
	int offsetDeBLoco;
	int conjuntoDoBlocoNaCache;
	int tag;
}Mapeamento;

// Parametros passados pelo usuário.
typedef struct T_Parametros{
	int algoritmos;
	int qntPalavras; 			// Tamanho da cache.
	int tamBloco;
	int qntDeBlocos;
	int numDeVias;
	int numDeConjuntos;
	int mapeamentoCache; 		// 1-Direto || 2-Associativo por conjunto || 3-Totalmente associativo.
	int politicaSubstituicao;	// 1-LRU || 2-LFU || 3-FIFO.
	int politicaEscrita; 		// 1-Write through || 2-Write back.
}Parametros;

typedef struct T_Estatisticas{
	int hitLeitura;
	int hitEscrita;
	int missLeitura;
	int missEscrita;
}Estatisticas;

typedef struct T_Bloco{
	_Bool V; // Bit de validade.
	_Bool mudou; // Bit mudou.
	unsigned long int contador; // Para política de substituição.
	int tag;
	Palavra *palavra; // Vetor de palavras para armazenar as palavras do bloco.
}Bloco;

typedef struct T_Cache{
	Parametros parametros; // Armazena os parametros da cache passados pelo usuário.
	Estatisticas estatisticas; // Armazena as estatísticas de cache hit/miss.
	Bloco **bloco; // A cache é uma matriz de blocos, onde cada linha é um conjunto.
	int **FIFO;
	int **LRU;
}Cache;

void inicializaCache(Cache *cache, int qntPalavras, int tamBloco, int mapeamento, int numDeVias, int politicaSub, int politicaEsc);
void limpaCache(Cache *cache);
Mapeamento mapeia(Cache *cache, int enderecoDaPalavra);

void setaParametros(Cache *C, Parametros *parametros);

Bloco newBloco(int tamBloco);
_Bool invalidaBlocoNoConjunto(Cache *cache, Memoria *memoria, Mapeamento mapeamento);
int copiaBloco(Cache *cache, Memoria *memoria, Mapeamento mapeamento);

void grava(Cache *cache, Memoria *memoria , int enderecoDaPalavra, Palavra palavra);
Palavra acessa(Cache *cache, Memoria *memoria, int enderecoDaPalavra);

void limpaEstatisticas(Cache *C);
void imprimeEstatisticas(Estatisticas estatisticas);

// =============================================================================
Memoria newMemoria(int tamMemoria);
void atualizaMemoria(Cache *cache, Memoria *memoria);

// =============================================================================
void bubbleSort(Memoria *memoria, Cache *cache);
void selectionSort(Memoria *memoria, Cache *cache);
void quickSort(Cache *cache, Memoria *memoria, int left, int right);
