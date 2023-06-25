#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

clock_t stime, etime;
double cpu_time_used;

#include "quickSort.h"

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

int binarySearch(int v[], int n, int x)
{
    int e, m, d;
    e = 0;
    d = n - 1;
    while (e <= d)
    {
        m = (e + d) / 2;
        if (v[m] == x)
            return m;
        if (v[m] < x)
            e = m + 1;
        else
            d = m - 1;
    }
    return -1;
}

int twoSumBinarySearch(int v[], int n, int alvo)
{
    int i, compl;
    sort(v, n);
    for (i = 0; i < n; i++)
    {
        compl = alvo - v[i];
        /* busca binaria pelo complemento */
        if (binarySearch(v, n, compl) != -1)
            return 1;
    }
    return 0;
}

int twoSumSorting(int v[], int n, int alvo)
{
    int i, j, compl;
    sort(v, n);
    j = n - 1;
    for (i = 0; i < j; i++)
    {
        compl = alvo - v[i];
        /* continuo a busca de onde parei */
        for (; j > i && v[j] >= compl; j--)
            if (v[j] == compl)
                return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i, n, achou;
    int *v;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Tente \".\\twoSum_avancado 10\" ");
        exit(0);
    }

    n = atoi(argv[1]);
    v = (int *)malloc(n * sizeof(int));

    /* preenche o vetor com pares não negativos */
    for (i = 0; i < n; i++)
        v[i] = 2 * (n - i - 1);
    // imprime(v, n);
    // printf("\n");

    printf("Testando metodo twoSumBruteForce\n");
    stime = clock();
    achou = twoSumBruteForce(v, n, 9);
    etime = clock();
    cpu_time_used = ((double)(etime - stime)) / CLOCKS_PER_SEC;
    if (achou == 1)
        printf("Encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    else
        printf("Nao encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    printf("\n");

    printf("Testando metodo twoSumLinearSearch\n");
    stime = clock();
    achou = twoSumLinearSearch(v, n, 9);
    etime = clock();
    cpu_time_used = ((double)(etime - stime)) / CLOCKS_PER_SEC;
    if (achou == 1)
        printf("Encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    else
        printf("Nao encontrou um par que soma 9 em %lf segundos\n ", cpu_time_used);
    printf("\n");

    printf("Testando metodo twoSumBinarySearch\n");
    stime = clock();
    achou = twoSumBinarySearch(v, n, 9);
    etime = clock();
    cpu_time_used = ((double)(etime - stime)) / CLOCKS_PER_SEC;
    if (achou == 1)
        printf("Encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    else
        printf("Nao encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    printf("\n");

    printf("Testando metodo twoSumSorting\n");
    stime = clock();
    achou = twoSumBinarySearch(v, n, 9);
    etime = clock();
    cpu_time_used = ((double)(etime - stime)) / CLOCKS_PER_SEC;
    if (achou == 1)
        printf("Encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    else
        printf("Nao encontrou um par que soma 9 em %lf segundos\n", cpu_time_used);
    printf("\n");

    return 0;
}