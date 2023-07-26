#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char byte;

#define DEBUG 0

void imprime(byte *v[], int n, int W)
{
    int i, j;
    if (DEBUG > 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < W; j++)
            {
                // printf("%d", (int)v[i][j]);
                printf("%c", v[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
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

// Rearranja em ordem lexicográfica um vetor v[0 .. n - 1]
// de strings. Cada v[i] é uma string v[i][0 .. W - 1]
// cujos elementos pertencem ao conjunto 0 .. R - 1.
void ordenacaoDigital(byte *v[], int n, int W)
{
    int *ocorr_pred, digito, valor, i, R = 256;
    byte **aux;
    ocorr_pred = malloc((R + 1) * sizeof(int));
    aux = malloc(n * sizeof(byte *));

    for (digito = W - 1; digito >= 0; digito--)
    {
        for (valor = 0; valor <= R; valor++)
            ocorr_pred[valor] = 0;
        for (i = 0; i < n; i++)
        {
            valor = v[i][digito];
            ocorr_pred[valor + 1] += 1;
        }
        // agora ocorr_pred[valor] é o número
        // de ocorrências de valor - 1
        for (valor = 1; valor <= R; valor++)
            ocorr_pred[valor] += ocorr_pred[valor - 1];
        // agora ocorr_pred[valor] é o número de
        // ocorrências dos predecessores de valor.
        // Logo, a carreira de elementos iguais a valor
        // deve começar no índice ocorr_pred[valor].
        for (i = 0; i < n; i++)
        {
            // note a diferença entre o valor analisado e copiado
            valor = v[i][digito];
            aux[ocorr_pred[valor]] = v[i];
            ocorr_pred[valor]++; // atualiza o número de predecessores
        }
        // aux[0..n-1] está em ordem crescente considerando
        // apenas os digitos entre digito .. W - 1
        for (i = 0; i < n; i++)
            v[i] = aux[i];
    }

    free(ocorr_pred);
    free(aux);
}

int main(int argc, char *argv[])
{
    int i, j, n, W, R;
    clock_t stime, etime;

    if (argc != 4)
    {
        printf("Numero incorreto de parametros. Ex: .\\ordenacaoDigital 10 5 3\n");
        return 0;
    }

    n = atoi(argv[1]);
    W = atoi(argv[2]);
    R = atoi(argv[3]);

    byte **v = (byte **)malloc(n * sizeof(byte *));
    for (i = 0; i < n; i++)
        v[i] = (byte *)malloc(W * sizeof(byte));

    // ordem aleatoria
    printf("ordem aleatoria\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < W; j++)
        {
            // v[i][j] = rand() % R;
            v[i][j] = 65 + ((rand() % (90 - 65 + 1)) % R);
        }
    imprime(v, n, W);

    stime = clock();
    ordenacaoDigital((byte **)v, n, W);
    etime = clock();
    imprime(v, n, W);
    printf("ordenacaoDigital com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < W; j++)
        {
            // v[i][j] = (W * i + j) % R;
            v[i][j] = 65 + (((W * i + j) % (90 - 65 + 1)) % R);
        }
    imprime(v, n, W);

    stime = clock();
    ordenacaoDigital((byte **)v, n, W);
    etime = clock();
    imprime(v, n, W);
    printf("ordenacaoDigital com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < W; j++)
        {
            // v[i][j] = (n - (W * i + j) % n) % R;
            v[i][j] = 65 + (((n - (W * i + j) % n) % (90 - 65 + 1)) % R);
        }
    imprime(v, n, W);

    stime = clock();
    ordenacaoDigital((byte **)v, n, W);
    etime = clock();
    imprime(v, n, W);
    printf("ordenacaoDigital com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    for (i = 0; i < n; i++)
        free(v[i]);
    free(v);
}