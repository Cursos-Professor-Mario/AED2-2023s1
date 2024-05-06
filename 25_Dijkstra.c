#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grafosListasAdj.h"

typedef struct elem
{
    int chave;    // vamos guardar dist aqui
    int conteudo; // vamos guardar o vértice aqui
} Elem;

#define PAI(i) (i - 1) / 2
#define FILHO_ESQ(i) (2 * i + 1)
#define FILHO_DIR(i) (2 * i + 2)

// considerar fazer uma troca que já faz a troca a e a troca_pos,
// simplificando assim todas as funções do heap
void troca(Elem *a, Elem *b)
{
    Elem aux = *a;
    *a = *b;
    *b = aux;
}

void troca_pos(int *a, int *b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}

// sobe o elemento em v[pos_elem_v] até restaurar a propriedade do heap
void sobeHeap(Elem v[], int pos_v[], int pos_elem_v)
{
    int f = pos_elem_v;
    while (f > 0 && v[PAI(f)].chave > v[f].chave)
    {
        troca(&v[f], &v[PAI(f)]);
        troca_pos(&pos_v[v[f].conteudo], &pos_v[v[PAI(f)].conteudo]);
        f = PAI(f);
    }
}

int insereHeap(Elem v[], int pos_v[], int m, Elem x)
{
    v[m] = x;
    pos_v[x.conteudo] = m;
    sobeHeap(v, pos_v, m);
    return m + 1;
}

// desce o elemento em v[pos_elem_v] até restaurar a propriedade do heap
void desceHeap(Elem v[], int pos_v[], int m, int pos_elem_v)
{
    int p = pos_elem_v, f;
    while (FILHO_ESQ(p) < m && (v[FILHO_ESQ(p)].chave < v[p].chave || (FILHO_DIR(p) < m && v[FILHO_DIR(p)].chave < v[p].chave)))
    {
        f = FILHO_ESQ(p);
        if (FILHO_DIR(p) < m &&
            v[FILHO_DIR(p)].chave < v[f].chave)
            f = FILHO_DIR(p);
        troca(&v[p], &v[f]);
        troca_pos(&pos_v[v[p].conteudo], &pos_v[v[f].conteudo]);
        p = f;
    }
}

int removeHeap(Elem v[], int pos_v[], int m, Elem *px)
{
    *px = v[0];
    troca(&v[0], &v[m - 1]);
    troca_pos(&pos_v[v[0].conteudo], &pos_v[v[m - 1].conteudo]);
    desceHeap(v, pos_v, m - 1, 0);
    return m - 1;
}

void atualizaHeap(Elem v[], int pos_v[], Elem x)
{
    int pos_x_v = pos_v[x.conteudo]; // pega a posição de x em v
    v[pos_x_v].chave = x.chave;      // atualiza a chave de x em v
    sobeHeap(v, pos_v, pos_x_v);     // por que mando subir e não descer?
}

void DijkstraComHeap(Grafo G, int origem, int *dist, int *pred)
{
    int i, *pos_H;
    Elem *H, elem_aux;
    int v, w, custo, tam_H;
    Noh *p;
    // inicializando distancias e predecessores
    for (i = 0; i < G->n; i++)
    {
        dist[i] = __INT_MAX__;
        pred[i] = -1;
    }
    dist[origem] = 0;
    // criando um min heap em H com vetor de posições pos_H
    H = malloc(G->n * sizeof(Elem));
    pos_H = malloc(G->n * sizeof(int));
    for (i = 0; i < G->n; i++)
    {
        H[i].chave = dist[i]; // chave é a "distância" do vértice
        H[i].conteudo = i;    // conteudo é o rótulo do vértice
        pos_H[i] = i;         // vértice i começa na posição i do heap H
    }
    troca(&H[0], &H[origem]);             // coloca origem no início do heap H
    troca_pos(&pos_H[0], &pos_H[origem]); // atualiza posição dos vértices
    tam_H = G->n;
    // enquanto não encontrar o caminho mínimo para todo vértice
    while (tam_H > 0)
    {
        // buscando vértice v que minimiza dist[v]
        tam_H = removeHeap(H, pos_H, tam_H, &elem_aux);
        v = elem_aux.conteudo;
        // percorrendo lista com vizinhos de v
        p = G->A[v];
        while (p != NULL)
        {
            w = p->rotulo;
            custo = p->custo;
            // e atualizando as distâncias e predecessores quando for o caso
            if (dist[w] > dist[v] + custo)
            {
                dist[w] = dist[v] + custo;
                pred[w] = v;
                elem_aux.chave = dist[w];
                elem_aux.conteudo = w;
                atualizaHeap(H, pos_H, elem_aux);
            }
            p = p->prox;
        }
    }
    free(H);
    free(pos_H);
}

void Dijkstra(Grafo G, int origem, int *dist, int *pred)
{
    int i, *R;
    int v, w, custo, tam_R, min_dist;
    Noh *p;
    // inicializando distâncias e predecessores
    for (i = 0; i < G->n; i++)
    {
        dist[i] = __INT_MAX__;
        pred[i] = -1;
    }
    dist[origem] = 0;
    // inicializando conjunto de vértices "resolvidos" R
    R = malloc(G->n * sizeof(int));
    for (i = 0; i < G->n; i++)
        R[i] = 0;
    tam_R = 0;
    // enquanto não encontrar o caminho mínimo para todo vértice
    while (tam_R < G->n)
    {
        // buscando vértice v em V \ R que minimiza dist[v]
        min_dist = __INT_MAX__;
        v = -1;
        for (i = 0; i < G->n; i++)
            if (R[i] == 0 && dist[i] < min_dist)
            {
                v = i;
                min_dist = dist[i];
            }
        // adicionando v a R e atualizando o conjunto R
        R[v] = 1;
        tam_R++;
        // percorrendo lista com vizinhos de v
        p = G->A[v];
        while (p != NULL)
        {
            w = p->rotulo;
            custo = p->custo;
            // e atualizando as distâncias e predecessores quando for o caso
            if (R[w] == 0 && dist[w] > dist[v] + custo)
            {
                dist[w] = dist[v] + custo;
                pred[w] = v;
            }
            p = p->prox;
        }
    }
    free(R);
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

    Dijkstra(G, 0, dist, pred);

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