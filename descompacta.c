#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TadArvore.h"
#include "TadLista.h"
#include "bitmap.h"

#define TAMANHOMAX 16777216

void codificaCaracter(unsigned int dec, bitmap* bm) {
	register int i;
	for(i=0; i<8; i++) {
		unsigned char c = ((dec & (128 >> i)) ? 1 : 0);
		bitmapAppendLeastSignificantBit(bm, c);
	}
}

int binario_inteiro(bitmap* bm) {
	int inteiro = 0;
	register int i, j;
	for(i=0, j=7; j>=0; i++, j--) {
		if(bitmapGetBit(*bm, i)==1) {
			inteiro += pow(2, j);
		}
	}
	return inteiro;
}

Arvore* desfazSerializacao(bitmap* bm, unsigned int* posicao) {
	Arvore* a;
	if(*posicao+1<=bitmapGetLength(*bm)) {
		if(bitmapGetBit(*bm, *posicao)==1) {
			*posicao+=1;
			a = criaNoArv(-1, 0, desfazSerializacao(bm, posicao), desfazSerializacao(bm, posicao));
		}else{
			bitmap bm2 = bitmapInit(8);
			int i;
			for(i=0; i<8; i++) {
				*posicao+=1;
				bitmapAppendLeastSignificantBit(&bm2, bitmapGetBit(*bm, *posicao));
			}
			*posicao+=1;
			a = criaNoArv(binario_inteiro(&bm2), 0, NULL, NULL);
			free(bitmapGetContents(bm2));
		}
	}
	return a;
}

Arvore* decodifica(bitmap* bm, Arvore* a, Arvore* aux, FILE* saida) {
	register int i;
	for(i=0; i<bitmapGetLength(*bm); i++) {
		if(bitmapGetBit(*bm, i)==1) {
			if(aux!=NULL) {
				aux = retornaDir(aux);
			}else{
				aux = retornaDir(a);
			}
		}else{
			if(aux!=NULL) {
				aux = retornaEsq(aux);
			}else{
				aux = retornaEsq(a);
			}
		}
		if(verifFolha(aux)) {
			int codigo;
			codigo = retornaCod(aux);
			unsigned char c = (unsigned char)codigo;
			fwrite(&c, sizeof(unsigned char), 1, saida);
			aux=NULL;
		}
	}
	return aux;
}

void carregaBitmap(bitmap* bm, FILE* entrada, FILE* saida, Arvore* a, int quantBits) {
	unsigned char auxc;
	int i = 0;
	int j;
	Arvore* aux = NULL; //ponteiro para o nó da arvore onde a leitura parou
	fread(&auxc, sizeof(unsigned char), 1, entrada);
	while(!feof(entrada)) {
		if(bitmapGetLength(*bm)+8<bitmapGetMaxSize(*bm)) {
			bitmap bm2 = bitmapInit(8);
			codificaCaracter(auxc, &bm2);
			for(j=0; j<8; j++) {
				bitmapAppendLeastSignificantBit(bm, bitmapGetBit(bm2, j));
				i++;
				if(i>=quantBits) {
					break;
				}
			}
			free(bitmapGetContents(bm2));
			fread(&auxc, sizeof(unsigned char), 1, entrada);
		}else{
			aux = decodifica(bm, a, aux, saida);
			free(bitmapGetContents(*bm));
			*bm = bitmapInit(TAMANHOMAX);
		}
		if(i>=quantBits) {
			break;
		}
	}
	if(bitmapGetLength(*bm)!=0) {
		aux = decodifica(bm, a, aux, saida);
		free(bitmapGetContents(*bm));
		*bm = bitmapInit(TAMANHOMAX);
	}	
}

int main (int argc, char* argv[]) {
	if(argc==2) {
		FILE* entrada = fopen(argv[1], "rb");
		if(!entrada) {
			printf("ERRO AO ABRIR ARQUIVO DE ENTRADA!\n");
			return 0;
		}

		register int i;
		int tamArvore, tamExten;
		unsigned int vet[256];
		int quantBits;

		fread(&quantBits, sizeof(int), 1, entrada);
		for(i=0; i<256; i++) {
			fread(&vet[i], sizeof(unsigned int), 1, entrada);
		}	
		fread(&tamExten, sizeof(unsigned int), 1, entrada);
		unsigned char* extensao = calloc(tamExten+1, sizeof(unsigned char));
		fread(extensao, sizeof(unsigned char), tamExten, entrada);
		
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
		
		char* nomeArq;
		nomeArq = strsep(&argv[1], ".");
		unsigned char* arquivo = calloc((strlen(nomeArq)+tamExten+2), sizeof(unsigned char));
		strcpy(arquivo, strcat(strcat(nomeArq, "."), extensao));
		FILE* saida = fopen(arquivo, "w");
		if(!saida) {
			printf("ERRO AO ABRIR ARQUIVO DE SAIDA!\n");
			return 0;
		}

		bitmap bm = bitmapInit(TAMANHOMAX);

		carregaBitmap(&bm, entrada, saida, retornaArvore(retornaPrimNo(s)), quantBits);


		s = liberaSentinela(s);
		free(bm.contents);
		fclose(entrada);
		fclose(saida);
		free(extensao);
		free(arquivo);  // ERRO
	}else{
		printf("ERRO!\n");
	}
	return 0;
}
