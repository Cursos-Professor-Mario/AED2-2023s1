#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef int Item;
typedef char byte;
typedef byte *Chave;

// #define bitsDigito 8
// #define Base (1 << bitsDigito)

const int bitsDigito = 8;
const int Base = 1 << bitsDigito; // Base = 2^bitsDigito

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
        printf("%s\n", r->chave);
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
        while (i <= r->chave[d]) // problema com comprimento das chaves aqui
        {
            inOrdem(r->filhos[i], d + 1);
            i++;
        }
        printf("%s ", r->chave);
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
    if (strcmp(r->chave, chave) == 0)
        return r;
    *ppai = r;
    return buscaR(r->filhos[(int)chave[digito]], chave, digito + 1, ppai);
}

Noh *novoNoh(Chave chave, Item conteudo)
{
    int i;
    Noh *novo;
    novo = (Noh *)malloc(sizeof(Noh));
    novo->chave = (char *)malloc((strlen(chave) + 1) * sizeof(char));
    strcpy(novo->chave, chave);
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
    i = (int)(novo->chave[digito]);
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
        free(alvo->chave);
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
    strcpy(alvo->chave, aux->chave);
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
    int i, j, n, W;
    byte *string_aux;
    Arvore r = NULL;
    Noh *aux, *pai;
    int chave_aux;
    byte *chaves[7];

    if (argc != 3)
    {
        printf("Numero incorreto de parametros. Ex: arvoreDigitalString 10 5\n");
        return 0;
    }

    n = atoi(argv[1]);
    W = atoi(argv[2]);

    string_aux = (char *)malloc((W + 1) * sizeof(char));

    for (i = 0; i < 7; i++)
        chaves[i] = (byte *)malloc((W + 1) * sizeof(char));

    // inserção aleatoria
    for (i = 0; i < n; i++)
    {
        chave_aux = rand();
        for (j = 0; j < W; j++)
        {
            // string_aux[j] = chave_aux % Base;
            // string_aux[j] = 65 + ((rand() % (90 - 65 + 1)) % Base);
            string_aux[j] = 48 + ((rand() % (57 - 48 + 1)) % Base);
        }
        string_aux[j] = '\0';
        r = inserir(r, string_aux, chave_aux);
        if (i == 0)
            strcpy(chaves[0], string_aux);
        else if (i == n / 2)
            strcpy(chaves[1], string_aux);
        else if (i == n - 1)
            strcpy(chaves[2], string_aux);
    }

    strcpy(chaves[3], "0");

    sprintf(string_aux, "%d", n / 2);
    strcpy(chaves[4], string_aux);

    sprintf(string_aux, "%d", n - 1);
    strcpy(chaves[5], string_aux);

    sprintf(string_aux, "%d", n);
    strcpy(chaves[6], string_aux);

    preOrdemHierarquico(r, 0, ' ');
    inOrdem(r, 0);
    printf("\n");

    printf("altura = %d\n", altura(r));

    for (i = 0; i < 7; i++)
    {
        aux = buscaR(r, chaves[i], 0, &pai);
        if (aux != NULL)
            printf("chave de %s = %s\n", chaves[i], aux->chave);
        else
        {
            printf("nao encontrou %s\n", chaves[i]);
            aux = r;
        }
    }

    for (i = 0; i < 7; i++)
    {
        // printf("remover %s\n", chaves[i]);
        r = remover(r, chaves[i]);
    }

    preOrdemHierarquico(r, 0, ' ');
    inOrdem(r, 0);
    printf("\n");

    printf("altura = %d\n", altura(r));

    for (i = 0; i < 7; i++)
    {
        aux = buscaR(r, chaves[i], 0, &pai);
        if (aux != NULL)
            printf("chave de %s = %s\n", chaves[i], aux->chave);
        else
        {
            printf("nao encontrou %s\n", chaves[i]);
            aux = r;
        }
    }

    free(string_aux);

    for (i = 0; i < 7; i++)
        free(chaves[i]);

    return 0;
}