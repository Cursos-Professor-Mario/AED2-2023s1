#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void imprime(int v[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

// ordena um vetor v[0 .. n-1] de inteiros em [0, R)
// desde que não existam elementos repetidos
// copia ao invés de rearranjar
void countingSortErrado1(int v[], int n, int R)
{
    int *presente, valor, i;
    presente = malloc(R * sizeof(int));
    for (valor = 0; valor < R; valor++)
        presente[valor] = 0;
    for (i = 0; i < n; i++)
        presente[v[i]] = 1;
    i = 0;
    for (valor = 0; valor < R; valor++)
        if (presente[valor] == 1)
            v[i++] = valor;
    free(presente);
}

// ordena um vetor v[0 .. n-1] de inteiros em [0, R)
// copia ao invés de rearranjar
void countingSortErrado2(int v[], int n, int R)
{
    int *ocorrencias, valor, i, repet;
    ocorrencias = malloc(R * sizeof(int));
    for (valor = 0; valor < R; valor++)
        ocorrencias[valor] = 0;
    for (i = 0; i < n; ++i)
        ocorrencias[v[i]] += 1;
    i = 0;
    for (valor = 0; valor < R; valor++)
        for (repet = 0; repet < ocorrencias[valor]; repet++)
            v[i++] = valor;
    free(ocorrencias);
}

// ordena um vetor v[0 .. n-1] de inteiros em [0, R)
void countingSort(int v[], int n, int R)
{
    int valor, i;
    int *ocorrencias, *ocorr_pred, *aux;
    ocorrencias = malloc(R * sizeof(int));
    ocorr_pred = malloc(R * sizeof(int));
    aux = malloc(n * sizeof(int));

    for (valor = 0; valor < R; valor++)
        ocorrencias[valor] = 0;
    for (i = 0; i < n; i++)
        ocorrencias[v[i]] += 1;
    ocorr_pred[0] = 0;
    for (valor = 1; valor < R; valor++)
        ocorr_pred[valor] = ocorr_pred[valor - 1] + ocorrencias[valor - 1];
    // Os elementos iguais a valor devem
    // começar no índice ocorr_pred[valor]
    for (i = 0; i < n; i++)
    {
        valor = v[i];
        aux[ocorr_pred[valor]] = v[i];
        ocorr_pred[valor]++; // atualiza o número de predecessores
    }
    // aux[0 .. n-1] está em ordem crescente
    for (i = 0; i < n; ++i)
        v[i] = aux[i];

    free(ocorrencias);
    free(ocorr_pred);
    free(aux);
}

// ordena um vetor v[0 .. n-1] de inteiros em [0, R)
void countingSort2(int v[], int n, int R)
{
    int valor, i;
    int *ocorr_pred, *aux;
    ocorr_pred = malloc((R + 1) * sizeof(int));
    aux = malloc(n * sizeof(int));

    for (valor = 0; valor <= R; valor++)
        ocorr_pred[valor] = 0;
    for (i = 0; i < n; i++)
    {
        valor = v[i];
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
        valor = v[i];
        aux[ocorr_pred[valor]] = v[i];
        ocorr_pred[valor]++; // atualiza o número de predecessores
    }
    // aux[0 .. n-1] está em ordem crescente
    for (i = 0; i < n; ++i)
        v[i] = aux[i];

    free(ocorr_pred);
    free(aux);
}

int main(int argc, char *argv[])
{
    int i, n, R;
    clock_t stime, etime;

    if (argc != 3)
    {
        printf("Numero incorreto de parametros. Ex: .\\countingSort 10 3\n");
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
    countingSort2(v, n, n);
    etime = clock();
    imprime(v, n);
    printf("countingSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        v[i] = i % R;
    imprime(v, n);

    stime = clock();
    countingSort2(v, n, n);
    etime = clock();
    imprime(v, n);
    printf("countingSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        v[i] = (n - i - 1) % R;
    imprime(v, n);

    stime = clock();
    countingSort2(v, n, n);
    etime = clock();
    imprime(v, n);
    printf("countingSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
}