typedef int Chave;
typedef int Cont;

typedef struct noh
{
    Chave chave;
    Cont conteudo;
    int tam;         // opcional
    struct noh *pai; // opcional
    struct noh *esq;
    struct noh *dir;
} Noh;

typedef Noh TS;

Noh *TSbusca(TS *tab, Chave x);
Noh *TSmin(TS *tab);
Noh *TSmax(TS *tab);
Noh *TSpred(TS *tab, Chave x);
Noh *TSsuc(TS *tab, Chave x);
void TSperc(TS *tab);
Noh *TSselec(TS *tab, int i);
int TSrank(TS *tab, Chave x);
TS *TSinserir(TS *tab, Chave chave, Cont conteudo);
TS *TSremover(TS *tab, Chave chave);
