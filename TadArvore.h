#ifndef _TADARVORE_H_
#define _TADARVORE_H_

#include "bitmap.h"

typedef struct arvore Arvore;

Arvore* criaNoArv (int codigo, int frequencia, Arvore* esq, Arvore* dir);

Arvore* retornaEsq(Arvore* a);

Arvore* retornaDir(Arvore* a);

int retornaFreq (Arvore* a);

int retornaCod (Arvore* a);

int arvVazia (Arvore* a);

void imprimeArvore (Arvore* a);

int altura (Arvore* a);

int verifPertence (Arvore* a, char c);

Arvore* liberaArvore (Arvore* a);

#endif
