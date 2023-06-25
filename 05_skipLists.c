#include <stdio.h>
#include <stdlib.h>

#define num_max_niveis 100

typedef int Chave;
typedef int Item;

typedef struct noh
{
    Chave chave;
    Item conteudo;
    struct noh **prox;
    int num_niveis;
} Noh;

static Noh *lista;
static int num_itens, nivel_max;

Noh *buscaR(Noh *t, Chave chave, int nivel)
{
    if (t != lista && chave == t->chave)
        return t;
    if (t->prox[nivel] == NULL || chave < t->prox[nivel]->chave)
    {
        if (nivel == 0)
            return NULL;
        return buscaR(t, chave, nivel - 1);
    }
    return buscaR(t->prox[nivel], chave, nivel);
}

Noh *TSbusca(Chave chave)
{
    return buscaR(lista, chave, nivel_max);
}

Noh *novo(Chave chave, Item conteudo, int num_niveis)
{
    int i;
    Noh *p = (Noh *)malloc(sizeof *p);
    p->chave = chave;
    p->conteudo = conteudo;
    p->prox = malloc(num_niveis * sizeof(Noh *));
    p->num_niveis = num_niveis;
    for (i = 0; i < num_niveis; i++)
        p->prox[i] = NULL;
    return p;
}

void TSinicializa()
{
    Noh *cabeca;
    num_itens = 0;
    nivel_max = 0;
    cabeca = novo(-1, -1, num_max_niveis);
    lista = cabeca;
}

int nivelAleatorio()
{
    int nivel, disp_acum, d = 2, v = rand();
    disp_acum = d;
    for (nivel = 0; nivel < num_max_niveis; nivel++)
    {
        if (v > RAND_MAX / disp_acum)
            break;
        disp_acum *= d;
    }
    if (nivel > nivel_max)
        nivel_max = nivel;
    return nivel;
}

void insereR(Noh *t, Noh *novoNoh, int nivel)
{
    Chave chave = novoNoh->chave;
    if (t->prox[nivel] == NULL || chave < t->prox[nivel]->chave)
    {
        if (nivel < novoNoh->num_niveis)
        {
            novoNoh->prox[nivel] = t->prox[nivel];
            t->prox[nivel] = novoNoh;
        }
        if (nivel > 0)
            insereR(t, novoNoh, nivel - 1);
        return;
    }
    insereR(t->prox[nivel], novoNoh, nivel);
}

void TSinsere(Chave chave, Item conteudo)
{
    int nivelAleat = nivelAleatorio();
    Noh *novoNoh = novo(chave, conteudo, nivelAleat + 1);
    insereR(lista, novoNoh, nivel_max);
    num_itens++;
}

int removeR(Noh *t, Chave chave, int nivel)
{
    Noh *p = t->prox[nivel];
    if (p == NULL || chave <= p->chave)
    {
        if (p != NULL && chave == p->chave)
        {
            t->prox[nivel] = p->prox[nivel];
            if (nivel == 0)
            {
                free(p->prox);
                free(p);
                return 1;
            }
        }
        if (nivel == 0)
            return 0;
        return removeR(t, chave, nivel - 1);
    }
    return removeR(t->prox[nivel], chave, nivel);
}

void TSremove(Chave chave)
{
    if (removeR(lista, chave, nivel_max))
        num_itens--;
}

void imprimeListaHier()
{
    Noh *t;
    int nivel;
    for (nivel = num_max_niveis - 1; nivel >= 0; nivel--)
    {
        if (lista->prox[nivel] != NULL)
        {
            t = lista->prox[nivel];
            while (t != NULL)
            {
                printf("%d ", t->chave);
                t = t->prox[nivel];
            }
            printf("\n");
        }
    }
}

void imprimeLista()
{
    Noh *t;
    t = lista->prox[0];
    while (t != NULL)
    {
        printf("%d ", t->chave);
        t = t->prox[0];
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int i, n;
    Noh *p;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex.: skipLists 100\n");
        return 0;
    }
    n = atoi(argv[1]);

    TSinicializa();
    printf("Numero de itens = %d\n", num_itens);

    // ordem direta
    for (i = 0; i < n; i++)
    {
        TSinsere(i, i);
        // TSinsere(1, 1);
    }
    imprimeLista();
    // imprimeListaHier();
    printf("Numero de itens = %d\n", num_itens);

    // ordem inversa
    for (i = n - 1; i >= 0; i--)
    {
        TSinsere(i, i);
        // TSinsere(1, 1);
    }
    // imprimeListaHier();
    imprimeLista();
    printf("Numero de itens = %d\n", num_itens);

    p = TSbusca(-1);
    if (p == NULL)
        printf("nao encontrou\n");
    else
        printf("%d\n", p->chave);

    p = TSbusca(n / 2);
    if (p == NULL)
        printf("nao encontrou\n");
    else
        printf("%d\n", p->chave);

    p = TSbusca(n);
    if (p == NULL)
        printf("nao encontrou\n");
    else
        printf("%d\n", p->chave);

    for (i = n - 1; i >= 0; i -= 2)
    {
        TSremove(i);
        // TSremove(1);
    }
    // imprimeLista();
    imprimeListaHier();
    printf("Numero de itens = %d\n", num_itens);
    for (i = 0; i < n; i += 2)
    {
        TSremove(i);
        // TSremove(1);
    }

    // imprimeLista();
    imprimeListaHier();
    printf("Numero de itens = %d\n", num_itens);

    return 0;
}
