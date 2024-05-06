typedef struct graph *Graph;
struct graph
{
    int **A;
    int n;
    int m;
};

Graph graphInit(int n);
void graphInsertArc(Graph G, int v, int w);
void graphInsertArcNotSafe(Graph G, int v, int w);
void graphRemoveArc(Graph G, int v, int w);
void graphShow(Graph G);
void graphPrint(Graph G);
Graph graphFree(Graph G);
