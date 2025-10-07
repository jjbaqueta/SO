#ifndef FILA_INCLUDED
#define FILA_INCLUDED

// Fila implementada usando um array circular
typedef struct{
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
    int* dados;
} Fila;

// Declarações das funções que manipulam a Fila
Fila* criar(int capacidade);
int inserir(Fila* fila, int valor);
int remover(Fila* fila);
int buscar(Fila* fila, int valor);
void exibir(Fila* fila);
void destruir(Fila* fila);

#endif // FILA_INCLUDED
