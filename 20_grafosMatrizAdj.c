#include <stdio.h>
#include <stdlib.h>

#include "grafosMatrizAdj.h"

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: 
A função inicializaGrafo() constrói um grafo 
com vértices 0 1 .. n-1 e nenhum arco. */
Grafo inicializaGrafo(int n)
{
    int i, j;
    Grafo G = malloc(sizeof *G);
    G->n = n;
    G->m = 0;
    G->A = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        G->A[i] = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            G->A[i][j] = 0;
    return G;
}

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: 
A função insereArcoGrafo() insere um arco v-w 
no grafo G. A função supõe que v e w são distintos, 
positivos e menores que G->n. Se o grafo já tem um 
arco v-w, a função não faz nada. */
void insereArcoGrafo(Grafo G, int v, int w)
{
    if (G->A[v][w] == 0)
    {
        G->A[v][w] = 1;
        G->m++;
    }
}

/* Versão da função insereArcoGrafo() que não testa 
se o arco v-w já está presente */
void insereArcoNaoSeguraGrafo(Grafo G, int v, int w)
{
    G->A[v][w] = 1;
    G->m++;
}

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: 
A função removeArcoGrafo() remove do grafo G o 
arco v-w. A função supõe que v e w são distintos, 
positivos e menores que G->n. Se não existe arco v-w, 
a função não faz nada. */
void removeArcoGrafo(Grafo G, int v, int w)
{
    if (G->A[v][w] == 1)
    {
        G->A[v][w] = 0;
        G->m--;
    }
}

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: 
A função mostraGrafo() imprime, para cada vértice v 
do grafo G, em uma linha, todos os vértices adjacentes a v. */
void mostraGrafo(Grafo G)
{
    int i, j;
    for (i = 0; i < G->n; i++)
    {
        printf("%2d:", i);
        for (j = 0; j < G->n; j++)
            if (G->A[i][j] == 1)
                printf(" %2d", j);
        printf("\n");
    }
}

/* Versão da função mostraGrafo() com impressão mais limpa 
para facilitar geração de instâncias e a leitura destas */
void imprimeGrafo(Grafo G)
{
    int i, j;
    for (i = 0; i < G->n; i++)
    {
        for (j = 0; j < G->n; j++)
            if (G->A[i][j] == 1)
                printf("%2d ", j);
        printf("-1"); // sentinela para marcar fim de lista
        printf("\n");
    }
}

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: 
A função liberaGrafo() libera toda a memória 
alocada para o grafo G e devolve NULL. */
Grafo liberaGrafo(Grafo G)
{
    int i;
    for (i = 0; i < G->n; i++)
    {
        free(G->A[i]);
        G->A[i] = NULL;
    }
    free(G->A);
    G->A = NULL;
    free(G);
    return NULL;
}