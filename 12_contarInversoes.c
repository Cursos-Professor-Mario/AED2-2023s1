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

// baseado no insertionSort
unsigned long long contarInversoes1(int v[], int n)
{
    int i, j, aux;
    unsigned long long num_inv = 0;
    for (j = 1; j < n; j++)
    {
        aux = v[j];
        for (i = j - 1; i >= 0 && aux < v[i]; i--)
        {
            v[i + 1] = v[i];
            // num_inv++;
        }
        num_inv += j - 1 - i;
        v[i + 1] = aux; /* por que i+1? */
    }
    return num_inv;
}

// baseado no bubbleSort
unsigned long long contarInversoes2(int v[], int n)
{
    int j, i, aux, ut, l;
    unsigned long long num_inv = 0;
    l = n;
    for (j = 0; j < n; j++)
    {
        ut = 0;
        for (i = 1; i < l; i++)
            if (v[i - 1] > v[i])
            {
                aux = v[i - 1];
                v[i - 1] = v[i];
                v[i] = aux;
                ut = i;
                num_inv++;
            }
        l = ut;
    }
    return num_inv;
}

// primeiro subvetor entre p e q-1, segundo subvetor entre q e r-1
unsigned long long intercalaContando(int v[], int p, int q, int r)
{
    int i, j, k, tam;
    unsigned long long num_inv = 0;
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
        {
            w[k++] = v[j++];
            num_inv += q - i;
        }
    }
    while (i < q)
        w[k++] = v[i++];
    while (j < r)
        w[k++] = v[j++];
    for (k = 0; k < tam; k++)
        v[p + k] = w[k];

    free(w);
    return num_inv;
}

// baseado no mergeSort
// p indica a primeira posicao e r-1 a ultima
unsigned long long contarInversoesR(int v[], int p, int r)
{
    int m;
    unsigned long long num_inv = 0;
    if (r - p > 1)
    {
        m = (p + r) / 2;
        // m = p + (r - p) / 2;
        num_inv += contarInversoesR(v, p, m);
        num_inv += contarInversoesR(v, m, r);
        num_inv += intercalaContando(v, p, m, r);
    }
    return num_inv;
}

unsigned long long contarInversoes3(int v[], int n)
{
    return contarInversoesR(v, 0, n);
}

int main(int argc, char *argv[])
{
    int i, n;
    clock_t stime, etime;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: .\\contarInversoes 10\n");
        return 0;
    }

    n = atoi(argv[1]);

    int *v = (int *)malloc(n * sizeof(int));

    // ordem direta
    printf("ordem direta\n");
    for (i = 0; i < n; i++)
        v[i] = i * 10;
    // imprime(v, n);

    stime = clock();
    printf("numero inversoes = %llu\n", contarInversoes3(v, n));
    etime = clock();
    printf("contar inversoes com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem alternada
    printf("ordem alternada\n");
    for (i = 0; i < n; i += 2)
        v[i] = (i / 2) * 10;
    for (i = 1; i < n; i += 2)
        v[i] = (n - i / 2 - 1) * 10;
    // imprime(v, n);

    stime = clock();
    printf("numero inversoes = %llu\n", contarInversoes3(v, n));
    etime = clock();
    printf("contar inversoes com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    // ordem inversa
    printf("ordem inversa\n");
    for (i = 0; i < n; i++)
        v[i] = (n - i - 1) * 10;
    // imprime(v, n);

    stime = clock();
    printf("numero inversoes = %llu\n", contarInversoes3(v, n));
    etime = clock();
    printf("contar inversoes com n = %d leva tempo (em milisegundos) = %ld\n", n, (etime - stime) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
