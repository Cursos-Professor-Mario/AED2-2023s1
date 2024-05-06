#include <stdio.h>
#include <stdlib.h>

#include "fila.h"

// #define N 100

struct fila
{
    type *v;
    int ini;
    int fim;
    int tam;
};

Fila *criaFila(int tam)
{
    Fila *q;
    q = (Fila *)malloc(sizeof(Fila));
    q->v = (type *)malloc(tam * sizeof(type));
    q->ini = 0;
    q->fim = 0;
    q->tam = tam;
    return q;
}

void insereFila(Fila *q, type x)
{
    q->v[q->fim] = x;
    // (q->fim)++;
    // if (q->fim == q->tam)
    //     q->fim = 0;
    q->fim = (q->fim + 1) % q->tam;
}

type removeFila(Fila *q)
{
    type x;
    x = q->v[q->ini];
    // (q->ini)++;
    // if (q->ini == q->tam)
    //     q->ini = 0;
    q->ini = (q->ini + 1) % q->tam;
    return x;
}

int filaVazia(Fila *q)
{
    return q->fim == q->ini;
}

int filaCheia(Fila *q)
{
    // return (q->fim + 1 == q->ini || (q->fim + 1 == q->tam && q->ini == 0));
    return (q->fim + 1) % q->tam == q->ini;
}

void imprimeFila(Fila *q)
{
    int i;
    if (q->ini <= q->fim)
        for (i = q->ini; i < q->fim; i++)
            printf("%c ", q->v[i]);
    else // q->fim < q->ini
    {
        for (i = q->ini; i < q->tam; i++)
            printf("%c ", q->v[i]);
        for (i = 0; i < q->fim; i++)
            printf("%c ", q->v[i]);
    }
    printf("\n");
}

int tamFila(Fila *q)
{
    if (q->ini <= q->fim)
        return q->fim - q->ini;
    return (q->tam - q->ini) + (q->fim - 0);
}

Fila *liberaFila(Fila *q)
{
    free(q->v);
    free(q);
    return NULL;
}
