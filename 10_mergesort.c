#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0

void imprime(int v[], int n)
{
    int i;
    if (DEBUG > 0)
    {
        for (i = 0; i < n; i++)
            printf("%d ", v[i]);
        printf("\n");
    }
}

void insertionSort(int v[], int n)
{
    int i, j, aux;
    for (j = 1; j < n; j++)
    {
        aux = v[j];
        for (i = j - 1; i >= 0 && aux < v[i]; i--)
            v[i + 1] = v[i];
        v[i + 1] = aux; /* por que i+1? */
    }
}

// primeiro subvetor em v[p .. q-1], segundo subvetor em v[q .. r-1]
void intercala1(int v[], int p, int q, int r)
{
    int i, j, k, tam;
    i = p;
    j = q;
    k = 0;
    tam = r - p;
    int *w = malloc(tam * sizeof(int));
    while (i < q && j < r)
    {
        if (v[i] <= v[j])
            w[k++] = v[i++];
        else // v[i] > v[j]
            w[k++] = v[j++];
    }
    while (j < r)
        w[k++] = v[j++];
    while (i < q)
        w[k++] = v[i++];
    for (k = 0; k < tam; k++)
        v[p + k] = w[k];
    free(w);
}

// primeiro subvetor em v[p .. q-1], segundo subvetor em v[q .. r-1]
void intercala2(int v[], int p, int q, int r)
{
    int i, j, k, *w;
    w = malloc((r - p) * sizeof(int));
    for (i = p; i < q; ++i)
        w[i - p] = v[i];
    for (j = q; j < r; ++j)
        w[(r - p - 1) - (j - q)] = v[j];
    i = 0;
    j = r - p - 1;
    for (k = p; k < r; ++k)
        if (w[i] <= w[j])
            v[k] = w[i++];
        else
            v[k] = w[j--];
    free(w);
}

// ordena o vetor v[p .. r-1]
void mergeSortR(int v[], int p, int r)
{
    int m;
    if (r - p > 1)
    {
        m = (p + r) / 2;
        // m = p + (r - p) / 2;
        mergeSortR(v, p, m); // 1o subproblema
        mergeSortR(v, m, r); // 2o subproblema
        intercala1(v, p, m, r);
    }
}

void mergeSort(int v[], int n)
{
    mergeSortR(v, 0, n);
}

void mergeSortI(int v[], int n)
{
    int b = 1;
    while (b < n)
    {
        int p = 0;
        while (p + b < n)
        {
            int r = p + 2 * b;
            if (r > n)
                r = n;
            intercala1(v, p, p + b, r);
            p = p + 2 * b;
        }
        b = 2 * b;
    }
}

int main(int argc, char *argv[])
{
    int i, n;
    clock_t stime, etime;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: mergesort 10\n");
        return 0;
    }

    n = atoi(argv[1]);

    int *v = (int *)malloc(n * sizeof(int));

    // ordem inversa
    for (i = 0; i < n; i++)
        v[i] = n - i - 1;
    imprime(v, n);

    stime = clock();
    mergeSort(v, n);
    etime = clock();
    printf("mergeSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    imprime(v, n);

    // ordem inversa
    for (i = 0; i < n; i++)
        v[i] = n - i - 1;
    imprime(v, n);

    stime = clock();
    mergeSortI(v, n);
    etime = clock();
    printf("mergeSort iterativo com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    imprime(v, n);

    // ordem inversa
    for (i = 0; i < n; i++)
        v[i] = n - i - 1;
    imprime(v, n);

    stime = clock();
    insertionSort(v, n);
    etime = clock();
    printf("insertionSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    imprime(v, n);

    return 0;
}
