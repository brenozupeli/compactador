#ifndef TADLISTA_H_
#define TADLISTA_H_

#include "TadArvore.h"

typedef struct listaarv listaArv;

typedef struct sentinela Sentinela;

Sentinela* criaSentinela ();

listaArv* criaNoLista (Arvore* a);

Arvore* retornaArvore (listaArv* l);

listaArv* retornaPrimNo (Sentinela* s);

int retornaCont (Sentinela* s);

void inserenaLista (Sentinela* s, listaArv* a);

Arvore* retiraNoLista (Sentinela* s);

void imprime (Sentinela* s);

listaArv* liberaNo(listaArv* l);

Sentinela* liberaSentinela (Sentinela* s);

#endif
