#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafosMatrizAdj.h"

void buscaProfR(Grafo G, int v, int *ordem_chegada, int *ordem_saida, int *ptempo)
{
    int w;
    ordem_chegada[v] = (*ptempo)++;
    /* para cada vizinho de v que ainda não foi visitado */
    for (w = 0; w < G->n; w++)
        if (G->A[v][w] == 1 && ordem_chegada[w] == -1)
            buscaProfR(G, w, ordem_chegada, ordem_saida, ptempo);
    ordem_saida[v] = (*ptempo)++;
}

// busca em profundidade iterativa com pilha que não trata do tempo de fechamento
void buscaProfI0(Grafo G, int origem, int *ordem_chegada, int *ordem_saida, int tempo)
{
    int v, w;
    // pilha implementada em vetor
    int *pilha;
    int topo = 0;
    pilha = malloc(G->m * sizeof(int));
    /* colocando a origem na pilha */
    pilha[topo++] = origem;
    /* enquanto a pilha dos ativos (encontrados
    mas não visitados) não estiver vazia */
    while (topo > 0)
    {
        /* remova o mais recente da pilha */
        v = pilha[--topo];
        if (ordem_chegada[v] == -1) // se v nao foi visitado
        {
            ordem_chegada[v] = tempo++;
            /* para cada vizinho de v que ainda não foi visitado */
            for (w = 0; w < G->n; w++)
                if (G->A[v][w] == 1 && ordem_chegada[w] == -1)
                    pilha[topo++] = w; // empilha o vizinho
        }
    }
    free(pilha);
}

void buscaProfI(Grafo G, int origem, int *ordem_chegada, int *ordem_saida, int tempo)
{
    int v, w;
    // pilha implementada em vetor
    int *pilha;
    int topo = 0;
    pilha = malloc(G->m * sizeof(int));
    /* colocando a origem na pilha */
    pilha[topo++] = origem;
    /* enquanto a pilha dos ativos (encontrados
    mas não visitados) não estiver vazia */
    while (topo > 0)
    {
        /* remova o mais recente da pilha */
        v = pilha[--topo];
        if (ordem_chegada[v] == -1) // se v nao foi visitado
        {
            ordem_chegada[v] = tempo++;
            pilha[topo++] = v; // empilha o vértice v pra saber quando marcar o tempo de término
            /* para cada vizinho de v que ainda não foi visitado */
            for (w = 0; w < G->n; w++)
                if (G->A[v][w] == 1 && ordem_chegada[w] == -1)
                    pilha[topo++] = w; // empilha o vizinho
        }
        else if (ordem_saida[v] == -1)
            ordem_saida[v] = tempo++;
    }
    free(pilha);
}

void buscaProf(Grafo G, int origem, int *ordem_chegada, int *ordem_saida)
{
    int i, tempo;
    /* inicializa todos como não encontrados */
    for (i = 0; i < G->n; i++)
    {
        ordem_chegada[i] = -1;
        ordem_saida[i] = -1;
    }
    tempo = 1;
    buscaProfR(G, origem, ordem_chegada, ordem_saida, &tempo);
    // buscaProfI(G, origem, ordem_chegada, ordem_saida, tempo);
}

Grafo lerMatrizGrafo(FILE *entrada)
{
    int n, v, w, value;
    Grafo G;
    fscanf(entrada, "%d\n", &n);
    G = inicializaGrafo(n);
    for (v = 0; v < G->n; v++)
        for (w = 0; w < G->n; w++)
        {
            fscanf(entrada, "%d", &value);
            if (value == 1)
            {
                // G->A[v][w] = 1;
                // G->m++;
                insereArcoNaoSeguraGrafo(G, v, w);
            }
        }
    return G;
}

Grafo lerImprimeGrafo(FILE *entrada)
{
    int n, m, v, w;
    Grafo G;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = inicializaGrafo(n);
    for (v = 0; v < G->n; v++)
    {
        fscanf(entrada, "%d", &w);
        while (w != -1)
        {
            // G->A[v][w] = 1;
            // G->m++;
            insereArcoNaoSeguraGrafo(G, v, w);
            fscanf(entrada, "%d", &w);
        }
    }
    return G;
}

Grafo lerMostraGrafo(FILE *entrada)
{
    int n, m, v, w, tam;
    Grafo G;
    char *str, *aux;
    fscanf(entrada, "%d %d\n", &n, &m);
    G = inicializaGrafo(n);
    tam = ((G->n * ((int)log10((double)G->n) + 1)) + 3) * sizeof(char);
    str = malloc(tam);
    for (v = 0; v < G->n; v++)
    {
        fgets(str, tam, entrada);
        aux = strtok(str, ":");
        aux = strtok(NULL, " \n");
        while (aux != NULL)
        {
            w = atoi(aux);
            // G->A[v][w] = 1;
            // G->m++;
            insereArcoNaoSeguraGrafo(G, v, w);
            aux = strtok(NULL, " \n");
        }
    }
    free(str);
    return G;
}

int main(int argc, char *argv[])
{
    FILE *entrada;
    int i, *ordem_chegada, *ordem_saida;
    Grafo G;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\buscaProf \".\\instancias\\inst10_30print\"");
        return 0;
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        printf("Nao encontrei o arquivo!");
        exit(EXIT_FAILURE);
    }

    // G = lerMatrizGrafo(entrada);
    G = lerImprimeGrafo(entrada);
    // G = lerMostraGrafo(entrada);

    // imprimindo a rede como lista de adjacências
    mostraGrafo(G);

    ordem_chegada = malloc(G->n * sizeof(int));
    ordem_saida = malloc(G->n * sizeof(int));

    buscaProf(G, 0, ordem_chegada, ordem_saida);
    // buscaProf(G, n / 2, ordem_chegada, ordem_saida);

    printf("vertices:      ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", i);
    printf("\n");
    printf("ordem chegada: ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", ordem_chegada[i]);
    printf("\n");
    printf("ordem saida:   ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", ordem_saida[i]);
    printf("\n");

    G = liberaGrafo(G);
    free(ordem_chegada);
    free(ordem_saida);
    fclose(entrada);
    return 0;
}