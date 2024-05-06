typedef struct noh Noh;
struct noh
{
    int rotulo;
    int custo;
    Noh *prox;
};

typedef struct grafo *Grafo;
struct grafo
{
    Noh **A;
    int n; // número de vértices
    int m; // número de arestas/arcos
};

Grafo inicializaGrafo(int n);
void insereArcoGrafo(Grafo G, int v, int w);
void insereArcoNaoSeguraGrafo(Grafo G, int v, int w);
void mostraGrafo(Grafo G);
void imprimeGrafo(Grafo G);
void imprimeArquivoGrafo(Grafo G, FILE *saida);
Grafo liberaGrafo(Grafo G);
