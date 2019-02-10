#include <stdio.h>
#include <stdlib.h>
#include "TadLista.h"

struct listaarv {
	Arvore* arv;
	struct listaarv* prox;
};

struct sentinela {
	int cont;
	struct listaarv* prim;
};

Sentinela* criaSentinela () {
	Sentinela* sent = (Sentinela*)malloc(sizeof(Sentinela));
	sent->cont = 0;
	sent->prim=NULL;
	return sent;
}

listaArv* criaNoLista (Arvore* a) {
	listaArv* novo = (listaArv*)malloc(sizeof(listaArv));
	novo->arv = a;
	novo->prox = NULL;
	return novo;
}

Arvore* retornaArvore (listaArv* l) {
	return l->arv;
}

listaArv* retornaPrimNo (Sentinela* s) {
	return s->prim;
}

int retornaCont (Sentinela* s) {
	return s->cont;
}

void inserenaLista (Sentinela* s, listaArv* a) {
	listaArv* ant = NULL;
	listaArv* p = s->prim;
	
	s->cont++;
	
	if (p == NULL) {
		s->prim = a;
		return;
	}
	
	while (p != NULL && retornaFreq(a->arv) >= retornaFreq(p->arv)) { 
		ant = p;
		p = p->prox;
	}
	
	if (ant == NULL) {
		a->prox = s->prim;
		s->prim = a;
	}else{
		ant->prox = a;
		a->prox = p;
	}
}

Arvore* retiraNoLista (Sentinela* s) {
	listaArv* aux = s->prim;
	Arvore* no = s->prim->arv;
	s->prim = s->prim->prox;
	s->cont--;
	free(aux);
	return no;
}

void imprime (Sentinela* s) {
	listaArv* aux;
	
	for(aux=s->prim; aux!=NULL; aux=aux->prox) {
		printf ("%d %d\n", retornaCod(aux->arv), retornaCod(aux->arv));
	}
}

listaArv* liberaNo(listaArv* l) {
	liberaArvore(l->arv);
	free(l);
	return NULL;
}

Sentinela* liberaSentinela (Sentinela* s) {
	s->prim = liberaNo(s->prim);
	free(s);
	return NULL;
}























