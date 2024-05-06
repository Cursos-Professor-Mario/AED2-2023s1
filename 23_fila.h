typedef struct fila Fila;

// typedef char type;
typedef int type;

Fila *criaFila(int tam);
void insereFila(Fila *q, type x);
type removeFila(Fila *q);
int filaVazia(Fila *q);
int filaCheia(Fila *q);
void imprimeFila(Fila *q);
int tamFila(Fila *q);
Fila *liberaFila(Fila *q);
