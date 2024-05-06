#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafosListasAdj.h"

void buscaProfTempoTermR(Grafo G, int v, int *visitado, int *tempoTermino, int *ptempo)
{
    int w;
    Noh *p;
    visitado[v] = 1; // marca v como visitado
    // para cada vizinho de v que ainda não foi visitado
    p = G->A[v];
    while (p != NULL)
    {
        w = p->rotulo;
        if (visitado[w] == 0)
            buscaProfTempoTermR(G, w, visitado, tempoTermino, ptempo);
        p = p->prox;
    }
    // observe que o vetor é indexado pelos tempos e armazena
    // os vértices em ordem crescente de tempo de término
    tempoTermino[*ptempo] = v;
    (*ptempo)++;
}

void loopBuscaProfTempoTerm(Grafo G, int *tempoTermino)
{
    int v, tempo, *visitado;
    visitado = malloc(G->n * sizeof(int));
    // inicializa todos como não visitados e sem tempo de término
    for (v = 0; v < G->n; v++)
    {
        visitado[v] = 0;
        tempoTermino[v] = -1;
    }
    tempo = 0;
    // inicia uma busca em profundidade a partir de cada vértice não visitado
    for (v = 0; v < G->n; v++)
        if (visitado[v] == 0)
            buscaProfTempoTermR(G, v, visitado, tempoTermino, &tempo);
    free(visitado);
}

void buscaProfIdentCompR(Grafo G, int v, int *comp, int num_comp)
{
    int w;
    Noh *p;
    comp[v] = num_comp; // coloca v no componente atual
    // para cada vizinho de v que ainda não foi visitado
    p = G->A[v];
    while (p != NULL)
    {
        w = p->rotulo;
        if (comp[w] == -1)
            buscaProfIdentCompR(G, w, comp, num_comp);
        p = p->prox;
    }
}

void loopBuscaProfIdentComp(Grafo G, int *tempoTermino, int *comp)
{
    int v, i, num_comp;
    // inicializa todos como não pertencentes
    for (v = 0; v < G->n; v++)
        comp[v] = -1;
    num_comp = 0;
    // inicia buscas a partir de vértices não visitados
    // seguindo a ordem decrescente dos tempos de término
    for (i = G->n - 1; i >= 0; i--)
    {
        v = tempoTermino[i];
        if (comp[v] == -1)
        {
            num_comp++;
            buscaProfIdentCompR(G, v, comp, num_comp);
        }
    }
}

void identCompForteConexo(Grafo G, int *comp)
{
    int u, v, *tempoTermino;
    Noh *p;
    Grafo Grev;
    Grev = inicializaGrafo(G->n);
    // reverte os arcos do grafo G
    for (u = 0; u < G->n; u++)
    {
        p = G->A[u];
        while (p != NULL)
        {
            v = p->rotulo;
            insereArcoGrafo(Grev, v, u);
            p = p->prox;
        }
    }
    tempoTermino = malloc(G->n * sizeof(int));
    loopBuscaProfTempoTerm(Grev, tempoTermino);
    Grev = liberaGrafo(Grev);
    loopBuscaProfIdentComp(G, tempoTermino, comp);
    free(tempoTermino);
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
    int i, *comp;
    Grafo G;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\compForteConexos .\\instancias\\inst10_30print");
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

    comp = malloc(G->n * sizeof(int));

    identCompForteConexo(G, comp);

    printf("vertices:   ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", i);
    printf("\n");
    printf("componente: ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", comp[i]);
    printf("\n");

    G = liberaGrafo(G);
    free(comp);
    fclose(entrada);
    return 0;
}