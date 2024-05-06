#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafosListasAdj.h"

void buscaCompR(Grafo G, int v, int *comp, int comp_atual)
{
    int w;
    Noh *p;
    comp[v] = comp_atual;
    /* para cada vizinho de v que ainda não foi visitado */
    p = G->A[v];
    while (p != NULL)
    {
        w = p->rotulo;
        if (comp[w] == -1)
            buscaCompR(G, w, comp, comp_atual);
        p = p->prox;
    }
}

void identComponetes(Grafo G, int *comp)
{
    int v, num_comps;
    /* inicializa todos os vértices como não pertencentes a qualquer componente */
    for (v = 0; v < G->n; v++)
        comp[v] = -1;
    num_comps = 0;
    for (v = 0; v < G->n; v++)
        if (comp[v] == -1)
        {
            num_comps++;
            buscaCompR(G, v, comp, num_comps);
        }
}

Grafo readGraphMatrixUndirected(FILE *entrada)
{
    int n, v, w, value;
    Grafo G;
    fscanf(entrada, "%d\n", &n);
    G = inicializaGrafo(n);
    for (v = 0; v < G->n; v++)
        for (w = 0; w < G->n; w++)
        {
            fscanf(entrada, "%d", &value);
            if (value == 1)
            {
                insereArcoNaoSeguraGrafo(G, v, w);
                insereArcoNaoSeguraGrafo(G, w, v);
            }
        }
    return G;
}

Grafo readGraphPrintUndirected(FILE *entrada)
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
            insereArcoNaoSeguraGrafo(G, w, v);
            fscanf(entrada, "%d", &w);
        }
    }
    return G;
}

Grafo readGraphShowUndirected(FILE *entrada)
{
    int n, m, v, w, tam;
    Grafo G;
    char *str, *aux;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = inicializaGrafo(n);
    tam = ((G->n * ((int)log10((double)G->n) + 1)) + 3) * sizeof(char);
    str = malloc(tam);
    for (v = 0; v < G->n; v++)
    {
        fgets(str, tam, entrada);
        aux = strtok(str, ":");
        aux = strtok(NULL, " \n");
        while (aux != NULL)
        {
            w = atoi(aux);
            insereArcoNaoSeguraGrafo(G, v, w);
            insereArcoNaoSeguraGrafo(G, w, v);
            aux = strtok(NULL, " \n");
        }
    }
    free(str);
    return G;
}

int main(int argc, char *argv[])
{
    FILE *entrada;
    int i, *comp;
    Grafo G;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\identCompLista \".\\instancias\\inst10_30print\"");
        return 0;
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        printf("Nao encontrei o arquivo!");
        exit(EXIT_FAILURE);
    }

    // G = readGraphMatrixUndirected(entrada);
    G = readGraphPrintUndirected(entrada);
    // G = readGraphShowUndirected(entrada);

    // imprimindo a rede como lista de adjacências
    mostraGrafo(G);

    comp = malloc(G->n * sizeof(int));

    identComponetes(G, comp);

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