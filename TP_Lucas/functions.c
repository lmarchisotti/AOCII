#include "functions.h"

void atualizaMemoria(Cache *cache, Memoria *memoria){
	int i,j,k, indice, tag, endBloco;

	// Se a política de escrita for Write back, gravar na memória.
	if(cache->parametros.politicaEscrita == 2){
		for(i=0; i<cache->parametros.numDeConjuntos ;i++){	// Percorre conjuntos.
			for(j=0; j<cache->parametros.numDeVias ;j++){	// Percorre vias.

				if(cache->bloco[i][j].mudou){ // Se o bloco foi alterado, grava ele na memoria.
					tag = cache->bloco[i][j].tag;
					indice = i;
					endBloco = tag * cache->parametros.numDeConjuntos + indice ;

					for(k=0;k<cache->parametros.tamBloco;k++){ // Percorre palavras do bloco.
						memoria->dados[endBloco*cache->parametros.tamBloco + k] =
						cache->bloco[i][j].palavra[k];
					}
				}

			}
		}
	}
}

Parametros recebeArgumentos(){

Parametros A;

	// Limpa a tela.
	printf("\e[H\e[2J");

	printf("Algoritmo de ordenação: 1-BubbleSort, 2-SelectionSort, 3-QuickSort\n");
	printf("Tamanho da cache: 16, 32, 64, 128, 256 ou 512 palavras: \n");
	printf("Tamanho de bloco: 1, 2, 4, 8, 16 ou 32 palavras por bloco: \n");
	printf("Ex: --> 3 64 4\n");
	printf("--> ");
	scanf("%d %d %d", &(A.algoritmos), &(A.qntPalavras), &(A.tamBloco));

	A.qntDeBlocos = (int)(A.qntPalavras/A.tamBloco);
	A.numDeConjuntos = (int)(A.qntDeBlocos/A.numDeVias);

	printf("Mapeamento: 1-> Mapeamento Direto, 2-> Associativo por conjunto, 3-> Totalmente Associativo: \n");
	printf("--> ");
	scanf("%d", &(A.mapeamentoCache));

	if(A.mapeamentoCache == 1){
		A.numDeVias = 1;
	}
	else if(A.mapeamentoCache == 2){
		printf("Associatividade: conjunto de 2 vias, 4 vias, 8 vias: \n");
		printf("--> ");
		scanf("%d", &(A.numDeVias));
	}
	else if(A.mapeamentoCache == 3){
			A.numDeVias = A.qntDeBlocos;
	}

	printf("Política de Substituição: 1-LRU, 2-LFU, 3-FIFO: \n");
	printf("Política de Escrita: 1-Write Through, 2-Write-Back: \n");
	printf("--> ");
	scanf("%d %d", &(A.politicaSubstituicao), &(A.politicaEscrita));

	return A;
}

_Bool comparaPalavras(Palavra A, Palavra B){

int a = (int) A;
int b = (int) B;

	return a<b;
}

void limpaCache(Cache *cache){

int i, j;

	// Invalida todos os blocos da cache.
	for(i = 0; i < cache->parametros.numDeConjuntos; i++){
		for(j = 0; j < cache->parametros.numDeVias; j++){
			cache->bloco[i][j].V = FALSE;
		}
	}
}

void imprimeEstatisticas(Estatisticas estatisticas){

	printf("Hit leitura: %d\n", estatisticas.hitLeitura);
	printf("Miss leitura: %d\n",estatisticas.hitEscrita);
	printf("Hit escrita: %d\n",estatisticas.missLeitura);
	printf("Miss escrita: %d\n",estatisticas.missEscrita);
	
}

Bloco newBloco(int tamBloco){

Bloco B; //Cria um bloco.

	B.V = FALSE;	// Seta o bit de validade do bloco para FALSE.
	B.contador = 0;	// Seta o contador para 0.
	B.mudou = 0;	// Seta o bit mudou para 0.
	B.tag = -1;		// Seta a tag para -1 (para não representar nenhum bloco real).

	// Aloca memoria para as palavras do bloco.
	B.palavra = (Palavra *) malloc(tamBloco*sizeof(Palavra));

	return B; // Retorna o bloco criado.
}

void limpaEstatisticas(Cache *C){

	C->estatisticas.hitLeitura = 0;
	C->estatisticas.hitEscrita = 0;
	C->estatisticas.missLeitura = 0;
	C->estatisticas.missEscrita = 0;

}

void setaParametros(Cache *C, Parametros *parametros){

	C->parametros.mapeamentoCache = parametros->mapeamentoCache;
	C->parametros.politicaSubstituicao = parametros->politicaSubstituicao;
	C->parametros.politicaEscrita = parametros->politicaEscrita;
	C->parametros.qntDeBlocos = parametros->qntPalavras/parametros->tamBloco;

	switch(parametros->mapeamentoCache){
		case 1:	// Diretamente mapeada.
			C->parametros.numDeVias = 1;
			C->parametros.numDeConjuntos = parametros->qntDeBlocos;
			break;

		case 2:	// Associativa por conjuntos.
			C->parametros.numDeVias = parametros->numDeVias;
			C->parametros.numDeConjuntos = parametros->qntDeBlocos/parametros->numDeVias;
			break;

		case 3:	// Totalmente associativa.
			C->parametros.numDeVias = parametros->qntDeBlocos;
			C->parametros.numDeConjuntos = 1;
	}

}

void inicializaCache(Cache *C, Parametros *parametros){

int i, j;

	// Seta os parametros da cache.
	C->parametros.qntPalavras = parametros->qntPalavras;
	C->parametros.tamBloco = parametros->tamBloco;
	C->parametros.mapeamentoCache = parametros->mapeamentoCache;
	C->parametros.politicaSubstituicao = parametros->politicaSubstituicao;
	C->parametros.politicaEscrita = parametros->politicaEscrita;
	C->parametros.qntDeBlocos = parametros->qntDeBlocos;

	switch(C->parametros.mapeamentoCache){
		case 1:	// Diretamente mapeada
			C->parametros.numDeVias = 1;
			C->parametros.numDeConjuntos = parametros->qntDeBlocos;
			break;

		case 2:	// Associativa por conjuntos
			C->parametros.numDeVias = parametros->numDeVias;
			C->parametros.numDeConjuntos = parametros->qntDeBlocos/parametros->numDeVias;
			break;

		case 3:	// Totalmente associativa
			C->parametros.numDeVias = parametros->qntDeBlocos;
			C->parametros.numDeConjuntos = 1;
	}

	limpaEstatisticas(C); // Zera as estatisticas da cache.

	// Aloca memória para os blocos da cache e FIFO.
	C->bloco = (Bloco **) malloc((C->parametros.numDeConjuntos) * sizeof(Bloco *));

	C->FIFO = (int**) malloc((C->parametros.numDeConjuntos) * sizeof(int *));

	C->LRU = (int**) malloc((C->parametros.numDeConjuntos) * sizeof(int *));

	for(i=0;i<C->parametros.numDeConjuntos;i++){
		C->bloco[i] = (Bloco *) malloc((C->parametros.numDeVias) * sizeof(Bloco));
		C->FIFO[i] = (int *) malloc((C->parametros.numDeVias) * sizeof(int));
		C->LRU[i] = (int *) malloc((C->parametros.numDeVias) * sizeof(int));
	}

	// Inicializa todos os blocos da cache e FIFO nula.
	for(i=0;i<C->parametros.numDeConjuntos;i++){
		for(j=0;j<C->parametros.numDeVias; j++){
			C->bloco[i][j] = newBloco(C->parametros.tamBloco);
			C->FIFO[i][j] = -1;
			C->LRU[i][j] = -1;
		}
	}
}


Mapeamento mapeia(Cache *cache, int enderecoDaPalavra){

Mapeamento M; // Cálculos do mapeamento.

	M.enderecoDeBloco = enderecoDaPalavra/cache->parametros.tamBloco;
	M.offsetDeBLoco = enderecoDaPalavra%cache->parametros.tamBloco;
	M.tag = M.enderecoDeBloco/cache->parametros.numDeConjuntos;
	M.conjuntoDoBlocoNaCache = M.enderecoDeBloco%cache->parametros.numDeConjuntos;

	return M;

}





Memoria newMemoria(int tamMemoria){

int i;
Memoria memoria; // Declara uma memoria.

	memoria.qntDePalavras = tamMemoria/TAM_PALAVRA; // Guarda o tamanho da memória (qnt de palavras).

	// Aloca memória para um vetor de palavras (uma memória pode ser vista como um vetor de palavras).
	memoria.dados = (Palavra *) malloc(tamMemoria);

	// Preenche o vetor com valores aleatórios.
	for(i=0;i<memoria.qntDePalavras;i++){
		memoria.dados[i] = rand()%10000;
	}

	// Retorna a memória criada.
	return memoria;

}

void bubbleSort(Memoria *memoria, Cache *cache) {

int i, j, a, b;
Palavra temp;

	for (i = 0; i < memoria->qntDePalavras - 1; i++){
        for(j = (i+1); j < memoria->qntDePalavras; j++){
        	a = acessa(cache, memoria, j);
        	b = acessa(cache, memoria, i);

        	if ( a < b ){ //troca
            	temp = b;
            	grava(cache, memoria ,i, a);
            	grava(cache, memoria , j, temp);
            }
        }
    }

}

void selectionSort(Memoria *memoria, Cache *cache) {

int i, j, min, aux;
int a, b, numI, numMin;

	for (i = 0; i < memoria->qntDePalavras - 1; i++) {
    	min = i;
    	for (j = (i+1); j < memoria->qntDePalavras; j++) {
			a = acessa(cache, memoria, j);
			b = acessa(cache, memoria, min);
			if(a < b) min = j;
		}

		numI = acessa(cache, memoria, i);
		numMin = b;
		if (numI != numMin) {
			aux = numI;
			grava(cache, memoria ,i, numMin);
			grava(cache, memoria ,min, aux);
		}
	}

}


void quickSort(Cache *cache, Memoria *memoria, int left, int right){

int i, j, x, a, b ,temp, valor;

	i = left;
	j = right;
	x = memoria->dados[(left + right) / 2];

	while(i <= j){
		valor = acessa(cache, memoria, i);
		while(valor < x && i < right){
			i++;
			valor = acessa(cache, memoria, i);
		}
		valor = acessa(cache, memoria, j);
		while(valor > x && j > left){
			j--;
			valor = acessa(cache, memoria, j);
		}
		if(i <= j){
			a = acessa(cache, memoria, j);
			b = acessa(cache, memoria, i);
			temp = b;
			grava(cache, memoria ,i, a);
			grava(cache, memoria , j, temp);
			i++;
			j--;
		}
	}

	if(j > left){
		quickSort(cache, memoria, left, j);
	}
	if(i < right){
		quickSort(cache, memoria, i, right);
	}

}

Palavra acessa(Cache *cache, Memoria *memoria, int enderecoDaPalavra){

int i;
Mapeamento M = mapeia(cache, enderecoDaPalavra);

	for(i=0;i<cache->parametros.numDeVias;i++){
		if(cache->bloco[M.conjuntoDoBlocoNaCache][i].tag == M.tag && (cache->bloco[M.conjuntoDoBlocoNaCache][i].V)){
			// Atualiza estatisticas: hitLeitura++.
			cache->estatisticas.hitLeitura++;

			// Incrementa contador de uso.
			cache->bloco[M.conjuntoDoBlocoNaCache][i].contador++;

			//LRU


			// Retorna a palavra da cache.
			return cache->bloco[M.conjuntoDoBlocoNaCache][i].palavra[M.offsetDeBLoco];
		}
	}

	// Se chegar aqui, então não encontrou o bloco na cache.

	// Copia o bloco pra cache.
	i = copiaBloco(cache, memoria, M); // i = (o offset de conjunto).

	// Atualiza estatisticas: missleitura++.
	cache->estatisticas.missLeitura++;

	// Retorna a palavra da cache.
	return cache->bloco[M.conjuntoDoBlocoNaCache][i].palavra[M.offsetDeBLoco];

}

void grava(Cache *cache, Memoria *memoria , int enderecoDaPalavra, Palavra palavra){

int i;

 	// Contém o mapeamento pra cache do endereço da palavra na memória.
	Mapeamento mapeamento = mapeia(cache, enderecoDaPalavra);

	// Procura em todas as vias do conjunto.
	for(i=0;i<cache->parametros.numDeVias;i++){

		// Se encontrar o bloco na cache.
		if(cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].tag == mapeamento.tag && (cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].V)){

			// Atualiza estatisticas: hitEscrita++.
			cache->estatisticas.hitEscrita++;

			// Grava a palavra no bloco.
			cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].palavra[mapeamento.offsetDeBLoco] = palavra;

			// Seta o bit mudou.
			cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].mudou = TRUE;

			// Incrementa contador de uso.
			cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].contador++;

			//LRU
			/*
			for(j=0;j<cache->parametros.numDeVias;j++){
				if(cache->LRU[mapeamento.conjuntoDoBlocoNaCache][j] == i){
					for(k=j;k>0;k--){
						cache->LRU[mapeamento.conjuntoDoBlocoNaCache][k] = cache->LRU[mapeamento.conjuntoDoBlocoNaCache][k-1];
					}
					cache->LRU[mapeamento.conjuntoDoBlocoNaCache][0] = i;
					break;

				}
			}
			*/

			// Write Through.
			if(cache->parametros.politicaEscrita == 1){
				memoria->dados[enderecoDaPalavra] = palavra;
			}

			return;
		}
	}

	// Se chegar aqui, então não encontrou o bloco na cache.

	// Copia o bloco pra cache.
	i = copiaBloco(cache, memoria, mapeamento); // i = (o offset de conjunto)

	// Atualiza estatisticas: missEscrita++.
	cache->estatisticas.missEscrita++;

	// Grava a palavra no bloco.
	cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].palavra[mapeamento.offsetDeBLoco] = palavra;

	// Seta o bit mudou.
	cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].mudou = TRUE;

	// Write Through.
	if(cache->parametros.politicaEscrita == 1){
		memoria->dados[enderecoDaPalavra] = palavra;
	}

}

_Bool invalidaBlocoNoConjunto(Cache *cache, Memoria *memoria, Mapeamento mapeamento){

int j, min;

	// Invalidar de acordo com a política de substituição.
	switch(cache->parametros.politicaSubstituicao){
		case 1: //LRU
			//  Invalida o bloco que não foi usado a mais tempo.

			/*
			cache->bloco[mapeamento.conjuntoDoBlocoNaCache][cache->LRU[mapeamento.conjuntoDoBlocoNaCache][cache->parametros.numDeVias-1]].V = FALSE;

			// Se a política de escrita for Write-Back, gravar na memória.
			if(cache->parametros.politicaEscrita == 2){

				int tag = cache->bloco[mapeamento.conjuntoDoBlocoNaCache][cache->LRU[mapeamento.conjuntoDoBlocoNaCache][cache->parametros.numDeVias-1]].tag;
				int indice = mapeamento.conjuntoDoBlocoNaCache;
				int endBloco = tag * cache->parametros.numDeConjuntos + indice ;

				for(j=0;j<cache->parametros.tamBloco;j++){
					memoria->dados[endBloco*cache->parametros.tamBloco + j] =
					cache->bloco[mapeamento.conjuntoDoBlocoNaCache][cache->LRU[mapeamento.conjuntoDoBlocoNaCache][cache->parametros.numDeVias-1]].palavra[j];
				}
			}*/
			break;

		case 2: //LFU
			min = cache->bloco[mapeamento.conjuntoDoBlocoNaCache][0].contador;
			for(int i=1; i < cache->parametros.qntDeBlocos; i++){
				if(cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].contador < min){
					min = i;
				}
			}
			cache->bloco[mapeamento.conjuntoDoBlocoNaCache][min].V = FALSE;

			// Se a política de escrita for Write-Back, gravar na memória.
			if(cache->parametros.politicaEscrita == 2){

				int tag = cache->bloco[mapeamento.conjuntoDoBlocoNaCache][min].tag;
				int indice = mapeamento.conjuntoDoBlocoNaCache;
				int endBloco = tag * cache->parametros.numDeConjuntos + indice ;

				for(j=0;j<cache->parametros.tamBloco;j++){
					memoria->dados[endBloco*cache->parametros.tamBloco + j] =
					cache->bloco[mapeamento.conjuntoDoBlocoNaCache][min].palavra[j];
				}
			}
			break;

		case 3: //FIFO
			// Invalida o bloco a mais tempo na cache.
			cache->bloco[mapeamento.conjuntoDoBlocoNaCache][cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][0]].V = FALSE;

			// Se a política de escrita for Write-Back, gravar na memória.
			if(cache->parametros.politicaEscrita == 2){

				int tag = cache->bloco[mapeamento.conjuntoDoBlocoNaCache][cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][0]].tag;
				int indice = mapeamento.conjuntoDoBlocoNaCache;
				int endBloco = tag * cache->parametros.numDeConjuntos + indice ;

				for(j=0;j<cache->parametros.tamBloco;j++){
					memoria->dados[endBloco*cache->parametros.tamBloco + j] =
					cache->bloco[mapeamento.conjuntoDoBlocoNaCache][cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][0]].palavra[j];
				}
			}
			break;
	}

	return TRUE; // Um bloco foi invalidado na cache (bit de validade = FALSE).
}

int copiaBloco(Cache *cache, Memoria *memoria, Mapeamento mapeamento){
	int i, j;
	while (1){
		// Procura um bloco invalido na cache para sobreescrever.
		for(i=0;i<cache->parametros.numDeVias;i++){

			if(!cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].V){

				// Se encontrou um bloco inválido, copia o bloco da memória pro endereço do bloco invalido na cache.
				for(j=0;j<cache->parametros.tamBloco;j++){
					cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].palavra[j] = memoria->dados[mapeamento.enderecoDeBloco*cache->parametros.tamBloco + j];
				}

				// Copia a tag.
				cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].tag = mapeamento.tag;

				// Seta o bit de validade do bloco.
				cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].V = TRUE;

				// Seta o bit mudou do bloco.
				cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].mudou = FALSE;

				// Seta o contador do bloco.
				cache->bloco[mapeamento.conjuntoDoBlocoNaCache][i].contador = 0;

				if(cache->parametros.politicaSubstituicao == 3 || cache->parametros.politicaSubstituicao == 1){
					// ATUALIZA FIFO
					if(cache->parametros.mapeamentoCache == 1){//se for diretamente mapeada
						cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][0] = 0;
					}
					else{
						for(j=0; j < cache->parametros.numDeVias-1; j++){ //desloca todos os outros blocos pra frente
							cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][j] = cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][j+1] ;
						}
						cache->FIFO[mapeamento.conjuntoDoBlocoNaCache][cache->parametros.numDeVias-1] = i;	//coloca o bloco que vai entrar na ultima posição da fila
					}
				}

				return i;
			}
		}

		// Se a cache estiver cheia, invalidar um bloco de acordo com a política de substituição para sobreescrever.
		invalidaBlocoNoConjunto(cache, memoria, mapeamento);
	}
}
