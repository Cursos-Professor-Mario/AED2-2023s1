#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *String;
typedef String Chave;
typedef int Valor;

void stInit(int);
Valor stSearch(Chave);
void stInsert(Chave, Valor);
void stDelete(Chave);
void stFree();

void *mallocSafe(int nbytes)
{
    void *p;
    p = malloc(nbytes);
    if (p == NULL)
    {
        printf("ERRO! Malloc devolveu NULL\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

static String copyString(String string)
{
    String str = NULL;
    int len = strlen(string) + 1;
    str = mallocSafe(len * sizeof(char));
    strncpy(str, string, len);
    return str;
}

// implementação com hash table e listas encadeadas

typedef struct celTS CelTS;
struct celTS
{
    Chave chave;
    Valor valor;
    CelTS *prox;
};

static CelTS **tab = NULL;
static int nChaves = 0;
static int M; // tamanho da tabela

// int hash(Chave chave, int M)
// {
//     return chave % M;
// }

// int hash(Chave chave, int M)
// {
//     return (17 * chave + 43) % M;
// }

int hash(Chave chave, int M)
{
    int i, h = 0;
    for (i = 0; chave[i] != '\0'; i++)
        h += chave[i];
    h = h % M;
    return h;
}

int hash(Chave chave, int M)
{
    int i, h = 0;
    int primo = 127;
    for (i = 0; chave[i] != '\0'; i++)
        h += primo * chave[i];
    h = h % M;
    return h;
}

int hash(Chave chave, int M)
{
    int i, h = 0;
    int primo = 127;
    for (i = 0; chave[i] != '\0'; i++)
        h += pow(primo, i) * chave[i];
    h = h % M;
    return h;
}

int hash(Chave chave, int M)
{
    int i, h = 0;
    int primo = 127;
    for (i = 0; chave[i] != '\0'; i++)
        h = (h * primo + chave[i]);
    h = h % M;
    return h;
}

int hash(Chave chave, int M)
{
    int i, h = 0;
    int primo = 127;
    for (i = 0; chave[i] != '\0'; i++)
        h = (h * primo + chave[i]) % M;
    return h;
}

void stInit(int max)
{
    int h;
    M = max;
    nChaves = 0;
    tab = mallocSafe(M * sizeof(CelTS *));
    for (h = 0; h < M; h++)
        tab[h] = NULL;
}

Valor stSearch(Chave chave)
{
    CelTS *p;
    int h = hash(chave, M);
    p = tab[h];
    while (p != NULL && strcmp(p->chave, chave) != 0)
        p = p->prox;
    if (p != NULL) // se encontrou devolve o valor
        return p->valor;
    return 0; // caso contrário devolve 0. E se o valor for 0? Como contornar esse problema?
}

void stInsert(Chave chave, Valor valor) // inserção ou edição
{
    CelTS *p;
    int h = hash(chave, M);
    p = tab[h];
    while (p != NULL && strcmp(p->chave, chave))
        p = p->prox;
    if (p == NULL) // se não encontrou insere nova célula na lista
    {
        p = mallocSafe(sizeof(*p));
        p->chave = copyString(chave);
        nChaves += 1;
        p->prox = tab[h];
        tab[h] = p;
    }
    p->valor = valor; // atualiza valor do item
}

void stDelete(Chave chave)
{
    CelTS *p, *aux;
    int h = hash(chave, M);
    p = tab[h];
    if (p == NULL) // se lista está vazia não tem o que remover
        return;
    if (strcmp(p->chave, chave) == 0) // remoção do começo da lista
    {
        tab[h] = p->prox;
        free(p->chave);
        free(p);
        nChaves--;
        return;
    }
    // remoção no restante da lista
    while (p->prox != NULL && strcmp((p->prox)->chave, chave) != 0)
        p = p->prox;
    if (p->prox != NULL) // caso em que o próximo é o valor por remover
    {
        aux = p->prox;
        p->prox = aux->prox;
        free(aux->chave);
        free(aux);
        nChaves--;
    }
}

void stFree()
{
    CelTS *p = NULL, *q = NULL;
    int h;
    for (h = 0; h < M; h++) // libera cada lista
    {
        p = tab[h];
        while (p != NULL)
        {
            q = p;
            p = p->prox;
            free(q->chave); // liberando a chave (string) de cada célula
            free(q);        // antes de liberar a célula
        }
    }
    free(tab); // então libera a tabela
    tab = NULL;
    nChaves = 0;
}

int main()
{
    return 0;
}