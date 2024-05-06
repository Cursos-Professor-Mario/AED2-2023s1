#include <stdio.h>
#include <stdlib.h>

// #include "grafosMatrizAdj.h"
#include "grafosListasAdj.h"

int main(int argc, char *argv[])
{
    int i, j, n, aux;
    Grafo G;

    printf("Digite o numero de vertices.\n");
    scanf("%d", &n);

    G = inicializaGrafo(n);

    printf("Digite a matriz binaria correspondente ao grafo.\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            scanf("%d", &aux);
            if (aux == 1)
                insereArcoGrafo(G, i, j);
        }

    mostraGrafo(G);

    G = liberaGrafo(G);
    return 0;
}