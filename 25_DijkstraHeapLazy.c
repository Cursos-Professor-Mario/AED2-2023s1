#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafosListasAdj.h"

typedef struct elem
{
    int chave;
    int conteudo;
} Elem;

#define PAI(i) (i - 1) / 2
#define FILHO_ESQ(i) (2 * i + 1)
#define FILHO_DIR(i) (2 * i + 2)

void troca(Elem *a, Elem *b)
{
    Elem aux = *a;
    *a = *b;
    *b = aux;
}

// sobe o elemento em v[pos_elem_v] até restaurar a propriedade do heap
void sobeHeap(Elem v[], int pos_elem_v)
{
    int f = pos_elem_v;
    while (f > 0 && v[PAI(f)].chave > v[f].chave)
    {
        troca(&v[f], &v[PAI(f)]);
        f = PAI(f);
    }
}

int insereHeap(Elem v[], int m, Elem x)
{
    v[m] = x;
    sobeHeap(v, m);
    return m + 1;
}

// desce o elemento em v[pos_elem_v] até restaurar a propriedade do heap
void desceHeap(Elem v[], int m, int pos_elem_v)
{
    int p = pos_elem_v, f;
    while (FILHO_ESQ(p) < m && (v[FILHO_ESQ(p)].chave < v[p].chave || (FILHO_DIR(p) < m && v[FILHO_DIR(p)].chave < v[p].chave)))
    {
        f = FILHO_ESQ(p);
        if (FILHO_DIR(p) < m &&
            v[FILHO_DIR(p)].chave < v[f].chave)
            f = FILHO_DIR(p);
        troca(&v[p], &v[f]);
        p = f;
    }
}

int removeHeap(Elem v[], int m, Elem *px)
{
    *px = v[0];
    troca(&v[0], &v[m - 1]);
    desceHeap(v, m - 1, 0);
    return m - 1;
}

void DijkstraComHeapLazy(Grafo G, int origem, int *dist, int *pred)
{
    int i, *R;
    Elem *H, x;
    int v, w, custo, tam_H;
    Noh *p;
    // inicializando distancias e predecessores
    for (i = 0; i < G->n; i++)
    {
        dist[i] = __INT_MAX__;
        pred[i] = -1;
    }
    dist[origem] = 0;
    // criando um min heap
    H = malloc(G->n * G->n * sizeof(Elem)); // heap precisa de mais espaço nessa versao
    for (i = 0; i < G->n; i++)
    {
        H[i].chave = dist[i];
        H[i].conteudo = i;
    }
    troca(&H[0], &H[origem]);
    tam_H = G->n;
    // inicializando conjunto de vértices "resolvidos" R
    R = malloc(G->n * sizeof(int));
    for (i = 0; i < G->n; i++)
        R[i] = 0;
    while (tam_H > 0)
    {
        // buscando vértice v que minimiza dist[v]
        tam_H = removeHeap(H, tam_H, &x);
        v = x.conteudo;
        if (R[v] == 0)
        { // precisa manter um controle pra evitar revisitar vértices nessa versao
            R[v] = 1;
            // percorrendo lista com vizinhos de v
            p = G->A[v];
            while (p != NULL)
            {
                w = p->rotulo;
                custo = p->custo;
                // e atualizando as distancias e predecessores quando for o caso
                if (dist[w] > dist[v] + custo)
                {
                    dist[w] = dist[v] + custo;
                    pred[w] = v;
                    x.chave = dist[w];
                    x.conteudo = w;
                    tam_H = insereHeap(H, tam_H, x);
                }
                p = p->prox;
            }
        }
    }
    free(H);
}

Grafo readGraphPrint(FILE *entrada)
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
            insereArcoNaoSeguraGrafo(G, v, w);
            fscanf(entrada, "%d", &w);
        }
    }
    return G;
}

int main(int argc, char *argv[])
{
    FILE *entrada;
    int i, *dist, *pred;
    Grafo G;

    if (argc != 2)
    {
        printf("Numero incorreto de parametros! Ex.: .\\Dijkstra .\\instancia\\inst10_30print");
        return 0;
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        printf("Nao encontrei o arquivo!");
        exit(EXIT_FAILURE);
    }

    G = readGraphPrint(entrada);

    // imprimindo a rede como lista de adjacências
    mostraGrafo(G);

    dist = malloc(G->n * sizeof(int));
    pred = malloc(G->n * sizeof(int));

    DijkstraComHeapLazy(G, 0, dist, pred);

    printf("vertices:   ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", i);
    printf("\n");
    printf("distancias: ");
    for (i = 0; i < G->n; i++)
        printf("%2d ", dist[i]);
    printf("\n");

    G = liberaGrafo(G);
    free(dist);
    free(pred);
    fclose(entrada);
    return 0;
}