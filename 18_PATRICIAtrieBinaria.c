#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Item;
typedef int Chave;

#define bitsPalavra 32
#define bitsDigito 1
#define digitosPalavra (bitsPalavra / bitsDigito)
#define Base (1 << bitsDigito)

// const int bitsPalavra = 32;
// const int bitsDigito = 1;
// const int digitosPalavra = bitsPalavra / bitsDigito;
// const int Base = 1 << bitsDigito; // Base = 2^bitsDigito

int pegaDigito(int chave, int digito)
{
    return (int)((chave >> (bitsDigito * (digitosPalavra - 1 - digito))) & (Base - 1));
}

typedef struct noh
{
    Chave chave;
    Item conteudo;
    int digito;
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
void preOrdemHierarquico(Arvore r, int altura, char t_filho, int digito_ant)
{
    if (r->digito > digito_ant)
    {
        imprimeSimbolos(altura, t_filho);
        printf("digito = %d, chave = %d\n", r->digito, r->chave);
        preOrdemHierarquico(r->esq, altura + 1, '>', r->digito);
        preOrdemHierarquico(r->dir, altura + 1, '<', r->digito);
    }
}

// Percurso in-ordem
void inOrdem(Arvore r, int digito_ant)
{
    if (r->digito <= digito_ant) // eh "folha"
    {
        printf("%d ", r->chave);
        return;
    }
    inOrdem(r->esq, r->digito);
    inOrdem(r->dir, r->digito);
}

int altura(Arvore r, int digito)
{
    int hesq, hdir;
    if (r->digito <= digito) // eh "folha"
        return -1;
    hesq = altura(r->esq, r->digito);
    hdir = altura(r->dir, r->digito);
    if (hesq > hdir)
        return hesq + 1;
    return hdir + 1;
}

Arvore inicializa()
{
    Noh *raiz;
    raiz = (Noh *)malloc(sizeof(Noh));
    raiz->chave = __UINT32_MAX__; // chave proibida
    raiz->esq = raiz;
    raiz->dir = raiz;
    raiz->digito = -1;
    return raiz;
}

Noh *buscaR(Arvore r, Chave chave, int digito_ant)
{
    if (r->digito <= digito_ant) // eh uma "folha"
        return r;
    if (pegaDigito(chave, r->digito) == 0) // desce à esquerda
        return buscaR(r->esq, chave, r->digito);
    // pegaDigito(chave, r->digito) == 1 - desce à direita
    return buscaR(r->dir, chave, r->digito);
}

Noh *busca(Arvore r, Chave chave)
{
    Noh *alvo;
    alvo = buscaR(r->esq, chave, -1);
    return alvo->chave == chave ? alvo : NULL;
}

Noh *novoNoh(Chave chave, Item conteudo, int digito)
{
    Noh *novo;
    novo = (Noh *)malloc(sizeof(Noh));
    novo->chave = chave;
    novo->conteudo = conteudo;
    novo->digito = digito;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

Arvore insereR(Arvore r, Noh *novo, int digito_dif, Noh *pai)
{
    // se encontrei o ponto de quebra ou cheguei numa folha
    if ((r->digito >= digito_dif) || (r->digito <= pai->digito))
    {
        if (pegaDigito(novo->chave, digito_dif) == 1)
        {
            novo->esq = r;
            novo->dir = novo;
        }
        else //pegaDigito(novo->chave, digito_dif) == 0
        {
            novo->esq = novo;
            novo->dir = r;
        }
        return novo;
    }
    if (pegaDigito(novo->chave, r->digito) == 0)
    // inserir descendo à esquerda
    {
        r->esq = insereR(r->esq, novo, digito_dif, r);
    }
    else // pegaDigito(novo->chave, r->digito) == 1
    // inserir descendo à direita
    {
        r->dir = insereR(r->dir, novo, digito_dif, r);
    }
    return r;
}

void inserir(Arvore r, Chave chave, Item conteudo)
{
    int i;
    Noh *aux = buscaR(r->esq, chave, -1);
    if (aux->chave == chave)
        return; // não inserimos duplicatas
    for (i = 0; pegaDigito(chave, i) == pegaDigito(aux->chave, i); i++)
        ; // descobre qual o primeiro dígito diferente nas chaves
    Noh *novo = novoNoh(chave, conteudo, i);
    r->esq = insereR(r->esq, novo, i, r);
}

int main(int argc, char *argv[])
{
    int i, n;
    Arvore r = NULL;
    Noh *aux;
    int chave_aux;
    int chaves[7];

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: .\\PATRICIAtrieBinaria 10\n");
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

    r = inicializa();

    // inserção em ordem crescente
    for (i = 0; i < n; i++)
    {
        chave_aux = (-1) * i;
        inserir(r, chave_aux, chave_aux);
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
    //     inserir(r, chave_aux, chave_aux);
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
    //     inserir(r, chave_aux, chave_aux);
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

    preOrdemHierarquico(r->esq, 0, ' ', -1);
    inOrdem(r->esq, -1);
    printf("\n");

    printf("altura = %d\n", altura(r->esq, -1));

    for (i = 0; i < 7; i++)
    {
        aux = busca(r, chaves[i]);
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