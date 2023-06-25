#include <stdio.h>
#include <stdlib.h>

/* imprime o vetor de inteiros v[0..n-1] */
void imprime(int v[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

int twoSumBruteForce(int v[], int n, int alvo)
{
    int i, j;
    /* testa todos os pares */
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (v[i] + v[j] == alvo)
                return 1;
    return 0;
}

int twoSumLinearSearch(int v[], int n, int alvo)
{
    int i, j, compl;
    for (i = 0; i < n; i++)
    {
        compl = alvo - v[i];
        /* busca linear pelo complemento */
        for (j = i + 1; j < n; j++)
            if (v[j] == compl)
                return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i, n;
    int *v;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Tente \".\\twoSum_basico 10\" ");
        exit(0);
    }

    n = atoi(argv[1]);
    v = (int *)malloc(n * sizeof(int));

    /* preenche o vetor com pares não negativos */
    for (i = 0; i < n; i++)
        v[i] = 2 * i;
    imprime(v, n);
    printf("\n");

    printf("Testando metodo twoSumBruteForce\n");
    if (twoSumBruteForce(v, n, 10) == 1)
        printf("Encontrou um par que soma 10\n");
    else
        printf("Nao encontrou um par que soma 10\n ");
    if (twoSumBruteForce(v, n, 9) == 1)
        printf("Encontrou um par que soma 9\n");
    else
        printf("Nao encontrou um par que soma 9\n ");
    printf("\n");

    printf("Testando metodo twoSumLinearSearch\n");
    if (twoSumLinearSearch(v, n, 10) == 1)
        printf("Encontrou um par que soma 10\n");
    else
        printf("Nao encontrou um par que soma 10\n ");
    if (twoSumLinearSearch(v, n, 9) == 1)
        printf("Encontrou um par que soma 9\n");
    else
        printf("Nao encontrou um par que soma 9\n ");
    printf("\n");

    return 0;
}