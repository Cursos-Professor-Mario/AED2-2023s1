/* Para ter acesso à função rand() e à constante RAND_MAX, preciso do arquivo-interface stdlib.h. */
#include <stdio.h>
#include <stdlib.h>

#include "grafosListasAdj.h"

/* Sorteia um inteiro em [0, n) */
int uniformeAleat(int n)
{
    return (int)((double)rand() / (double)(RAND_MAX + 1) * n);
}

void troca(int *a, int *b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

// Knuth shuffles
int *permAleat(int *perm, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        j = i + uniformeAleat(n - i);
        troca(&perm[i], &perm[j]);
    }
    return perm;
}

/* A função verticeAleatorio() devolve um vértice aleatório 
do grafo G. Vamos supor que G->n <= RAND_MAX. */
int verticeAleatorio(Grafo G)
{
    double r;
    r = (double)rand() / ((double)RAND_MAX + 1.0);
    return (int)(r * G->n);
}

Grafo DAGaleatorio1(int n, int m, int *perm)
{
    Grafo G = inicializaGrafo(n);
    while (G->m < m)
    {
        int v = verticeAleatorio(G);
        int w = verticeAleatorio(G);
        // verificando se o arco respeita a orientação do DAG dada por perm
        if (perm[v] < perm[w])
            insereArcoGrafo(G, v, w);
    }
    return G;
}

Grafo DAGaleatorio2(int n, int m, int *perm)
{
    // ajuste no cálculo da probabilidade, pois número máximo de arcos num DAG é menor
    double prob = (double)m / n / (n - 1) * 2;
    Grafo G = inicializaGrafo(n);
    for (int v = 0; v < n; v++)
        for (int w = 0; w < n; w++)
        {
            // verificando se o arco respeita a orientação do DAG dada por perm
            if (perm[v] < perm[w])
                if (rand() < prob * (RAND_MAX + 1.0))
                    insereArcoGrafo(G, v, w);
        }
    return G;
}

Grafo DAGaleatorio2_1(int n, int m, int *perm)
{
    // ajuste no cálculo da probabilidade, pois número máximo de arcos num DAG é menor
    double prob = (double)m / n / (n - 1) * 2;
    Grafo G = inicializaGrafo(n);
    for (int v = 0; v < n; v++)
        for (int w = 0; w < n; w++)
        {
            // verificando se o arco respeita a orientação do DAG dada por perm
            if (perm[v] < perm[w])
                if (rand() < prob * (RAND_MAX + 1.0))
                    insereArcoNaoSeguraGrafo(G, v, w);
        }
    return G;
}

int main(int argc, char *argv[])
{
    int n, m, *perm, i;
    Grafo G;
    // FILE *saida;

    if (argc != 4)
    {
        printf("Numero incorreto de parametros! Ex.: .\\gerarDAGAleat 10 30 inst10_30print");
        return 0;
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    perm = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        perm[i] = i + 1;

    permAleat(perm, n);

    printf("vertices =   ");
    for (i = 0; i < n; i++)
        printf(" %2d", i);
    printf("\n");
    printf("permutacao = ");
    for (i = 0; i < n; i++)
        printf(" %2d", perm[i]);
    printf("\n");

    srand(0);
    G = DAGaleatorio1(n, m, perm);
    printf("%d %d\n", n, m);
    // imprimeGrafo(G);
    mostraGrafo(G);

    // saida = fopen(argv[3], "w");
    // imprimeArquivoGrafo(G, saida);
    // fclose(saida);
    G = liberaGrafo(G);
    free(perm);

    return 0;
}