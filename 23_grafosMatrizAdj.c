#include <stdio.h>
#include <stdlib.h>

#include "grafosMatrizAdj.h"

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: A função graphInit() constrói um grafo com vértices 0 1 .. V-1 e nenhum arco. */
Graph graphInit(int n)
{
    int i, j;
    Graph G = malloc(sizeof *G);
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

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: A função graphInsertArc() insere um arco v-w no grafo G. A função supõe que v e w são distintos, positivos e menores que G->V. Se o grafo já tem um arco v-w, a função não faz nada. */
void graphInsertArc(Graph G, int v, int w)
{
    if (G->A[v][w] == 0)
    {
        G->A[v][w] = 1;
        G->m++;
    }
}

void graphInsertArcNotSafe(Graph G, int v, int w)
{
    G->A[v][w] = 1;
    G->m++;
}

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: A função graphRemoveArc() remove do grafo G o arco v-w. A função supõe que v e w são distintos, positivos e menores que G->V. Se não existe arco v-w, a função não faz nada. */
void graphRemoveArc(Graph G, int v, int w)
{
    if (G->A[v][w] == 1)
    {
        G->A[v][w] = 0;
        G->m--;
    }
}

/* REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIA: A função graphShow() imprime, para cada vértice v do grafo G, em uma linha, todos os vértices adjacentes a v. */
void graphShow(Graph G)
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

void graphPrint(Graph G)
{
    int i, j;
    for (i = 0; i < G->n; i++)
    {
        for (j = 0; j < G->n; j++)
            if (G->A[i][j] == 1)
                printf("%2d ", j);
        printf("-1"); // digito para marcar fim de lista
        printf("\n");
    }
}

Graph graphFree(Graph G)
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