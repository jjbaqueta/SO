#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"

Fila* criar(int capacidade){
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;
    fila->capacidade = capacidade;
    fila->dados = (int*) malloc(capacidade * sizeof(int));
    return fila;
}

int inserir(Fila* fila, int valor){
    if (fila->tamanho == fila->capacidade) {
        printf("A fila está cheia!\n");
        return 0;
    }
    int posicao = fila->fim % fila->capacidade;
    fila->dados[posicao] = valor;
    fila->fim = posicao + 1;
    fila->tamanho++;
    return 1;
}

int remover(Fila* fila){
    if (fila->tamanho == 0) {
        printf("A fila está vazia!\n");
        return -1;
    }
    int posicao = fila->inicio % fila->capacidade;
    int valor = fila->dados[posicao];
    fila->inicio = posicao + 1;
    fila->tamanho--;
    return valor;
}

int buscar(Fila* fila, int valor){
    for (int i = 0; i < fila->tamanho; i++) {
        int posicao = (i + fila->inicio) % fila->capacidade;
        if (fila->dados[posicao] == valor) {
            return 1;
        }
    }
    return 0;
}

void exibir(Fila* fila){
    printf("[");
    for (int i = 0; i < fila->tamanho; i++) {
        int posicao = (i + fila->inicio) % fila->capacidade;
        printf("%d ", fila->dados[posicao]);
    }
    printf("]\n");
}

void destruir(Fila* fila){
    free(fila->dados);
    free(fila);
}
