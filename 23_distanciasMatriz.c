#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fila.h"
#include "grafosMatrizAdj.h"

// A função recebe um inteiro x, uma matriz de inteiros A e a dimensão da matriz n, sendo 0 <= x < n.
// Ela devolve um vetor contendo a distância de x até cada elemento entre 0 e n-1.
int *distancias(Graph G, int s)
{
    int i, y;
    int *dist;
    Fila *q;

    dist = malloc(G->n * sizeof(int));
    /* inicializa a fila */
    q = criaFila(G->n);
    /* inicializa todos como não encontrados, exceto pelo x */
    for (i = 0; i < G->n; i++)
        dist[i] = -1;
    dist[s] = 0;
    /* colocando x na fila */
    insereFila(q, s);
    /* enquanto a fila dos ativos (encontrados 
    mas não visitados) não estiver vazia */
    while (!filaVazia(q))
    {
        /* remova o mais antigo da fila */
        y = removeFila(q);
        /* para cada vizinho deste que ainda não foi encontrado */
        for (i = 0; i < G->n; i++)
            if (G->A[y][i] == 1 && dist[i] == -1)
            {
                /* calcule a distancia do vizinho 
                e o coloque na fila */
                dist[i] = dist[y] + 1;
                insereFila(q, i);
            }
    }
    q = liberaFila(q);
    return dist;
}

Graph readGraphMatrix(FILE *entrada)
{
    int n, v, w, value;
    Graph G;
    fscanf(entrada, "%d\n", &n);
    G = graphInit(n);
    for (v = 0; v < G->n; v++)
        for (w = 0; w < G->n; w++)
        {
            fscanf(entrada, "%d", &value);
            if (value == 1)
            {
                // G->A[v][w] = 1;
                // G->m++;
                graphInsertArcNotSafe(G, v, w);
            }
        }
    return G;
}

Graph readGraphPrint(FILE *entrada)
{
    int n, m, v, w;
    Graph G;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = graphInit(n);
    for (v = 0; v < G->n; v++)
    {
        fscanf(entrada, "%d", &w);
        while (w != -1)
        {
            // G->A[v][w] = 1;
            // G->m++;
            graphInsertArcNotSafe(G, v, w);
            fscanf(entrada, "%d", &w);
        }
    }
    return G;
}

Graph readGraphShow(FILE *entrada)
{
    int n, m, v, w, tam;
    Graph G;
    char *str, *aux;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = graphInit(n);
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
            // G->A[v][w] = 1;
            // G->m++;
            graphInsertArcNotSafe(G, v, w);
            aux = strtok(NULL, " \n");
        }
    }
    free(str);
    return G;
}

int main(int argc, char *argv[])
{
    FILE *entrada;
    int i, *dist;
    Graph G;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\distancias \".\\instancia\\inst10_30print\"");
        return 0;
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        printf("Nao encontrei o arquivo!");
        exit(EXIT_FAILURE);
    }

    // G = readGraphMatrix(entrada);
    G = readGraphPrint(entrada);
    // G = readGraphShow(entrada);

    // imprimindo o grafo lido como lista de adjacências
    graphShow(G);

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

    G = graphFree(G);
    free(dist);
    fclose(entrada);
    return 0;
}