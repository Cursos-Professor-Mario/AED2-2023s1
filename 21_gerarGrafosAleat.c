/* Para ter acesso à função rand() e à constante RAND_MAX, preciso do arquivo-interface stdlib.h. */
#include <stdio.h>
#include <stdlib.h>

// #include "grafosMatrizAdj.h"
#include "grafosListasAdj.h"

/* A função verticeAleatorio() devolve um vértice aleatório 
do grafo G. Vamos supor que G->n <= RAND_MAX. */
int verticeAleatorio(Grafo G)
{
    double r;
    r = (double)rand() / ((double)RAND_MAX + 1.0);
    return (int)(r * G->n);
}

/* Esta função constrói um grafo aleatório com vértices 0..n-1 
e exatamente m arcos. A função supõe que m <= n*(n-1). Se m 
for próximo de n*(n-1), a função pode consumir muito tempo. 
(Código inspirado no Programa 17.7 de Sedgewick.) */
Grafo grafoAleatorio1(int n, int m)
{
    Grafo G = inicializaGrafo(n);
    while (G->m < m)
    {
        int v = verticeAleatorio(G);
        int w = verticeAleatorio(G);
        if (v != w)
            insereArcoGrafo(G, v, w);
    }
    return G;
}

Grafo grafoAleatorio2(int n, int m)
{
    int v, w;
    double prob = (double)m / n / (n - 1);
    Grafo G = inicializaGrafo(n);
    for (v = 0; v < n; v++)
        for (w = 0; w < n; w++)
            if (v != w)
                if (rand() < prob * (RAND_MAX + 1.0))
                    insereArcoGrafo(G, v, w);
    return G;
}

Grafo grafoAleatorio2_1(int n, int m)
{
    int v, w;
    double prob = (double)m / n / (n - 1);
    Grafo G = inicializaGrafo(n);
    for (v = 0; v < n; v++)
        for (w = 0; w < n; w++)
            if (v != w)
                if (rand() < prob * (RAND_MAX + 1.0))
                    insereArcoNaoSeguraGrafo(G, v, w);
    return G;
}

int main(int argc, char *argv[])
{
    int n, m;
    Grafo G;
    FILE *saida;

    if (argc != 4)
    {
        printf("Numero incorreto de parametros! Ex.: .\\gerarGrafosAleat 10 30 inst10_30print");
        return 0;
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    srand(0);
    G = grafoAleatorio1(n, m);
    // printf("%d %d\n", n, m);
    // imprimeGrafo(G);
    // mostraGrafo(G);

    saida = fopen(argv[3], "w");
    imprimeArquivoGrafo(G, saida);
    fclose(saida);
    G = liberaGrafo(G);

    return 0;
}