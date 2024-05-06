#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fila.h"
#include "grafosListasAdj.h"

int *distancias(Grafo G, int origem)
{
    int v, w, *dist;
    Fila *fila;
    Noh *p;
    dist = malloc(G->n * sizeof(int));
    /* inicializa a fila */
    fila = criaFila(G->n);
    /* inicializa todos como não encontrados, exceto pela origem */
    for (v = 0; v < G->n; v++)
        dist[v] = -1;
    dist[origem] = 0;
    /* colocando origem na fila */
    insereFila(fila, origem);
    /* enquanto a fila dos ativos 
    (encontrados mas não visitados) 
    não estiver vazia */
    while (!filaVazia(fila))
    {
        /* remova o mais antigo da fila */
        v = removeFila(fila);
        /* para cada vizinho de v que ainda não foi encontrado */
        p = G->A[v];
        while (p != NULL)
        {
            w = p->rotulo;
            if (dist[w] == -1)
            {
                /* calcule a distancia do vizinho 
                e o coloque na fila */
                dist[w] = dist[v] + 1;
                insereFila(fila, w);
            }
            p = p->prox;
        }
    }
    fila = liberaFila(fila);
    return dist;
}

Grafo lerGrafoMatriz(FILE *entrada)
{
    int n, v, w, value;
    Grafo G;
    // Noh * p;
    fscanf(entrada, "%d\n", &n);
    G = inicializaGrafo(n);
    for (v = 0; v < G->n; v++)
        for (w = 0; w < G->n; w++)
        {
            fscanf(entrada, "%d", &value);
            if (value == 1)
            {
                // p = malloc(sizeof *p);
                // p->rotulo = w;
                // p->prox = G->A[v];
                // G->A[v] = p;
                // G->m++;
                insereArcoNaoSeguraGrafo(G, v, w);
            }
        }
    return G;
}

Grafo lerGrafoImpresso(FILE *entrada)
{
    int n, m, v, w;
    Grafo G;
    // Noh * p;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = inicializaGrafo(n);
    for (v = 0; v < G->n; v++)
    {
        fscanf(entrada, "%d", &w);
        while (w != -1)
        {
            // p = malloc(sizeof *p);
            // p->rotulo = w;
            // p->prox = G->A[v];
            // G->A[v] = p;
            // G->m++;
            insereArcoNaoSeguraGrafo(G, v, w);
            fscanf(entrada, "%d", &w);
        }
    }
    return G;
}

Grafo lerGrafoMostra(FILE *entrada)
{
    int n, m, v, w, tam;
    Grafo G;
    char *str, *aux;
    // Noh * p;
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
            // p = malloc(sizeof *p);
            // p->rotulo = w;
            // p->prox = G->A[v];
            // G->A[v] = p;
            // G->m++;
            insereArcoNaoSeguraGrafo(G, v, w);
            aux = strtok(NULL, " \n");
        }
    }
    free(str);
    return G;
}

int main(int argc, char *argv[])
{
    FILE *entrada;
    Grafo G;
    int i, *dist;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\distancias \".\\instancias\\inst10_30print\"");
        return 0;
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        printf("Nao encontrei o arquivo!");
        exit(EXIT_FAILURE);
    }

    // G = lerGrafoMatriz(entrada);
    G = lerGrafoImpresso(entrada);
    // G = lerGrafoMostra(entrada);

    // imprimindo o grafo lido como lista de adjacências
    mostraGrafo(G);

    dist = distancias(G, 0);
    // dist = distancias(G, n / 2);

    printf("vertices:   ");
    for (i = 0; i < G->n; i++)
        printf("%d ", i);
    printf("\n");
    printf("distancias: ");
    for (i = 0; i < G->n; i++)
        printf("%d ", dist[i]);
    printf("\n");

    G = liberaGrafo(G);
    free(dist);
    fclose(entrada);
    return 0;
}