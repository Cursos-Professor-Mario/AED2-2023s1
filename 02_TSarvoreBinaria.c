#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TS.h"

typedef Noh *Arvore;

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
void inOrdemR(Arvore r)
{
    if (r != NULL)
    {
        inOrdemR(r->esq);
        printf("(%d, %d) ", r->chave, r->tam);
        inOrdemR(r->dir);
    }
}

void inOrdemI(Arvore r)
{
    Noh *x;
    Noh *p[100];
    int t = 0; // inicializa a pilha
    x = r;     // começa pela raiz
    // enquanto nó corrente não for nulo
    // e ainda houverem nós por imprimir na pilha
    while (x != NULL || t > 0)
    {
        if (x != NULL)
        {
            p[t++] = x; // empilha x
            x = x->esq; // visita filho esquerdo de x
        }
        else
        {
            x = p[--t]; // desempilha x
            printf("(%d, %d) ", x->chave, x->tam);
            x = x->dir; // visita o filho direito de x
        }
    }
}

// Percurso pré-ordem
void preOrdem(Arvore r)
{
    if (r != NULL)
    {
        printf("%d\n", r->chave);
        preOrdem(r->esq);
        preOrdem(r->dir);
    }
}

// Percurso pós-ordem
void posOrdem(Arvore r)
{
    if (r != NULL)
    {
        posOrdem(r->esq);
        posOrdem(r->dir);
        printf("%d\n", r->chave);
    }
}

void TSperc(TS *tab)
{
    inOrdemR(tab);
    printf("\n");
}

Noh *TSmin(Arvore r)
{
    while (r->esq != NULL)
        r = r->esq;
    return r;
}

Noh *TSmax(Arvore r)
{
    while (r->dir != NULL)
        r = r->dir;
    return r;
}

Noh *TSsuc(TS *tab, Chave x)
{
    Noh *q, *p;
    q = TSbusca(tab, x);
    if (q == NULL)
        return NULL;
    if (q->dir != NULL)
        return TSmin(q->dir);
    p = q->pai;
    while (p != NULL && p->dir == q)
    {
        q = p;
        p = p->pai;
    }
    return p;
}

Noh *TSpred(TS *tab, Chave x)
{
    Noh *q, *p;
    q = TSbusca(tab, x);
    if (q == NULL)
        return NULL;
    if (q->esq != NULL)
        return TSmax(q->esq);
    p = q->pai;
    while (p != NULL && p->esq == q)
    {
        q = p;
        p = p->pai;
    }
    return p;
}

Noh *TSpredSemPai(TS *tab, Chave x)
{
    Noh *q, *p = NULL;
    q = tab;
    while (q != NULL && q->chave != x)
    {
        if (x < q->chave)
            q = q->esq;
        else
        {
            p = q; // guarda o antepassado mais recente que é menor
            q = q->dir;
        }
    }
    if (q == NULL)
        return NULL;
    if (q->esq != NULL)
        return TSmax(q->esq);
    return p;
}

Noh *TSbuscaR(Arvore r, Chave chave)
{
    if (r == NULL)
        return r;
    if (r->chave == chave)
        return r;
    if (chave < r->chave)
        return TSbuscaR(r->esq, chave);
    // r->chave < chave
    return TSbuscaR(r->dir, chave);
}

Noh *TSbusca(Arvore r, Chave chave)
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

Noh *novoNoh(Chave chave, Cont conteudo)
{
    Noh *novo;
    novo = (Noh *)malloc(sizeof(Noh));
    novo->chave = chave;
    novo->conteudo = conteudo;
    novo->tam = 1;
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
    r->tam++;
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
        corr->tam++;
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

TS *TSinserir(TS *tab, Chave chave, Cont conteudo)
{
    Noh *novo = novoNoh(chave, conteudo);
    return insereR(tab, novo);
    // return insereI(tab, novo);
}

Arvore removeRaiz(Arvore alvo)
{
    Noh *aux, *p;
    if (alvo->esq == NULL && alvo->dir == NULL)
    {
        // printf("Caso 1\n");
        p = alvo->pai;
        free(alvo);

        while (p != NULL)
        {
            p->tam--;
            p = p->pai;
        }

        return NULL;
    }
    if (alvo->esq == NULL || alvo->dir == NULL)
    {
        // printf("Caso 2\n");
        if (alvo->esq == NULL)
            aux = alvo->dir;
        else // alvo->dir == NULL
            aux = alvo->esq;
        aux->pai = alvo->pai;
        p = alvo->pai;
        free(alvo);

        while (p != NULL)
        {
            p->tam--;
            p = p->pai;
        }

        return aux;
    }
    // printf("Caso 3\n");
    aux = TSmax(alvo->esq);
    printf("chave do predecessor do alvo = %d\n", aux->chave);
    alvo->chave = aux->chave;
    alvo->conteudo = aux->conteudo;
    p = aux->pai;
    if (p == alvo)
        p->esq = removeRaiz(aux);
    else // aux->pai != alvo
        p->dir = removeRaiz(aux);

    while (p != NULL)
    {
        p->tam--;
        p = p->pai;
    }

    return alvo;
}

Arvore removeRaiz2(Arvore alvo)
{
    Noh *aux, *p;
    if (alvo->esq == NULL && alvo->dir == NULL)
    {
        // printf("Caso 1\n");
        p = alvo->pai;
        free(alvo);

        while (p != NULL)
        {
            p->tam--;
            p = p->pai;
        }

        return NULL;
    }
    if (alvo->esq == NULL || alvo->dir == NULL)
    {
        // printf("Caso 2\n");
        if (alvo->esq == NULL)
            aux = alvo->dir;
        else // alvo->dir == NULL
            aux = alvo->esq;
        aux->pai = alvo->pai;

        p = alvo->pai;

        free(alvo);

        while (p != NULL)
        {
            p->tam--;
            p = p->pai;
        }

        return aux;
    }
    // printf("Caso 3\n");
    aux = TSmax(alvo->esq);
    printf("chave do predecessor do alvo = %d\n", aux->chave);
    p = aux->pai;
    if (p != alvo)
    {
        p->dir = aux->esq;
        aux->esq = alvo->esq;
        aux->dir = alvo->dir;
        aux->tam = aux->esq->tam + aux->dir->tam + 1;
        free(alvo);
    }
    else // p == alvo
    {
        aux->dir = alvo->dir;
        aux->tam = 1 + aux->dir->tam + 1;
        p = aux;
        free(alvo);
    }

    while (p != NULL)
    {
        p->tam--;
        p = p->pai;
    }

    return aux;
}

TS *TSremover(TS *tab, Chave chave)
{
    Noh *alvo, *p, *aux;
    alvo = TSbusca(tab, chave);
    // if (alvo != NULL)
    //     printf("chave do alvo = %d\n", alvo->chave);
    // else
    //     printf("nao encontrou a chave alvo\n");
    if (alvo == NULL)
        return tab;
    p = alvo->pai;
    aux = removeRaiz(alvo);
    if (p == NULL)
        return aux;
    if (p->esq == alvo)
        p->esq = aux;
    if (p->dir == alvo)
        p->dir = aux;
    return tab;
}

Noh *TSselec(Arvore r, int i)
{
    int t_esq;
    if (r == NULL)
        return NULL;
    if (r->esq != NULL)
        t_esq = r->esq->tam;
    else
        t_esq = 0;
    if (i == t_esq + 1)
        return r;
    if (i < t_esq + 1)
        return TSselec(r->esq, i);
    // i > t_esq + 1
    return TSselec(r->dir, i - t_esq - 1);
}

int TSrank(Arvore r, Chave chave)
{
    int tam = 0, t_esq;
    while (r != NULL && r->chave != chave)
    {
        if (chave < r->chave)
            r = r->esq;
        else
        {
            if (r->esq != NULL)
                t_esq = r->esq->tam;
            else
                t_esq = 0;
            tam += t_esq + 1;
            r = r->dir;
        }
    }
    if (r != NULL)
    {
        if (r->esq != NULL)
            tam += r->esq->tam;
        tam++;
    }
    return tam;
}

Arvore rotacaoDir(Arvore r)
{
    Noh *aux;
    aux = r->esq;
    r->esq = aux->dir;
    aux->dir = r;
    return aux;
}

Arvore rotacaoEsq(Arvore r)
{
    Noh *aux;
    aux = r->dir;
    r->dir = aux->esq;
    aux->esq = r;
    return aux;
}

// int main(int argc, char *argv[])
// {
//     int i, n;
//     Arvore r = NULL;
//     Noh *aux;

//     if (argc != 2)
//     {
//         printf("Numero incorreto de parametros. Ex: arvoreBinaria 10\n");
//         return 0;
//     }

//     n = atoi(argv[1]);

//     // inserção em ordem crescente
//     for (i = 0; i < n; i++)
// r = TSinserir(r, i, i);

//     // inserção em ordem decrescente
//     // for (i = n - 1; i >= 0; i--)
//     //     r = TSinserir(r, i, i);

//     // inserção alternada
//     // for (i = 0; i < n / 2; i++)
//     //     r = TSinserir(r, 2 * i, 2 * i);
//     // for (i = 0; i < n / 2; i++)
//     //     r = TSinserir(r, 2 * i + 1, 2 * i + 1);

//     // inserção alternando positivos e negativos
//     // for (i = 0; i < n; i++)
//     // {
//     //     r = TSinserir(r, i * pow(-1, i % 2), i * pow(-1, i % 2));
//     //     TSperc(r);
//     // }

//     TSperc(r);
//     preOrdemHierarquico(r, 0, ' ');

//     aux = TSmin(r);
//     printf("min = %d\n", aux->chave);

//     aux = TSmax(r);
//     printf("max = %d\n", aux->chave);

//     printf("altura = %d\n", altura(r));

//     aux = TSbusca(r, n / 2);
//     if (aux != NULL)
//         printf("chave de n/2 = %d\n", aux->chave);
//     else
//     {
//         printf("nao encontrou n/2\n");
//         aux = r;
//     }
//     aux = TSpred(r, n / 2);
//     if (aux != NULL)
//         printf("chave do predecessor de n / 2 = % d\n ", aux->chave);
//     else
//         printf("n/2 nao tem predecessor\n ");
//     aux = TSbusca(r, n / 2);
//     if (aux != NULL)
//         printf("chave de n/2 = %d\n", aux->chave);
//     else
//     {
//         printf("nao encontrou n/2\n");
//         aux = r;
//     }
//     aux = TSsuc(r, n / 2);
//     if (aux != NULL)
//         printf("chave do sucessor de n/2 = %d\n", aux->chave);
//     else
//         printf("n/2 nao tem sucessor\n");

//     TSperc(r);

// r = TSremover(r, n / 2);
//     TSperc(r);

//     for (i = 0; i < n; i += 3)
//     {
//         r = TSremover(r, i);
//         TSperc(r);
//     }

//     preOrdemHierarquico(r, 0, ' ');
//     TSperc(r);

//     i = 1;
//     aux = TSselec(r, i);
//     printf("chave do %d-esimo = %d\n", i, aux->chave);
//     printf("rank do %d-esimo = %d\n", i, TSrank(r, aux->chave));

//     i = 5;
//     aux = TSselec(r, i);
//     printf("chave do %d-esimo = %d\n", i, aux->chave);
//     printf("rank do %d-esimo = %d\n", i, TSrank(r, aux->chave));

//     return 0;
// }