// Comando de compilação: gcc -Wall somatorio.c -lpthread -o out
// Comando de execução: ./out

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 100         // Número de threads
#define NUM_ITERACOES 100000    // Número de iterações

int soma = 0;

void* threadComportamento(void* id) {
    for (int i = 0; i < NUM_ITERACOES; i++) {
        // Seção crítica        
        soma += 1;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    // Threads criadas serão JOINABLE
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Criando as threads
    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], &attr, threadComportamento, (void*) i) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Erro na sincronização das threads");
            exit(1);
        }
    }

    // Exibe o resultado esperado
    printf("Soma esperada: %d - Soma obtida: %d\n", NUM_THREADS * NUM_ITERACOES, soma);

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}
