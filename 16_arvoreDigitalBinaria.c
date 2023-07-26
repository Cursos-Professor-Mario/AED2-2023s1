#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Item;
typedef int Chave;

// #define bitsPalavra 32
// #define bitsDigito 8
// #define digitosPalavra (bitsPalavra / bitsDigito)
// #define Base (1 << bitsDigito)

const int bitsPalavra = 32;
const int bitsDigito = 1;
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

Noh *buscaR(Arvore r, Chave chave, int digito, Noh **ppai)
{
    if (r == NULL)
        return r;
    if (r->chave == chave)
        return r;
    if (pegaDigito(chave, digito) == 0)
    {
        *ppai = r;
        return buscaR(r->esq, chave, digito + 1, ppai);
    }
    // else - pegaDigito(chave, digito) == 1
    *ppai = r;
    return buscaR(r->dir, chave, digito + 1, ppai);
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

Arvore insereR(Arvore r, Noh *novo, int digito)
{
    if (r == NULL)
    {
        return novo;
    }
    if (pegaDigito(novo->chave, digito) == 0)
        r->esq = insereR(r->esq, novo, digito + 1);
    else // pegaDigito(novo->chave, digito) == 1
        r->dir = insereR(r->dir, novo, digito + 1);
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
        pai = aux;
        if (aux->dir != NULL)
            aux = aux->dir;
        else
            aux = aux->esq;
    }
    // aux chegou numa folha
    alvo->chave = aux->chave;
    alvo->conteudo = aux->conteudo;
    if (pai->esq == aux)
        pai->esq = removeRaiz(aux);
    else // pai->dir == aux
        pai->dir = removeRaiz(aux);
    return alvo;
}

Arvore remover(Arvore r, Chave chave)
{
    Noh *alvo, *aux, *pai = NULL;
    alvo = buscaR(r, chave, 0, &pai);
    if (alvo == NULL)
    {
        // printf("Nao achou\n");
        return r;
    }
    aux = removeRaiz(alvo);
    if (alvo == r) // removeu a raiz da árvore
        return aux;
    if (pai->esq == alvo)
        pai->esq = aux;
    if (pai->dir == alvo)
        pai->dir = aux;
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
        printf("Numero incorreto de parametros. Ex: arvoreDigitalBinaria 10\n");
        return 0;
    }

    n = atoi(argv[1]);

    // for (int j = 1; j < n; j++)
    // {
    //     printf("%d\n", j);
    //     for (i = 0; i < digitosPalavra; i++)
    //         printf("%d ", pegaDigito(j, i));
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
    inOrdem(r);
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