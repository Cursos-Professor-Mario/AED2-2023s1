#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Item;
typedef int Chave;

typedef struct noh
{
    int vermelho;
    Chave chave;
    Item conteudo;
    struct noh *pai;
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
    int i;
    if (r != NULL)
    {
        imprimeSimbolos(altura, t_filho);
        printf("%d\n", r->chave);
        preOrdemHierarquico(r->esq, altura + 1, '>');
        preOrdemHierarquico(r->dir, altura + 1, '<');
    }
}

// Percurso in-ordem
void inOrdemR(Arvore r)
{
    if (r != NULL)
    {
        inOrdemR(r->esq);
        printf("%d ", r->chave, r->vermelho);
        inOrdemR(r->dir);
    }
}

void inOrdem(Arvore r)
{
    inOrdemR(r);
    printf("\n");
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

Noh *buscaR(Arvore r, Chave chave)
{
    if (r == NULL)
        return r;
    if (r->chave == chave)
        return r;
    if (chave < r->chave)
        return buscaR(r->esq, chave);
    // r->chave > chave
    return buscaR(r->dir, chave);
}

Noh *buscaI(Arvore r, Chave chave)
{
    while (r != NULL && r->chave != chave)
    {
        if (chave < r->chave)
            r = r->esq;
        else
            r = r->dir;
    }
    return r;
}

Noh *novoNoh(Chave chave, Item conteudo)
{
    Noh *novo;
    novo = (Noh *)malloc(sizeof(Noh));
    novo->vermelho = 1;
    novo->chave = chave;
    novo->conteudo = conteudo;
    novo->esq = NULL;
    novo->dir = NULL;
    //    novo->pai = ??
    return novo;
}

Arvore insereR(Arvore r, Noh *novo)
{
    if (r == NULL)
    {
        novo->pai = NULL;
        return novo;
    }
    if (novo->chave <= r->chave)
    {
        r->esq = insereR(r->esq, novo);
        r->esq->pai = r;
    }
    else
    {
        r->dir = insereR(r->dir, novo);
        r->dir->pai = r;
    }
    return r;
}

Arvore insereI(Arvore r, Noh *novo)
{
    Noh *corr, *ant = NULL;
    if (r == NULL)
    {
        novo->pai = NULL;
        return novo;
    }
    corr = r;
    while (corr != NULL)
    {
        ant = corr;
        if (novo->chave <= corr->chave)
            corr = corr->esq;
        else
            corr = corr->dir;
    }
    novo->pai = ant;
    if (novo->chave <= ant->chave)
        ant->esq = novo;
    else
        ant->dir = novo;
    return r;
}

Arvore rotacaoDir(Arvore r)
{
    Noh *aux;
    aux = r->esq;
    r->esq = aux->dir;
    if (aux->dir != NULL)
        aux->dir->pai = r;
    aux->dir = r;
    // aux->pai = r->pai // não é necessário, mas mais elegante corrigir aqui do que na inserção/remoção
    r->pai = aux;
    return aux;
}

Arvore rotacaoEsq(Arvore r)
{
    Noh *aux;
    aux = r->dir;
    r->dir = aux->esq;
    if (aux->esq != NULL)
        aux->esq->pai = r;
    aux->esq = r;
    // aux->pai = r->pai // não é necessário, mas mais elegante corrigir aqui do que na inserção/remoção
    r->pai = aux;
    return aux;
}

Arvore insereRN(Noh *r, Noh *novo)
{
    if (r == NULL) // subárvore era vazia
    {
        // printf("novo->chave = %d\n", novo->chave);
        novo->pai = NULL;
        return novo;
    }
    // printf("novo->chave = %d, r->chave = %d\n", novo->chave, r->chave);
    if (novo->chave <= r->chave) // desce à esquerda
    {
        // printf("desceu a esq de %d\n", r->chave);
        r->esq = insereRN(r->esq, novo);
        r->esq->pai = r;
    }
    else // desce à direita
    {
        // printf("desceu a dir de %d\n", r->chave);
        r->dir = insereRN(r->dir, novo);
        r->dir->pai = r;
    }
    if (r->dir != NULL && r->dir->vermelho == 1 && (r->esq == NULL || r->esq->vermelho == 0))
    {
        r = rotacaoEsq(r);
        r->vermelho = r->esq->vermelho;
        r->esq->vermelho = 1;
    }
    if (r->esq != NULL && r->esq->vermelho == 1 && r->esq->esq != NULL && r->esq->esq->vermelho == 1)
    {
        // se filho e neto esquerdos forem vermelhos faz rotação a direita e recolore de acordo
        r = rotacaoDir(r);
        r->vermelho = 0;
        r->dir->vermelho = 1;
    }
    if (r->esq != NULL && r->esq->vermelho == 1 && r->dir != NULL && r->dir->vermelho == 1)
    {
        // se os dois filhos são vermelhos, troque a cor com o pai preto
        r->esq->vermelho = 0;
        r->dir->vermelho = 0;
        r->vermelho = 1;
    }
    return r;
}

Arvore inserir(Arvore r, Chave chave, Item conteudo)
{
    Noh *novo = novoNoh(chave, conteudo);
    return insereRN(r, novo);
    // return insereR(r, novo);
}

int main(int argc, char *argv[])
{
    int i, j, n;
    Arvore r = NULL;
    Noh *aux;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: AVL 10\n");
        return 0;
    }

    n = atoi(argv[1]);

    // inserção em ordem crescente
    for (i = 0; i < n; i++)
        r = inserir(r, i, i);

    // inserção em ordem decrescente
    // for (i = n - 1; i >= 0; i--)
    //     r = inserir(r, i, i);

    // inserção pares depois ímpares
    // for (i = 0; i < n / 2; i++)
    //     r = inserir(r, 2 * i, 2 * i);
    // for (i = 0; i < n / 2; i++)
    //     r = inserir(r, 2 * i + 1, 2 * i + 1);

    // inserção alternada
    // for (i = 0; i < n; i++)
    //     r = inserir(r, i * pow(-1, i % 2), i * pow(-1, i % 2));

    preOrdemHierarquico(r, 0, ' ');

    inOrdem(r);

    printf("altura = %d\n", altura(r));

    return 0;
}