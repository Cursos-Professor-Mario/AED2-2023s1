typedef struct grafo *Grafo;
struct grafo
{
    int **A;
    int n; // número de vértices
    int m; // número de arestas/arcos
};

Grafo inicializaGrafo(int n);
void insereArcoGrafo(Grafo G, int v, int w);
void insereArcoNaoSeguraGrafo(Grafo G, int v, int w);
void removeArcoGrafo(Grafo G, int v, int w);
void mostraGrafo(Grafo G);
void imprimeGrafo(Grafo G);
Grafo liberaGrafo(Grafo G);
