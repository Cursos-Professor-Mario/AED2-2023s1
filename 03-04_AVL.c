#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Cont;
typedef int Chave;

typedef struct noh
{
    int bal;
    Chave chave;
    Cont conteudo;
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

// Percurso pre-ordem destacando altura e balanceamento de cada subárvore
void preOrdemHierarquicoBal(Arvore r, int altura, char t_filho)
{
    if (r != NULL)
    {
        imprimeSimbolos(altura, t_filho);
        printf("%d \t\tbal=%d\n", r->chave, r->bal);
        preOrdemHierarquicoBal(r->esq, altura + 1, '>');
        preOrdemHierarquicoBal(r->dir, altura + 1, '<');
    }
}

// Percurso in-ordem
void inOrdemR(Arvore r)
{
    if (r != NULL)
    {
        inOrdemR(r->esq);
        printf("%d ", r->chave);
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

Noh *buscaR(Arvore r, Chave chave, Noh **ppai)
{
    if (r == NULL)
        return r;
    if (r->chave == chave)
        return r;
    *ppai = r;
    if (chave < r->chave)
        return buscaR(r->esq, chave, ppai);
    // r->chave > chave
    return buscaR(r->dir, chave, ppai);
}

Noh *buscaI(Arvore r, Chave chave, Noh **ppai)
{
    while (r != NULL && r->chave != chave)
    {
        *ppai = r;
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
    novo->bal = 0;
    novo->chave = chave;
    novo->conteudo = conteudo;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
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

Arvore insereAVL(Noh *r, Noh *novo, int *paumentou_altura)
{
    if (r == NULL) // Caso 1: subárvore era vazia
    {
        // printf("novo->chave = %d\n", novo->chave);
        *paumentou_altura = 1;
        return novo;
    }
    // printf("novo->chave = %d, r->chave = %d\n", novo->chave, r->chave);
    if (novo->chave <= r->chave) // desce à esquerda
    {
        // printf("desceu a esq de %d\n", r->chave);
        r->esq = insereAVL(r->esq, novo, paumentou_altura);
        if (*paumentou_altura == 1) // altura da subárvore esquerda aumentou após inserção
        {
            if (r->bal == +1) // inseriu do lado mais baixo
            {
                // printf("Caso 2: inseriu do lado mais baixo\n");
                r->bal = 0;
                *paumentou_altura = 0;
            }
            else if (r->bal == 0) // dois lados tinham a mesma altura
            {
                // printf("Caso 3: dois lados tinham a mesma altura\n");
                r->bal = -1;
                *paumentou_altura = 1;
            }
            else if (r->bal == -1) // inseriu do lado mais alto
            {
                // printf("inseriu do lado mais alto\n");
                if (r->esq->bal == -1) // Caso 4.1: inseriu à esquerda do filho esquerdo
                {
                    // printf("desceu esq: inseriu à esquerda do filho esquerdo\n");
                    // rotação simples a direita
                    r = rotacaoDir(r);
                    r->dir->bal = 0;
                }
                else // r->esq->bal == +1 - Caso 4.2: inseriu à direita do filho esquerdo
                {
                    // printf("desceu esq: inseriu à direita do filho esquerdo\n");
                    // rotação dupla esquerda-direita
                    r->esq = rotacaoEsq(r->esq);
                    r = rotacaoDir(r);
                    if (r->bal == 0)
                    {
                        r->esq->bal = 0;
                        r->dir->bal = 0;
                    }
                    else if (r->bal == -1)
                    {
                        r->esq->bal = 0;
                        r->dir->bal = +1;
                    }
                    else // r->bal == +1
                    {
                        r->esq->bal = -1;
                        r->dir->bal = 0;
                    }
                }
                r->bal = 0;
                *paumentou_altura = 0;
            }
        }
    }
    else // desce à direita - Quiz: faça essa parte do código
    {
    }
    return r;
}

Arvore inserir(Arvore r, Chave chave, Cont conteudo)
{
    int aumentou_altura;
    Noh *novo = novoNoh(chave, conteudo);
    return insereAVL(r, novo, &aumentou_altura);
    // return insereR(r, novo);
}

// recebe um nó r e um apontador para o pai de r em *ppai
// devolve um apontador para o nó com maior chave na árvore r
// e o pai desse nó em *ppai
Noh *maxI(Arvore r, Noh **ppai)
{
    while (r->dir != NULL)
    {
        *ppai = r;
        r = r->dir;
    }
    return r;
}

// recebe um nó r e um apontador para o pai de r em *ppai
// devolve um apontador para o nó com maior chave na árvore r
// e o pai desse nó em *ppai
Noh *maxR(Arvore r, Noh **ppai)
{
    if (r->dir != NULL)
    {
        *ppai = r;
        return maxR(r->dir, ppai);
    }
    return r;
}

Arvore removeRaiz(Arvore alvo)
{
    Noh *aux, *pai;
    if (alvo->esq == NULL && alvo->dir == NULL)
    {
        printf("Caso 1\n");
        free(alvo);

        return NULL;
    }
    if (alvo->esq == NULL || alvo->dir == NULL)
    {
        printf("Caso 2\n");
        if (alvo->esq == NULL)
            aux = alvo->dir;
        else // alvo->dir == NULL
            aux = alvo->esq;
        free(alvo);

        return aux;
    }
    printf("Caso 3\n");

    pai = alvo;
    aux = maxR(alvo->esq, &pai);
    printf("chave do predecessor do alvo = %d\n", aux->chave);
    alvo->chave = aux->chave;
    alvo->conteudo = aux->conteudo;
    if (pai == alvo) // aux == alvo->esq
        pai->esq = removeRaiz(aux);
    else // pai != alvo
        pai->dir = removeRaiz(aux);

    return alvo;
}

Arvore removeRaiz2(Arvore alvo)
{
    Noh *aux, *pai;
    if (alvo->esq == NULL && alvo->dir == NULL)
    {
        printf("Caso 1\n");
        free(alvo);

        return NULL;
    }
    if (alvo->esq == NULL || alvo->dir == NULL)
    {
        printf("Caso 2\n");
        if (alvo->esq == NULL)
            aux = alvo->dir;
        else // alvo->dir == NULL
            aux = alvo->esq;
        free(alvo);

        return aux;
    }
    printf("Caso 3\n");
    pai = alvo;
    aux = maxR(alvo->esq, &pai);
    printf("chave do predecessor do alvo = %d\n", aux->chave);
    if (pai != alvo)
    {
        pai->dir = aux->esq;
        aux->esq = alvo->esq;
        aux->dir = alvo->dir;
        free(alvo);
    }
    else // pai == alvo
    {
        aux->dir = alvo->dir;
        free(alvo);
    }

    return aux;
}

// recebe uma árvore r e uma chave "chave"
// remove nó com chave "chave" da árvore r
// devolve nova raiz da árvore
Arvore removeR(Arvore r, Chave chave, Noh **palvo)
{
    Noh *aux;

    if (*palvo == NULL) // ainda não encontrei o nó que desejo remover
    {
        if (r == NULL)
            return NULL; // a chave alvo não está na árvore

        if (chave == r->chave) // se encontrou a chave alvo nesse nível
        {
            *palvo = r;
            if (r->esq == NULL && r->dir == NULL) // se nó alvo é folha
            {
                printf("Caso 1\n");
                free(r);
                return NULL;
            }
            if (r->esq == NULL || r->dir == NULL) // se nó alvo tem um único filho
            {
                printf("Caso 2\n");
                if (r->esq == NULL)
                    aux = r->dir;
                else // r->dir == NULL
                    aux = r->esq;
                free(r);

                return aux;
            }
            // se nó alvo tem dois filhos
            printf("Caso 3\n");
            // remove nó alvo trocando ele com maior nó da subárvore esquerda
            // (*palvo)->esq = removeR(r->esq, -1, palvo); // ERRO PERIGOSO
            aux = removeR(r->esq, -1, palvo);
            r = *palvo;
            r->esq = aux;
            return r;
        }
        else // se não encontrou a chave alvo nesse nível desce na árvore
        {
            if (chave < r->chave) // à esquerda
                r->esq = removeR(r->esq, chave, palvo);
            else // r->chave > chave - ou à direita
                r->dir = removeR(r->dir, chave, palvo);
            return r;
        }
    }
    else // *palvo != NULL
    {    // encontrei o nó que desejo remover e estou procurando com quem trocá-lo
        if (r->dir != NULL)
        {
            r->dir = removeR(r->dir, -1, palvo);
            return r;
        }
        else
        {
            // encontrei o maior elemento da subárvore esquerda pra trocar com o alvo
            printf("chave do predecessor do alvo = %d\n", r->chave);
            if (r != (*palvo)->esq)
            {
                // // trocando filhos direitos
                // aux = r->dir;
                // r->dir = (*palvo)->dir;
                // (*palvo)->dir = aux;

                // // trocando filhos esquerdos
                // aux = r->esq;
                // r->esq = (*palvo)->esq;
                // (*palvo)->esq = aux;

                // // removendo o alvo
                // aux = (*palvo)->esq;
                // free(*palvo);
                // *palvo = r; // necessário na volta da recursão do caso 3
                // return aux;

                // versão compacta
                r->dir = (*palvo)->dir;
                aux = r->esq;
                r->esq = (*palvo)->esq;
                free(*palvo);
                *palvo = r; // necessário na volta da recursão do caso 3
                return aux;
            }
            else // r == (*palvo)->esq
            {
                // // trocando filhos direitos
                // aux = r->dir;
                // r->dir = (*palvo)->dir;
                // (*palvo)->dir = aux;

                // // trocando filhos esquerdos
                // aux = r->esq;
                // r->esq = *palvo;
                // (*palvo)->esq = aux;

                // // removendo o alvo
                // aux = (*palvo)->esq;
                // free(*palvo);
                // *palvo = r; // necessário na volta da recursão do caso 3
                // return aux;

                // versão compacta
                r->dir = (*palvo)->dir;
                aux = r->esq;
                free(*palvo);
                *palvo = r; // necessário na volta da recursão do caso 3
                return aux;
            }
            // return aux;
        }
    }
    // return r;
}

// recebe uma árvore r e uma chave "chave"
// remove nó com chave "chave" da árvore r
// devolve nova raiz da árvore
Arvore removeAVL(Arvore r, Chave chave, Noh **palvo, int *pdiminuiu_altura)
{
    Noh *aux;
    int desceu_esq;

    if (*palvo == NULL) // ainda não encontrei o nó que desejo remover
    {                   // então estou descendo fazendo busca por ele
        if (r == NULL)
            return NULL; // a chave alvo não está na árvore

        if (chave == r->chave) // se encontrou a chave alvo nesse nível
        {
            *palvo = r;
            if (r->esq == NULL && r->dir == NULL) // se nó alvo é folha
            {
                // printf("Caso 1 da remoção\n");
                free(r);
                *pdiminuiu_altura = 1;
                return NULL;
            }
            if (r->esq == NULL || r->dir == NULL) // se nó alvo tem um único filho
            {
                // printf("Caso 2 da remoção\n");
                if (r->esq == NULL)
                    aux = r->dir;
                else // r->dir == NULL
                    aux = r->esq;
                free(r);
                *pdiminuiu_altura = 1;
                return aux;
            }
            // se nó alvo tem dois filhos
            // printf("Caso 3 da remoção\n");
            // remove nó alvo trocando ele com maior nó da subárvore esquerda

            // (*palvo)->esq = removeR(r->esq, -1, palvo); // ERRO PERIGOSO
            // que nos conta algo sobre o processo de compilação

            aux = removeAVL(r->esq, -1, palvo, pdiminuiu_altura); // -1 indica que chave nao importa nessa chamada
            r = *palvo;
            // aux = removeAVL(r->esq, -1, &r, pdiminuiu_altura); // mais simples
            desceu_esq = 1;
            r->esq = aux;
            // return r;
        }
        else // se não encontrou a chave alvo nesse nível desce na árvore
        {
            if (chave < r->chave)
            { // à esquerda
                r->esq = removeAVL(r->esq, chave, palvo, pdiminuiu_altura);
                desceu_esq = 1;
            }
            else
            { // r->chave > chave - ou à direita
                r->dir = removeAVL(r->dir, chave, palvo, pdiminuiu_altura);
                desceu_esq = 0;
            }
            // return r;
        }
    }
    else // *palvo != NULL - encontrei o nó que desejo remover e estou procurando com quem trocá-lo
    {
        if (r->dir != NULL) // descendo à direita procurando o máximo da subárvore
        {
            r->dir = removeAVL(r->dir, -1, palvo, pdiminuiu_altura);
            desceu_esq = 0;
            // return r;
        }
        else // encontrei o maior elemento da subárvore esquerda pra trocar com o alvo
        {
            // printf("chave do predecessor do alvo = %d\n", r->chave);
            if (r != (*palvo)->esq) // r é filho direito de seu pai
            {
                // // trocando filhos direitos
                // aux = r->dir;
                // r->dir = (*palvo)->dir;
                // (*palvo)->dir = aux;

                // // trocando filhos esquerdos
                // aux = r->esq;
                // r->esq = (*palvo)->esq;
                // (*palvo)->esq = aux;

                // // removendo o alvo
                // r->bal = (*palvo)->bal;
                // aux = (*palvo)->esq;
                // free(*palvo);
                // *pdiminuiu_altura = 1;
                // *palvo = r; // necessário na volta da recursão do caso 3 da remoção
                // return aux;

                // Quiz: analisar simplificações que levaram à versão compacta da troca
                // versão compacta
                r->dir = (*palvo)->dir;
                aux = r->esq;
                r->esq = (*palvo)->esq;
                r->bal = (*palvo)->bal;
                free(*palvo);
                *pdiminuiu_altura = 1;
                *palvo = r; // necessário na volta da recursão do caso 3 da remoção
                return aux;
            }
            else // r == (*palvo)->esq - r é filho esquerdo de seu pai, que é o nó alvo
            {
                // // trocando filhos direitos
                // aux = r->dir;
                // r->dir = (*palvo)->dir;
                // (*palvo)->dir = aux;

                // // trocando filhos esquerdos
                // aux = r->esq;
                // r->esq = *palvo;
                // (*palvo)->esq = aux;

                // // removendo o alvo
                // r->bal = (*palvo)->bal;
                // aux = (*palvo)->esq;
                // free(*palvo);
                // *pdiminuiu_altura = 1;
                // *palvo = r; // necessário na volta da recursão do caso 3 da remoção
                // return aux;

                // Quiz: analisar simplificações que levaram à versão compacta da troca
                // versão compacta
                r->dir = (*palvo)->dir;
                aux = r->esq;
                r->bal = (*palvo)->bal;
                free(*palvo);
                *pdiminuiu_altura = 1;
                *palvo = r; // necessário na volta da recursão do caso 3 da remoção
                return aux;
            }
            // return aux e outras linhas comuns podiam vir pra cá
        }
    }

    if (*pdiminuiu_altura == 1)
    { // corrigir balanceamento em cada caso
        if (desceu_esq == 1)
        { // casos em que a remoção ocorreu à esquerda de r
            if (r->bal == 0)
            { // caso 2 do rebalanceamento - subárvore r estava balanceada
                r->bal = +1;
                *pdiminuiu_altura = 0;
                // raiz da subárvore não mudou
            }
            else if (r->bal == -1)
            { // caso 3 do rebalanceamento
                r->bal = 0;
                *pdiminuiu_altura = 1;
                // raiz da subárvore não mudou
            }
            else // r->bal == +1
            {    // caso 4 do rebalanceamento
                if (r->dir->bal == 0)
                { // caso 4.1 do rebalanceamento
                    r = rotacaoEsq(r);
                    // raiz da subárvore mudou
                    r->esq->bal = +1; // suplérfuo
                    r->bal = -1;
                    *pdiminuiu_altura = 0;
                }
                else if (r->dir->bal == +1)
                { // caso 4.2 do rebalanceamento
                    r = rotacaoEsq(r);
                    // raiz da subárvore mudou
                    r->esq->bal = 0;
                    r->bal = 0;
                    *pdiminuiu_altura = 1;
                }
                else // r->dir->bal == -1
                {    // caso 4.3 do rebalanceamento
                    r->dir = rotacaoDir(r->dir);
                    r = rotacaoEsq(r);
                    // raiz da subárvore mudou
                    if (r->bal == 0)
                    {
                        r->esq->bal = 0;
                        r->dir->bal = 0;
                    }
                    else if (r->bal == -1)
                    {
                        r->esq->bal = 0;
                        r->dir->bal = +1;
                    }
                    else // r->bal == +1
                    {
                        r->esq->bal = -1;
                        r->dir->bal = 0;
                    }
                    *pdiminuiu_altura = 1;
                }
            }
        }
        else // desceu_esq == 0 - Quiz: faça essa parte do código
        {    // casos em que a remoção ocorreu à direita de r
        }
    }
    return r;
}

Arvore remover(Arvore r, Chave chave)
{
    Noh *alvo = NULL;
    int diminuiu_altura;
    return removeAVL(r, chave, &alvo, &diminuiu_altura);
    // return removeR(r, chave, &alvo);
}

int main(int argc, char *argv[])
{
    int i, n;
    Arvore r = NULL;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros. Ex: AVL2 10\n");
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

    preOrdemHierarquicoBal(r, 0, ' ');

    inOrdem(r);

    printf("altura = %d\n", altura(r));

    // r = remover(r, 0);
    // r = remover(r, 1);
    // r = remover(r, 3);
    // r = remover(r, 2);
    // r = remover(r, 6);
    // r = remover(r, 7);

    int entrada;
    scanf("%d", &entrada);
    while (entrada != -1)
    {
        r = remover(r, entrada);

        preOrdemHierarquicoBal(r, 0, ' ');

        inOrdem(r);

        printf("altura = %d\n", altura(r));

        scanf("%d", &entrada);
    }

    return 0;
}