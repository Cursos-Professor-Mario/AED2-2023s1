#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Item;
typedef int Chave;

// #define bitsword 32
// #define bitsdigit 8
// #define digitsword (bitsword / bitsdigit)
// #define Base (1 << bitsdigit)

const int bitsword = 32;
const int bitsdigit = 1;
const int digitsword = bitsword / bitsdigit;
const int Base = 1 << bitsdigit; // Base = 2^bitsdigit

int digit(int a, int d)
{
    return (int)((a >> (bitsdigit * (digitsword - 1 - d))) & (Base - 1));
}

typedef struct noh
{
    Chave chave;
    Item conteudo;
    struct noh *esq;
    struct noh *dir;
} Noh;

typedef Noh *Arvore;

void imprimeSimbolos(int n, char c)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%c", c);
}

// Percurso pre-ordem destacando altura de cada subárvore
void preOrdemHierarquico(Arvore r, int altura, char t_filho)
{
    if (r != NULL)
    {
        imprimeSimbolos(altura, t_filho);
        printf("%d\n", r->chave);
        preOrdemHierarquico(r->esq, altura + 1, '>');
        preOrdemHierarquico(r->dir, altura + 1, '<');
    }
}

// Percurso in-ordem
void inOrdem(Arvore r)
{
    if (r != NULL)
    {
        inOrdem(r->esq);
        printf("%d ", r->chave);
        inOrdem(r->dir);
    }
}

int altura(Arvore r)
{
    int hesq, hdir;
    if (r == NULL)
        return -1;
    hesq = altura(r->esq);
    hdir = altura(r->dir);
    if (hesq > hdir)
        return hesq + 1;
    return hdir + 1;
}

Noh *buscaR(Arvore r, Chave chave, int d, Noh **pai)
{
    if (r == NULL)
        return r;
    if (r->chave == chave)
        return r;
    if (digit(chave, d) == 0)
    {
        *pai = r;
        return buscaR(r->esq, chave, d - 1, pai);
    }
    // digit(chave, d) == 1
    *pai = r;
    return buscaR(r->dir, chave, d - 1, pai);
}

Noh *novoNoh(Chave chave, Item conteudo)
{
    Noh *novo;
    novo = (Noh *)malloc(sizeof(Noh));
    novo->chave = chave;
    novo->conteudo = conteudo;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

Arvore insereR(Arvore r, Noh *novo, int d)
{
    if (r == NULL)
    {
        return novo;
    }
    if (digit(novo->chave, d) == 0)
    {
        r->esq = insereR(r->esq, novo, d - 1);
    }
    else // digit(novo->chave, d) == 1
    {
        r->dir = insereR(r->dir, novo, d - 1);
    }
    return r;
}

Arvore inserir(Arvore r, Chave chave, Item conteudo)
{
    Noh *novo = novoNoh(chave, conteudo);
    return insereR(r, novo, digitsword - 1);
}

Arvore removeRaiz(Arvore alvo)
{
    Noh *aux = NULL, *p = NULL;
    // se eh folha
    if (alvo->esq == NULL && alvo->dir == NULL)
    {
        free(alvo);
        return NULL;
    }
    // se nao eh folha
    aux = alvo;
    while (aux->dir != NULL || aux->esq != NULL)
    {
        p = aux;
        if (aux->dir != NULL)
            aux = aux->dir;
        else
            aux = aux->esq;
    }
    alvo->chave = aux->chave;
    alvo->conteudo = aux->conteudo;
    if (p->esq == aux)
        p->esq = removeRaiz(aux);
    else // p->dir == aux
        p->dir = removeRaiz(aux);
    return alvo;
}

Arvore remover(Arvore r, Chave chave)
{
    Noh *alvo, *aux, *p = NULL;
    alvo = buscaR(r, chave, digitsword - 1, &p);
    if (alvo == NULL)
    {
        // printf("Nao achou\n");
        return r;
    }
    aux = removeRaiz(alvo);
    if (alvo == r) // removeu a raiz da árvore
        return aux;
    if (p->esq == alvo)
        p->esq = aux;
    if (p->dir == alvo)
        p->dir = aux;
    return r;
}

int main(int argc, char *argv[])
{
    int i, n;
    Arvore r = NULL;
    Noh *aux, *pai;
    int chave_aux;
    int chaves[7];

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: arvoreDigitalBinariaLSD 10\n");
        return 0;
    }

    n = atoi(argv[1]);

    // for (int j = 1; j < n; j++)
    // {
    //     printf("%d\n", j);
    //     for (i = 0; i < digitsword; i++)
    //         printf("%d ", digit(j, i));
    //     printf("\n");
    // }
    // exit(1);

    // inserção em ordem crescente
    for (i = 0; i < n; i++)
    {
        chave_aux = i;
        r = inserir(r, chave_aux, chave_aux);
        if (i == 0)
            chaves[0] = chave_aux;
        else if (i == n / 2)
            chaves[1] = chave_aux;
        else if (i == n - 1)
            chaves[2] = chave_aux;
    }

    // inserção em ordem decrescente
    // for (i = 0; i < n; i++)
    // {
    //     chave_aux = n - 1 - i;
    //     r = inserir(r, chave_aux, chave_aux);
    //     if (i == 0)
    //         chaves[0] = chave_aux;
    //     else if (i == n / 2)
    //         chaves[1] = chave_aux;
    //     else if (i == n - 1)
    //         chaves[2] = chave_aux;
    // }

    // inserção aleatoria
    // for (i = 0; i < n; i++)
    // {
    //     chave_aux = rand();
    //     r = inserir(r, chave_aux, chave_aux);
    //     if (i == 0)
    //         chaves[0] = chave_aux;
    //     else if (i == n / 2)
    //         chaves[1] = chave_aux;
    //     else if (i == n - 1)
    //         chaves[2] = chave_aux;
    // }

    chaves[3] = 0;
    chaves[4] = n / 2;
    chaves[5] = n - 1;
    chaves[6] = n;

    preOrdemHierarquico(r, 0, ' ');
    inOrdem(r);
    printf("\n");

    printf("altura = %d\n", altura(r));

    for (i = 0; i < 7; i++)
    {
        aux = buscaR(r, chaves[i], digitsword - 1, &pai);
        if (aux != NULL)
            printf("chave de %d = %d\n", chaves[i], aux->chave);
        else
        {
            printf("nao encontrou %d\n", chaves[i]);
            aux = r;
        }
    }

    for (i = 0; i < n / 2; i++)
    {
        // printf("remover %d\n", 2 * i + 1);
        r = remover(r, 2 * i + 1);
    }

    preOrdemHierarquico(r, 0, ' ');
    inOrdem(r);
    printf("\n");

    printf("altura = %d\n", altura(r));

    for (i = 0; i < 7; i++)
    {
        aux = buscaR(r, chaves[i], digitsword - 1, &pai);
        if (aux != NULL)
            printf("chave de %d = %d\n", chaves[i], aux->chave);
        else
        {
            printf("nao encontrou %d\n", chaves[i]);
            aux = r;
        }
    }

    return 0;
}