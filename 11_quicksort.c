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

void troca(int *a, int *b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

// separa v[p .. r] e devolve a posição do pivô
int separa1(int v[], int p, int r)
{
    int i = p, j = r - 1, c = v[r]; // c é o pivô
    while (1)
    {
        while (i < r && v[i] <= c)
            i++;
        while (j > i && v[j] > c)
            j--;
        if (i >= j)
            break;
        troca(&v[i], &v[j]);
        // i++;
        // j--;
    }
    troca(&v[i], &v[r]);
    return i;
}

// separa v[p .. r] e devolve a posição do pivô
int separa2(int v[], int p, int r)
{
    int i, j, c = v[r]; // c é o pivô
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

// p indica a primeira posicao e r a ultima
void quicksortR(int v[], int p, int r)
{
    int i;
    if (p < r) // se vetor corrente tem mais de um elemento
    {
        i = separa2(v, p, r); // i é a posição do pivô após a separação
        quicksortR(v, p, i - 1);
        quicksortR(v, i + 1, r);
    }
}

// p indica a primeira posicao e r a ultima
void quicksortRAleat(int v[], int p, int r)
{
    int desl, i;
    if (p < r)
    {
        // desl = rand() % (r - p + 1);
        desl = (int)(((double)rand() / (RAND_MAX + 1)) * (double)(r - p + 1));
        // printf("p = %d, r = %d, r-p+1 = %d, desl = %d\n", p, r, r - p + 1, desl);
        troca(&v[p + desl], &v[r]);
        i = separa1(v, p, r);
        quicksortRAleat(v, p, i - 1);
        quicksortRAleat(v, i + 1, r);
    }
}

void quicksortRSemiIter(int v[], int p, int r)
{
    int i;
    while (p < r)
    {
        i = separa1(v, p, r);
        if (i - p < r - i) // se o subvetor esquerdo é menor
        {                  // ordena recursivamente o subvetor esquerdo
            quicksortRSemiIter(v, p, i - 1);
            p = i + 1;
        }
        else // se o subvetor direito é menor
        {    // ordena recursivamente o subvetor direito
            quicksortRSemiIter(v, i + 1, r);
            r = i - 1;
        }
    }
}

// p indica a primeira posicao e r a ultima
void quicksortRAleatRepet(int v[], int p, int r, int n)
{
    int i, j, k, desl;
    if (p < r)
    {
        desl = (int)(((double)rand() / (RAND_MAX + 1)) * (double)(r - p + 1));
        troca(&v[p + desl], &v[r]);
        j = separa2(v, p, r);
        i = j;
        for (k = i - 1; k >= p; k--)
            if (v[k] == v[i])
            {
                i--;
                troca(&v[i], &v[k]);
            }
        quicksortRAleatRepet(v, p, i - 1, n);
        quicksortRAleatRepet(v, j + 1, r, n);
    }
}

void quicksort(int v[], int n)
{
    quicksortRAleatRepet(v, 0, n - 1, n);
}

int main(int argc, char *argv[])
{
    int i, n;
    clock_t stime, etime;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: quicksortR 10\n");
        return 0;
    }

    n = atoi(argv[1]);

    int *v = (int *)malloc(n * sizeof(int));

    // ordem alternada
    printf("ordem alternada\n");
    for (i = 0; i < n; i += 2)
        v[i] = i / 2;
    for (i = 1; i < n; i += 2)
        v[i] = n - i / 2 - 1;
    // imprime(v, n);

    stime = clock();
    quicksort(v, n);
    etime = clock();
    printf("quicksort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    // imprime(v, n);

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        v[i] = i;
    // imprime(v, n);

    stime = clock();
    quicksort(v, n);
    etime = clock();
    printf("quicksort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    // imprime(v, n);

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        v[i] = n - i - 1;
    // imprime(v, n);

    stime = clock();
    quicksort(v, n);
    etime = clock();
    printf("quicksort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    // imprime(v, n);

    // blocos
    printf("blocos\n");
    for (i = 0; i < n; i++)
        v[i] = i % 3;
    // imprime(v, n);

    stime = clock();
    quicksort(v, n);
    etime = clock();
    printf("quicksort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    // imprime(v, n);

    // todos iguais
    printf("todos iguais\n");
    for (i = 0; i < n; i++)
        v[i] = 0;
    // imprime(v, n);

    stime = clock();
    quicksort(v, n);
    etime = clock();
    printf("quicksort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    // imprime(v, n);

    // // ordem inversa
    // for (i = 0; i < n; i++)
    //     v[i] = n - i - 1;
    // // imprime(v, n);

    // stime = time(NULL);
    // insertionSort(v, n);
    // etime = time(NULL);
    // printf("insertionSort com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));
    // // imprime(v, n);

    return 0;
}
