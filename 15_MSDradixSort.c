#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0

// #define bitsPalavra 32
// #define bitsDigito 1
// #define digitosPalavra (bitsPalavra / bitsDigito)
// #define Base (1 << bitsDigito)

const int bitsPalavra = 32;
const int bitsDigito = 1;
const int digitosPalavra = bitsPalavra / bitsDigito;
const int Base = 1 << bitsDigito; // Base = 2^bitsDigito

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

void troca(int *a, int *b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int pegaDigito(int chave, int digito)
{
    // return (int)((chave >> (bitsDigito * (digitosPalavra - 1 - digito))) & (Base - 1));
    return (int)((chave >> (digitosPalavra - 1 - digito)) & (Base - 1));
}

int pegaDigito2(int chave, int digito)
{
    // return (int)(chave / exp2(bitsDigito * (digitosPalavra - 1 - digito))) % Base;
    return (int)(chave / exp2(digitosPalavra - 1 - digito)) % Base;
}

// esq indica a primeira posicao, dir indica a ultima, digito indica o digito corrente
void quicksortBin(int v[], int esq, int dir, int digito)
{
    int i, j;
    i = esq;
    j = dir;
    // caso base quando subvetor é pequeno ou acabaram os dígitos
    if (dir <= esq || digito > digitosPalavra)
        return;
    // separa as chaves do subvetor de acordo com o dígito corrente
    while (j > i)
    {
        while (pegaDigito(v[i], digito) == 0 && i < j)
            i++;
        while (pegaDigito(v[j], digito) == 1 && j > i)
            j--;
        troca(&v[i], &v[j]);
    }
    // ajusta j para que v[esq .. j-1] tenha chaves com digito 0
    if (pegaDigito(v[dir], digito) == 0)
        j++;
    quicksortBin(v, esq, j - 1, digito + 1);
    // quicksortBin(v, esq, j - 1, digito + bitsDigito);
    quicksortBin(v, j, dir, digito + 1);
    // quicksortBin(v, j, dir, digito + bitsDigito);
}

void MSDradixSort(int v[], int n)
{
    quicksortBin(v, 0, n - 1, 0);
}

int main(int argc, char *argv[])
{
    int i, n, R;
    clock_t stime, etime;

    if (argc != 3)
    {
        printf("Numero incorreto de parametros. Ex: MSDradixSort 10 3\n");
        return 0;
    }

    n = atoi(argv[1]);

    R = atoi(argv[2]);

    int *v = (int *)malloc(n * sizeof(int));

    // ordem alternada
    printf("ordem alternada\n");
    for (i = 0; i < n; i += 2)
        v[i] = (i / 2) % R;
    for (i = 1; i < n; i += 2)
        v[i] = (n - i / 2 - 1) % R;
    imprime(v, n);

    stime = clock();
    MSDradixSort(v, n);
    etime = clock();
    imprime(v, n);
    printf("MSDradixSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        v[i] = i % R;
    imprime(v, n);

    stime = clock();
    MSDradixSort(v, n);
    etime = clock();
    imprime(v, n);
    printf("MSDradixSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        v[i] = (n - i - 1) % R;
    imprime(v, n);

    stime = clock();
    MSDradixSort(v, n);
    etime = clock();
    imprime(v, n);
    printf("MSDradixSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
}