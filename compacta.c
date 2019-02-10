#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TadArvore.h"
#include "TadLista.h"
#include "bitmap.h"

#define TAMANHOMAX 16777216

unsigned int codificaArvore (char c, Arvore* a, bitmap* bm, FILE* saida) {
	int quantBits = 0;
	if(!verifFolha(a)) {
		if(bitmapGetLength(*bm)<bitmapGetMaxSize(*bm)){
			if(verifPertence (retornaEsq(a), c)) {
				bitmapAppendLeastSignificantBit(bm, 0);
				quantBits++;
				quantBits += codificaArvore(c, retornaEsq(a), bm, saida);
			}else{
				bitmapAppendLeastSignificantBit(bm, 1);
				quantBits++;
				quantBits += codificaArvore(c, retornaDir(a), bm, saida);
			}
		}else{
			fwrite(bitmapGetContents(*bm), sizeof(char), (bitmapGetLength(*bm)+7)/8, saida);
			free(bitmapGetContents(*bm));
			*bm = bitmapInit(TAMANHOMAX);
			quantBits += codificaArvore(c, a, bm, saida);
		}
	}
	return quantBits;
}

void escreveCodificacao (FILE* entrada, FILE* saida, bitmap* bm, Sentinela* s) {
	unsigned char aux;
	int quantBits=0;
	fread(&aux, sizeof(unsigned char), 1, entrada);
	while(!feof(entrada)) {
		if(bitmapGetLength(*bm)<bitmapGetMaxSize(*bm)){
			quantBits += codificaArvore(aux, retornaArvore(retornaPrimNo(s)), bm, saida);
		}else{
			fwrite(bitmapGetContents(*bm), sizeof(char), (bitmapGetLength(*bm)+7)/8, saida);
			quantBits += bitmapGetLength(*bm);
			free(bitmapGetContents(*bm));
			*bm = bitmapInit(TAMANHOMAX);
			quantBits += codificaArvore(aux, retornaArvore(retornaPrimNo(s)), bm, saida);
		}
		fread(&aux, sizeof(char), 1, entrada);
	}
	if(bitmapGetLength(*bm)!=0) {
		fwrite(bitmapGetContents(*bm), sizeof(char), (bitmapGetLength(*bm)+7)/8, saida);
		free(bitmapGetContents(*bm));
		*bm = bitmapInit(TAMANHOMAX);
	}
	rewind(saida);
	fwrite(&quantBits, sizeof(int), 1, saida);
}

int main (int argc, char* argv[]) {
	if(argc==2) {
		unsigned int i, x, j;
		unsigned int vet[256];
		
		for(i=0; i<256; i++) {
			vet[i]=0;
		}

		FILE* entrada = fopen (argv[1], "rb");
		if(!entrada) {
			printf("ERRO AO ABRIR ARQUIVO!\n");
			return 0;
		}
		
		unsigned char aux;
		fscanf (entrada, "%c", &aux);
		while(!feof(entrada)) {
			vet[(int)aux]++;
			fscanf (entrada, "%c", &aux);
		}
		
		Sentinela* s = criaSentinela();
		for(i=0; i<256; i++) {
			if(vet[i]!=0) {
				Arvore* no = criaNoArv (i, vet[i], NULL, NULL);
				listaArv* a = criaNoLista(no);
				inserenaLista(s, a);
			}
		}
		
		while(retornaCont(s)>1) {
			Arvore* esq = retiraNoLista(s);
			Arvore* dir = retiraNoLista(s);
			Arvore* pai = criaNoArv (retornaFreq(esq)+retornaFreq(dir), retornaFreq(esq)+retornaFreq(dir), esq, dir);
			listaArv* noPai = criaNoLista(pai);
			inserenaLista(s, noPai);		
		}
		
				
		rewind(entrada);
		char* nomeArq;
		nomeArq = strsep(&argv[1], ".");
		unsigned int tamanho = strlen(argv[1]);
		char* extensao = calloc(tamanho+1, sizeof(char));
		strcpy(extensao, argv[1]);
		
		
		FILE* saida = fopen(strcat(nomeArq, ".comp"), "wb");
		if(!saida) {
			printf("ERRO AO ABRIR ARQUIVO!\n");
		}
			
		// escreve cabeçalho do arquivo compactado
		int quantBits = 0;
		fwrite(&quantBits, sizeof(int), 1, saida);
		for(i=0; i<256; i++) {
			fwrite(&vet[i], sizeof(unsigned int), 1, saida);
		}
		fwrite(&tamanho, sizeof(unsigned int), 1, saida);
		fwrite(extensao, sizeof(char), tamanho, saida);
		free(extensao);
				
		// codifica e escreve no arquivo
		bitmap bm = bitmapInit(TAMANHOMAX);
		escreveCodificacao(entrada, saida, &bm, s);
		
		// libera memória e fecha os arquivos
		s = liberaSentinela(s);
		free(bm.contents);
		fclose(entrada);
		fclose(saida);		
	}else{
		printf("ERRO!\n");
	}
	return 0;
}
