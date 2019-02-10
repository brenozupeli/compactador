#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TadArvore.h"

struct arvore {
	int cod;
	int freq;
	struct arvore* esq;
	struct arvore* dir;
};

Arvore* criaNoArv (int codigo, int frequencia, Arvore* esq, Arvore* dir) {
	Arvore* novo = (Arvore*)malloc(sizeof(Arvore));
	novo->cod = codigo;
	novo->freq = frequencia;
	novo->esq = esq;
	novo->dir = dir;
	return novo;
}

Arvore* retornaEsq(Arvore* a) {
	return a->esq;
}

Arvore* retornaDir(Arvore* a) {
	return a->dir;
}

int retornaFreq (Arvore* a) {
	return a->freq;
}

int retornaCod (Arvore* a) {
	return a->cod;
}

int verifFolha(Arvore* a) {
	if(a->esq==NULL && a->dir==NULL) {
		return 1;
	}else{
		return 0;
	}
}

int arvVazia (Arvore* a) {
	if(a==NULL) {
		return 1;
	}else{
		return 0;
	}
}

int altura (Arvore* a) {
	if(a==NULL) {
		return -1;
	}else{
		int he = altura(a->esq);
		int hd = altura(a->dir);
		if(he<hd) {
			return hd+1;
		}else{
			return he+1;
		}
	}
}

int verifPertence (Arvore* a, char c) {
	if (!arvVazia(a)) {
		return ((char)a->cod==c && verifFolha(a)) || verifPertence(a->esq, c) || verifPertence(a->dir, c);
	}else{
		return 0;
	}
}

Arvore* liberaArvore (Arvore* a) {
	if (!arvVazia(a)){
		liberaArvore(a->esq);
		liberaArvore(a->dir);
		free(a);
	}
	return NULL;
}
