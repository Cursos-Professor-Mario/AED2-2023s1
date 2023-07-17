#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// soma os valores em v[e .. d] e devolve em *psoma
void somaSeg(int v[], int e, int d, int *psoma)
{
    int i;
    *psoma = 0;
    for (i = e; i <= d; i++)
        *psoma += v[i];
}

// encontra o segmento de soma máxima em v[0 .. n - 1] e
// devolve seus limites em *pe, *pd e valor em *psMax
void segMax3(int v[], int n, int *pe, int *pd, int *psMax)
{
    int i, j, k, sAux;
    *psMax = 0;
    *pe = *pd = -1;
    for (i = 0; i < n; i++)     // i é o início do segmento corrente
        for (j = i; j < n; j++) // j é o final de tal segmento
        {
            // somaSeg(v, i, j, &sAux);
            sAux = 0;
            for (k = i; k <= j; k++)
                sAux += v[k];
            if (sAux > *psMax)
            {
                *psMax = sAux;
                *pe = i;
                *pd = j;
            }
        }
}

// encontra o segmento de soma máxima em v[0 .. n - 1] e
// devolve seus limites em *pe, *pd e valor em *psMax
void segMax2(int v[], int n, int *pe, int *pd, int *psMax)
{
    int i, j, sAux;
    *psMax = 0;
    *pe = *pd = -1;
    for (i = 0; i < n; i++) // i é o início do segmento corrente
    {
        sAux = 0;
        for (j = i; j < n; j++) // j é o final de tal segmento
        {
            sAux += v[j];
            if (sAux > *psMax)
            {
                *psMax = sAux;
                *pe = i;
                *pd = j;
            }
        }
    }
}

// encontra o segmento de soma máxima em v[0 .. n - 1] e
// devolve seus limites em *pe, *pd e valor em *psMax
void segMax1(int v[], int n, int *pe, int *pd, int *psMax)
{
    int i, j, sAux;
    *psMax = 0;
    *pe = *pd = -1;
    // seja S[j - 1] o seg. soma máx. que termina e contém j - 1
    sAux = 0;                   // SAux guarda o valor de S[j - 1]
    for (i = j = 0; j < n; j++) // i é o início de S[j - 1]
    {
        if (sAux >= 0)
            sAux += v[j];
        else // sAux < 0
        {
            sAux = v[j];
            i = j;
        }
        if (sAux > *psMax)
        {
            *psMax = sAux;
            *pe = i;
            *pd = j;
        }
    }
}

// encontra o segmento de soma máxima em v[0 .. n - 1] e
// devolve seus limites em *pe, *pd e valor em *psMax
void segMax0(int v[], int n, int *pe, int *pd, int *psMax)
{
    int i, j, sAux;
    *psMax = 0;
    *pe = *pd = -1;
    // seja S[j - 1] o seg. soma máx. que termina, mas pode não conter j - 1
    sAux = 0;                   // SAux guarda o valor de S[j - 1]
    for (i = j = 0; j < n; j++) // i é o início de S[j - 1]
    {
        if (sAux + v[j] > 0)
            sAux += v[j];
        else // sAux + v[j] <= 0
        {
            sAux = 0;
            i = j + 1;
        }
        if (sAux > *psMax)
        {
            *psMax = sAux;
            *pe = i;
            *pd = j;
        }
    }
}

int main(int argc, char *argv[])
{
    int i, n;
    int *v;
    int e, d, sMax;
    clock_t stime, etime;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex.: segSomaMax 20");
        return 0;
    }

    n = atoi(argv[1]);
    v = (int *)malloc(n * sizeof(int));

    // vetor homogeneo
    // for (i = 0; i < n; i++)
    //     v[i] = 1;

    // vetor aleatório
    for (i = 0; i < n; i++)
        v[i] = rand() - RAND_MAX / 2;

    // for (i = 0; i < n; i++)
    //     printf("%d ", v[i]);
    // printf("\n");

    stime = clock();
    segMax0(v, n, &e, &d, &sMax);
    etime = clock();
    printf("segMax0 time (in milliseconds) = %ld\n", (etime - stime) / (CLOCKS_PER_SEC / 1000));

    printf("sMax = %d, e = %d, d = %d\n", sMax, e, d);
    // for (i = e; i <= d; i++)
    //     printf("%d ", v[i]);
    // printf("\n");

    stime = clock();
    segMax1(v, n, &e, &d, &sMax);
    etime = clock();
    printf("segMax1 time (in milliseconds) = %ld\n", (etime - stime) / (CLOCKS_PER_SEC / 1000));

    printf("sMax = %d, e = %d, d = %d\n", sMax, e, d);
    // for (i = e; i <= d; i++)
    //     printf("%d ", v[i]);
    // printf("\n");

    stime = clock();
    segMax2(v, n, &e, &d, &sMax);
    etime = clock();
    printf("segMax2 time (in milliseconds) = %ld\n", (etime - stime) / (CLOCKS_PER_SEC / 1000));

    printf("sMax = %d, e = %d, d = %d\n", sMax, e, d);
    // for (i = e; i <= d; i++)
    //     printf("%d ", v[i]);
    // printf("\n");

    stime = clock();
    segMax3(v, n, &e, &d, &sMax);
    etime = clock();
    printf("segMax3 (in milliseconds) = %ld\n", (etime - stime) / (CLOCKS_PER_SEC / 1000));

    printf("sMax = %d, e = %d, d = %d\n", sMax, e, d);
    // for (i = e; i <= d; i++)
    //     printf("%d ", v[i]);
    // printf("\n");

    return 0;
}
