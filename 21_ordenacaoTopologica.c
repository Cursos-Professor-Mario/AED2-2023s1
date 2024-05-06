#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafosListasAdj.h"

void buscaProfOrdTopoR(Grafo G, int v, int *visitado, int *ordTopo, int *protulo_atual)
{
    int w;
    Noh *p;
    visitado[v] = 1;
    /* para cada vizinho de v que ainda não foi visitado */
    p = G->A[v];
    while (p != NULL)
    {
        w = p->rotulo;
        if (visitado[w] == 0)
            buscaProfOrdTopoR(G, w, visitado, ordTopo, protulo_atual);
        p = p->prox;
    }
    ordTopo[v] = (*protulo_atual)--;
}

void buscaProfOrdTopoI(Grafo G, int origem, int *visitado, int *ordTopo, int *protulo_atual)
{
    int v, w;
    Noh *p;
    // pilha implementada em vetor
    int *pilha;
    int topo = 0;
    pilha = malloc(G->m * sizeof(int));
    /* colocando vértice origem na pilha */
    pilha[topo++] = origem;
    /* enquanto a pilha dos ativos (encontrados
    mas não visitados) não estiver vazia */
    while (topo > 0)
    {
        /* remova o mais recente da pilha */
        v = pilha[--topo];
        if (visitado[v] == 0) // se v nao foi visitado
        {
            visitado[v] = 1;
            pilha[topo++] = v; // empilha o vértice pra saber quando marcar o tempo de término
            /* para cada vizinho deste que ainda não foi visitado */
            p = G->A[v];
            while (p != NULL)
            {
                w = p->rotulo;
                if (visitado[w] == 0)
                    pilha[topo++] = w; // empilha o vizinho
                p = p->prox;
            }
        }
        else if (ordTopo[v] == -1) // se v ja foi visitado e sua ordem topologica ainda nao foi atribuida
            ordTopo[v] = (*protulo_atual)--;
    }
}

void ordenacaoTopologica(Grafo G, int *ordTopo)
{
    int v, rotulo_atual, *visitado;
    visitado = malloc(G->n * sizeof(int));
    /* inicializa todos como não visitados e sem ordem topológica */
    for (v = 0; v < G->n; v++)
    {
        visitado[v] = 0;
        ordTopo[v] = -1;
    }
    rotulo_atual = G->n;
    for (v = 0; v < G->n; v++)
        if (visitado[v] == 0)
        {
            buscaProfOrdTopoR(G, v, visitado, ordTopo, &rotulo_atual);
            // buscaProfOrdTopoI(G, v, visitado, ordTopo, &rotulo_atual);
        }
    free(visitado);
}

Grafo readGraphPrint(FILE *entrada)
{
    int n, m, v, w;
    Grafo G;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = inicializaGrafo(n);
    for (v = 0; v < G->n; v++)
    {
        fscanf(entrada, "%d", &w);
        while (w != -1)
        {
            insereArcoNaoSeguraGrafo(G, v, w);
            fscanf(entrada, "%d", &w);
        }
    }
    return G;
}

int main(int argc, char *argv[])
{
    FILE *entrada;
    int i, *ordTopo;
    Grafo G;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\ordenacaoTopologica .\\instancias\\DAGident10_30print");
        return 0;
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        printf("Nao encontrei o arquivo!");
        exit(EXIT_FAILURE);
    }

    G = readGraphPrint(entrada);

    // imprimindo a rede como lista de adjacências
    mostraGrafo(G);

    ordTopo = malloc(G->n * sizeof(int));

    ordenacaoTopologica(G, ordTopo);

    printf("vertices:         ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", i);
    printf("\n");
    printf("ordem topologica: ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", ordTopo[i]);
    printf("\n");

    G = liberaGrafo(G);
    free(ordTopo);
    fclose(entrada);
    return 0;
}