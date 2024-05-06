#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define maximo(a, b) (a > b ? a : b)

// Recebe vetores palavra[1..m] e texto[1..n],
// com m >= 1 e n >= 0, e devolve o número de
// ocorrências de palavra em texto.
int basico(char palavra[], int m, char texto[], int n)
{
    int pos_t, desl_p, ocorrs;
    ocorrs = 0;
    for (pos_t = m; pos_t <= n; pos_t++)
    {
        desl_p = 0;
        // palavra[1..m] casa com texto[pos_t-m+1 .. pos_t]?
        while (desl_p < m && palavra[m - desl_p] == texto[pos_t - desl_p])
            desl_p++;
        if (desl_p >= m)
            ocorrs++;
    }
    return ocorrs;
}

int *preProcBadCharac(char palavra[], int m)
{
    int i;
    int *dist_ult = malloc(256 * sizeof(int)); // o alfabeto é 0..255
    for (i = 0; i < 256; i++)
        dist_ult[i] = m;
    for (i = 1; i <= m; i++) // ordem do laço importa?
        dist_ult[(int)palavra[i]] = m - i;
    return dist_ult;
}

// Recebe vetores palavra[1..m] e texto[1..n],
// com m >= 1 e n >= 0, e devolve o número de
// ocorrências de palavra em texto.
int BoyerMoore1(char palavra[], int m, char texto[], int n)
{
    int *dist_ult;
    int pos_t, desl_p, ocorrs;
    // pré-processamento da palavra p
    dist_ult = preProcBadCharac(palavra, m);
    // busca da palavra p no texto t
    ocorrs = 0;
    pos_t = m;
    while (pos_t <= n)
    {
        desl_p = 0;
        // palavra[1..m] casa com p[pos_t-m+1..pos_t]?
        while (desl_p < m && palavra[m - desl_p] == texto[pos_t - desl_p])
            desl_p++;
        if (desl_p >= m)
            ocorrs++;
        if (pos_t == n) // para evitar acessar posição inválida de memória
            pos_t += 1;
        else
            pos_t += 1 + dist_ult[(int)texto[pos_t + 1]];
    }
    free(dist_ult);
    return ocorrs;
}

int *preProcGoodSuff(char palavra[], int m)
{
    int i, q, desl_p;
    int *alcance = malloc((m + 1) * sizeof(int));
    for (i = m; i >= 1; i--)
    {
        q = m - 1;
        desl_p = 0;
        // continua enquanto desl_p for menor que
        // o tamanho do sufixo palavra[i .. m]
        // e do prefixo palavra[1 .. q]
        while (desl_p < m - i + 1 && desl_p < q)
            if (palavra[m - desl_p] == palavra[q - desl_p])
                desl_p++;
            else
                q--, desl_p = 0;
        alcance[i] = m - q;
    }
    return alcance;
}

// se o primeiro casamento do segmento palavra[i .. m]
// ocorreu com o prefixo palavra[1 .. q], o primeiro
// casamento de palavra[i-1 .. m] não pode ocorrer
// com um prefixo maior que palavra[1 .. q]
int *preProcGoodSuff2(char palavra[], int m)
{
    int i, q, desl_p;
    int *alcance = malloc((m + 1) * sizeof(int));
    q = m - 1;
    desl_p = 0;
    for (i = m; i >= 1; i--)
    {
        // continua enquanto desl_p for menor que
        // o tamanho do sufixo palavra[i .. m]
        // e do prefixo palavra[1 .. q]
        while (desl_p < m - i + 1 && desl_p < q)
            if (palavra[m - desl_p] == palavra[q - desl_p])
                desl_p++;
            else
                q--, desl_p = 0;
        alcance[i] = m - q;
    }
    return alcance;
}

int *preProcGoodSuff3(char palavra[], int m)
{
    int i, q, desl_p;
    int *alcance = malloc((m + 1) * sizeof(int));
    i = q = m;
    do
    {
        q -= 1;
        desl_p = 0;
        while (q > desl_p && palavra[m - desl_p] == palavra[q - desl_p])
            desl_p++;
        while (desl_p > m - i)
            alcance[i--] = m - q;
    } while (q > desl_p); // enquanto não for um prefixo que é sufixo
    while (i >= 1)
        alcance[i--] = m - q;
    return alcance;
}

// Recebe vetores palavra[1..m] e texto[1..n],
// com m >= 1 e n >= 0, e devolve o número de
// ocorrências de palavra em texto.
int BoyerMoore2(char palavra[], int m, char texto[], int n)
{
    int *alcance, pos_t, desl_p, ocorrs;
    // pré-processamento da palavra
    alcance = preProcGoodSuff3(palavra, m);
    // busca da palavra no texto
    ocorrs = 0;
    pos_t = m;
    while (pos_t <= n)
    {
        desl_p = 0;
        // palavra[1..m] casa com p[pos_t-m+1..pos_t]?
        // ordem desse laço é importante?
        while (desl_p < m && palavra[m - desl_p] == texto[pos_t - desl_p])
            desl_p++;
        if (desl_p >= m)
            ocorrs++;
        if (desl_p == 0)
            pos_t += 1;
        else
            pos_t += alcance[m - desl_p + 1]; // por que + 1?
    }
    free(alcance);
    return ocorrs;
}

// Recebe vetores palavra[1..m] e texto[1..n],
// com m >= 1 e n >= 0, e devolve o número de
// ocorrências de palavra em texto.
int BoyerMoore(char palavra[], int m, char texto[], int n)
{
    int *alcance, *dist_ult, pos_t, desl_p, ocorrs, incr1, incr2;
    dist_ult = preProcBadCharac(palavra, m);
    alcance = preProcGoodSuff(palavra, m);
    ocorrs = 0;
    pos_t = m;
    while (pos_t <= n)
    {
        desl_p = 0;
        while (desl_p < m && palavra[m - desl_p] == texto[pos_t - desl_p])
            desl_p++;
        if (desl_p >= m)
            ocorrs++;
        if (pos_t == n)
            incr1 = 1;
        else
            incr1 = 1 + dist_ult[(int)texto[pos_t + 1]];
        if (desl_p == 0)
            incr2 = 1;
        else
            incr2 = alcance[m - desl_p + 1];
        pos_t += maximo(incr1, incr2);
    }
    free(dist_ult);
    free(alcance);
    return ocorrs;
}

int main(int argc, char *argv[])
{
    char *p, *t;
    int m, n;

    if (argc != 3)
    {
        printf("Numero incorreto de argumentos. Ex.: .\\buscaPalavras \"TACTA\" \"GTAGTATATATACTACTAGTAG\" ");
        return 0;
    }
    m = strlen(argv[1]);
    n = strlen(argv[2]);
    p = (char *)malloc((m + 1) * sizeof(char));
    t = (char *)malloc((n + 1) * sizeof(char));
    strcpy(p + 1, argv[1]);
    strcpy(t + 1, argv[2]);

    printf("Numero de ocorrencias de p=%s em t=%s igual a %d.\n", p + 1, t + 1, BoyerMoore2(p, m, t, n));

    return 0;
}