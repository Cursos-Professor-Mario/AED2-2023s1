#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0

// #define bitsPalavra 32
// #define bitsDigito 8
// #define digitosPalavra (bitsPalavra / bitsDigito)
// #define Base (1 << bitsDigito)

const int bitsPalavra = 32;
const int bitsDigito = 16;
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

int pegaDigito(int chave, int digito)
{
    return (int)((chave >> (bitsDigito * (digitosPalavra - 1 - digito))) & (Base - 1));
}

int pegaDigito2(int chave, int digito)
{
    return (int)(chave / exp2(bitsDigito * (digitosPalavra - 1 - digito))) % Base;
}

// Rearranja v[0..n-1] em ordem crescente
// supondo que os elementos do vetor
// pertencem ao universo 0..R-1.
void countingSort(int v[], int n, int R)
{
    int r, i;
    int *fp, *aux;
    fp = malloc((R + 1) * sizeof(int));
    aux = malloc(n * sizeof(int));

    for (r = 0; r <= R; ++r)
        fp[r] = 0;
    for (i = 0; i < n; ++i)
    {
        r = v[i];
        fp[r + 1] += 1;
    }
    // agora fp[r] é a frequência de r-1
    for (r = 1; r <= R; ++r)
        fp[r] += fp[r - 1];
    // agora fp[r] é a freq dos predecessores de r
    // logo, a carreira de elementos iguais a r
    // deve começar no índice fp[r]
    for (int i = 0; i < n; ++i)
    {
        r = v[i];
        aux[fp[r]] = v[i];
        fp[r]++; // *
    }
    // aux[0..n-1] está em ordem crescente
    for (int i = 0; i < n; ++i)
        v[i] = aux[i];

    free(fp);
    free(aux);
}

void LSDradixSort(int v[], int n)
{
    int digito, valor, i;
    int *ocorr_pred, *aux;
    ocorr_pred = malloc((Base + 1) * sizeof(int));
    aux = malloc(n * sizeof(int));

    for (digito = digitosPalavra - 1; digito >= 0; digito--)
    {
        for (valor = 0; valor <= Base; valor++)
            ocorr_pred[valor] = 0;
        for (i = 0; i < n; i++)
        {
            valor = pegaDigito(v[i], digito);
            ocorr_pred[valor + 1] += 1;
        }
        // agora ocorr_pred[valor] é o número
        // de ocorrências de valor - 1
        for (valor = 1; valor <= Base; valor++)
            ocorr_pred[valor] += ocorr_pred[valor - 1];
        // agora ocorr_pred[valor] é o número de
        // ocorrências dos predecessores de valor.
        // Logo, a carreira de elementos iguais a valor
        // deve começar no índice ocorr_pred[valor].
        for (i = 0; i < n; ++i)
        {
            // note a diferença entre o valor analisado e copiado
            valor = pegaDigito(v[i], digito);
            aux[ocorr_pred[valor]] = v[i];
            ocorr_pred[valor]++; // atualiza o número de predecessores
        }
        // aux[0..n-1] está em ordem crescente considerando
        // apenas os digitos entre digito .. digitosPalavra - 1
        for (i = 0; i < n; i++)
            v[i] = aux[i];
    }
    free(ocorr_pred);
    free(aux);
}

int main(int argc, char *argv[])
{
    int i, n, R;
    clock_t stime, etime;

    if (argc != 3)
    {
        printf("Numero incorreto de parametros. Ex: .\\LSDradixSort 10 3\n");
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
    LSDradixSort(v, n);
    // countingSort(v, n, n);
    etime = clock();
    imprime(v, n);
    printf("LSDradixSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        v[i] = i % R;
    imprime(v, n);

    stime = clock();
    LSDradixSort(v, n);
    // countingSort(v, n, n);
    etime = clock();
    imprime(v, n);
    printf("LSDradixSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        v[i] = (n - i - 1) % R;
    imprime(v, n);

    stime = clock();
    LSDradixSort(v, n);
    // countingSort(v, n, n);
    etime = clock();
    imprime(v, n);
    printf("LSDradixSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
}