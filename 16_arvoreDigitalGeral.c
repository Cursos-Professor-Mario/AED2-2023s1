#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Item;
typedef int Chave;

// #define bitsword 32
// #define bitsdigit 8
// #define digitsword (bitsword / bitsdigit)
// #define Base (1 << bitsdigit)

const int bitsPalavra = 32;
const int bitsDigito = 8;
const int digitosPalavra = bitsPalavra / bitsDigito;
const int Base = 1 << bitsDigito; // Base = 2^bitsDigito

int pegaDigito(int chave, int digito)
{
    return (int)((chave >> (bitsDigito * (digitosPalavra - 1 - digito))) & (Base - 1));
}

typedef struct noh
{
    Chave chave;
    Item conteudo;
    struct noh **filhos;
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
    int i;
    if (r != NULL)
    {
        imprimeSimbolos(altura, t_filho);
        printf("%d\n", r->chave);
        for (i = 0; i < Base; i++)
            preOrdemHierarquico(r->filhos[i], altura + 1, ' ');
    }
}

// Percurso in-ordem
void inOrdem(Arvore r, int d)
{
    int i;
    if (r != NULL)
    {
        i = 0;
        while (i <= pegaDigito(r->chave, d))
        {
            inOrdem(r->filhos[i], d + 1);
            i++;
        }
        printf("%d ", r->chave);
        while (i < Base)
        {
            inOrdem(r->filhos[i], d + 1);
            i++;
        }
    }
}

int altura(Arvore r)
{
    int i, h, hmax;
    if (r == NULL)
        return -1;
    hmax = -1;
    for (i = 0; i < Base; i++)
    {
        h = altura(r->filhos[i]);
        if (h > hmax)
            hmax = h;
    }
    return hmax + 1;
}

Noh *buscaR(Arvore r, Chave chave, int digito, Noh **ppai)
{
    if (r == NULL)
        return r;
    if (r->chave == chave)
        return r;
    *ppai = r;
    return buscaR(r->filhos[pegaDigito(chave, digito)], chave, digito + 1, ppai);
}

Noh *novoNoh(Chave chave, Item conteudo)
{
    int i;
    Noh *novo;
    novo = (Noh *)malloc(sizeof(Noh));
    novo->chave = chave;
    novo->conteudo = conteudo;
    novo->filhos = malloc(Base * sizeof(Noh *));
    for (i = 0; i < Base; i++)
        novo->filhos[i] = NULL;
    return novo;
}

Arvore insereR(Arvore r, Noh *novo, int digito)
{
    int i;
    if (r == NULL)
    {
        return novo;
    }
    i = pegaDigito(novo->chave, digito);
    r->filhos[i] = insereR(r->filhos[i], novo, digito + 1);
    return r;
}

Arvore inserir(Arvore r, Chave chave, Item conteudo)
{
    Noh *novo = novoNoh(chave, conteudo);
    return insereR(r, novo, 0);
}

// remove alvo e devolve a nova raiz da subárvore
Arvore removeRaiz(Arvore alvo)
{
    Noh *aux = NULL, *pai = NULL;
    int i, iaux;
    for (i = 0; i < Base; i++)
        if (alvo->filhos[i] != NULL)
            break;
    // se eh folha
    if (i == Base)
    {
        free(alvo->filhos);
        free(alvo);
        return NULL;
    }
    // se nao eh folha
    aux = alvo;
    while (i < Base)
    {
        pai = aux;
        aux = pai->filhos[i];
        iaux = i;
        for (i = 0; i < Base; i++)
            if (aux->filhos[i] != NULL)
                break;
    }
    // aux chegou numa folha
    alvo->chave = aux->chave;
    alvo->conteudo = aux->conteudo;
    pai->filhos[iaux] = removeRaiz(aux);
    return alvo;
}

Arvore remover(Arvore r, Chave chave)
{
    Noh *alvo, *aux, *pai = NULL;
    int i;
    alvo = buscaR(r, chave, 0, &pai);
    if (alvo == NULL)
    {
        // printf("Nao achou\n");
        return r;
    }
    aux = removeRaiz(alvo);
    if (alvo == r) // removeu a raiz da árvore
        return aux;
    for (i = 0; i < Base; i++)
        if (pai->filhos[i] == alvo)
            break;
    pai->filhos[i] = aux;
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
        printf("Numero incorreto de parametros. Ex: arvoreDigitalGeral 10\n");
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
    inOrdem(r, 0);
    printf("\n");

    printf("altura = %d\n", altura(r));

    for (i = 0; i < 7; i++)
    {
        aux = buscaR(r, chaves[i], 0, &pai);
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
    inOrdem(r, 0);
    printf("\n");

    printf("altura = %d\n", altura(r));

    for (i = 0; i < 7; i++)
    {
        aux = buscaR(r, chaves[i], 0, &pai);
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