#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAI(i) (i - 1) / 2
#define FILHO_ESQ(i) (2 * i + 1)
#define FILHO_DIR(i) (2 * i + 2)

void imprime(int v[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

void troca(int *a, int *b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

// baseado no selectionSort
int selecao1(int v[], int n, int k)
{
    int i, j, ind_min;
    for (i = 0; i <= k; i++)
    {
        ind_min = i;
        for (j = i + 1; j < n; j++)
            if (v[j] < v[ind_min])
                ind_min = j;
        troca(&v[i], &v[ind_min]);
    }
    return v[k];
}

void desceHeap(int v[], int m, int k)
{
    int p = k, f;
    while (FILHO_ESQ(p) < m && (v[FILHO_ESQ(p)] > v[p] || (FILHO_DIR(p) < m && v[FILHO_DIR(p)] > v[p])))
    {
        f = FILHO_ESQ(p);
        if (FILHO_DIR(p) < m &&
            v[FILHO_DIR(p)] > v[f])
            f = FILHO_DIR(p);
        troca(&v[p], &v[f]);
        p = f;
    }
}

// baseado no heapSort
int selecao2(int v[], int n, int k)
{
    int i, m = n;
    for (i = n / 2; i >= 0; i--)
        desceHeap(v, n, i);
    for (m = n - 1; m >= k; m--)
    {
        troca(&v[0], &v[m]);
        desceHeap(v, m, 0);
    }
    return v[k];
}

int separa(int v[], int p, int r)
{
    int i, j, c = v[r];
    i = p;
    for (j = p; j < r; j++)
        if (v[j] <= c)
        {
            troca(&v[i], &v[j]);
            i++;
        }
    troca(&v[i], &v[r]);
    return i;
}

// baseado no quickSort determinístico
// p indica a primeira posicao e r a ultima
int selecao3(int v[], int p, int r, int k)
{
    int j;
    j = separa(v, p, r);
    if (k == j)
        return v[j];
    if (k < j)
        return selecao3(v, p, j - 1, k);
    // if (k > j)
    return selecao3(v, j + 1, r, k);
}

// baseado no quickSort aleatorizado
// p indica a primeira posicao e r a ultima
int selecao4(int v[], int p, int r, int k)
{
    int desl, j;
    desl = (int)(((double)rand() / (RAND_MAX + 1)) * (double)(r - p + 1));
    troca(&v[p + desl], &v[r]);
    j = separa(v, p, r);
    if (k == j)
        return v[j];
    if (k < j)
        return selecao4(v, p, j - 1, k);
    // if (k > j)
    return selecao4(v, j + 1, r, k);
}

// versão iterativa obtida pela remoção da recursão caudal do anterior
// p indica a primeira posicao e r a ultima
int selecao5(int v[], int p, int r, int k)
{
    int desl, j;
    while (1)
    {
        desl = (int)(((double)rand() / (RAND_MAX + 1)) * (double)(r - p + 1));
        troca(&v[p + desl], &v[r]);
        j = separa(v, p, r);
        if (k == j)
            return v[j];
        if (k < j)
            r = j - 1;
        else // if (k > j)
            p = j + 1;
    }
}

// versão iterativa refinada
// p indica a primeira posicao e r a ultima
int selecao6(int v[], int n, int k)
{
    int desl, j;
    int p = 0;
    int r = n - 1;
    do
    {
        desl = (int)(((double)rand() / (RAND_MAX + 1)) * (double)(r - p + 1));
        // printf("p = %d, r = %d, r-p+1 = %d, desl = %d\n", p, r, r - p + 1, desl);
        troca(&v[p + desl], &v[r]);
        j = separa(v, p, r);
        if (k < j)
            r = j - 1;
        else // if (k > j)
            p = j + 1;
    } while (k != j);
    return v[j];
}

int selecao(int v[], int n, int k)
{
    if (k < n)
    {
        // return selecao1(v, n, k);
        // return selecao2(v, n, k);
        // return selecao3(v, 0, n - 1, k);
        // return selecao4(v, 0, n - 1, k);
        // return selecao5(v, 0, n - 1, k);
        return selecao6(v, n, k);
    }
    printf("Erro! k = %d >= %d = n\n", k, n);
    exit(1);
}

int main(int argc, char *argv[])
{
    int i, n, k;
    clock_t stime, etime;

    if (argc != 3)
    {
        printf("Numero incorreto de parametros. Ex: .\\selecao 10 4\n");
        return 0;
    }

    n = atoi(argv[1]);
    k = atoi(argv[2]);

    int *v = (int *)malloc(n * sizeof(int));

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        v[i] = i * 10;
    // imprime(v, n);

    stime = clock();
    printf("%d-esimo = %d\n", k, selecao(v, n, k));
    etime = clock();
    printf("selecao com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem alternada
    printf("ordem alternada\n");
    for (i = 0; i < n; i += 2)
        v[i] = (i / 2) * 10;
    for (i = 1; i < n; i += 2)
        v[i] = (n - i / 2 - 1) * 10;
    // imprime(v, n);

    stime = clock();
    printf("%d-esimo = %d\n", k, selecao(v, n, k));
    etime = clock();
    printf("selecao com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        v[i] = (n - i - 1) * 10;
    // imprime(v, n);

    stime = clock();
    printf("%d-esimo = %d\n", k, selecao(v, n, k));
    etime = clock();
    printf("selecao com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
