#include <stdio.h>
#include <stdlib.h>

#include "grafosListasAdj.h"

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: 
A função inicializaGrafo() constrói um grafo 
com vértices 0 1 .. n-1 e nenhum arco. */
Grafo inicializaGrafo(int n)
{
    int i;
    Grafo G = malloc(sizeof *G);
    G->n = n;
    G->m = 0;
    G->A = malloc(n * sizeof(Noh *));
    for (i = 0; i < n; i++)
        G->A[i] = NULL;
    return G;
}

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: 
A função insereArcoGrafo() insere um arco v-w 
no grafo G. A função supõe que v e w são distintos, 
positivos e menores que G->n. Se o grafo já tem um 
arco v-w, a função não faz nada. */
void insereArcoGrafo(Grafo G, int v, int w)
{
    Noh *p;
    for (p = G->A[v]; p != NULL; p = p->prox)
        if (p->rotulo == w)
            return;
    p = malloc(sizeof(Noh));
    p->rotulo = w;
    p->prox = G->A[v];
    G->A[v] = p;
    G->m++;
}

/* Versão da função insereArcoGrafo() que não testa 
se o arco v-w já está presente */
void insereArcoNaoSeguraGrafo(Grafo G, int v, int w)
{
    Noh *p;
    p = malloc(sizeof(Noh));
    p->rotulo = w;
    p->prox = G->A[v];
    G->A[v] = p;
    G->m++;
}

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: 
A função mostraGrafo() imprime, para cada vértice v 
do grafo G, em uma linha, todos os vértices adjacentes a v. */
void mostraGrafo(Grafo G)
{
    int i;
    Noh *p;
    for (i = 0; i < G->n; i++)
    {
        printf("%2d:", i);
        for (p = G->A[i]; p != NULL; p = p->prox)
            printf(" %2d", p->rotulo);
        printf("\n");
    }
}

void imprimeGrafo(Grafo G)
{
    imprimeArquivoGrafo(G, stdout);
}

/* Versão da função mostraGrafo() com impressão mais limpa 
para facilitar geração de instâncias e a leitura destas */
void imprimeArquivoGrafo(Grafo G, FILE *saida)
{
    int i;
    Noh *p;
    fprintf(saida, "%d %d\n", G->n, G->m);
    for (i = 0; i < G->n; i++)
    {
        for (p = G->A[i]; p != NULL; p = p->prox)
            fprintf(saida, "%2d ", p->rotulo);
        fprintf(saida, "-1"); // sentinela para marcar fim de lista
        fprintf(saida, "\n");
    }
}

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: 
A função liberaGrafo() libera toda a memória 
alocada para o grafo G e devolve NULL. */
Grafo liberaGrafo(Grafo G)
{
    int i;
    Noh *p;
    for (i = 0; i < G->n; i++)
    {
        p = G->A[i];
        while (p != NULL)
        {
            G->A[i] = p;
            p = p->prox;
            free(G->A[i]);
        }
        G->A[i] = NULL;
    }
    free(G->A);
    G->A = NULL;
    free(G);
    return NULL;
}
