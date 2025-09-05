// Comando de compilação: gcc -Wall altEstrita.c -lpthread -o out
// Comando de execução: ./out

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 100         // Número de threads
#define NUM_ITERACOES 100       // Número de iterações

int soma = 0;
int turno = 0;

// Variáveis para contagem de tempo REAL
struct timespec inicio, fim;
double tempo;

// Variáveis para contagem de tempo CPU
clock_t inicioCPU, fimCPU;
double tempoCPU;


// Diretiva de entrada
void enter(long id){
    while(turno != id);         // Espera ativa
}

// Diretiva de saída
void leave(){
    turno = (turno + 1) % NUM_THREADS;
}

void* threadComportamento(void* id) {
    long threadId = (long) id;
    
    for (int i = 0; i < NUM_ITERACOES; i++) {
        // Seção crítica com alternância estrita (garantia de exclusão mútua)
        enter(threadId);
        soma += 1;
        leave();
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    // Threads criadas serão JOINABLE
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Contagem do tempo é iniciada
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    inicioCPU = clock();

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

    // Contagem do tempo é finalizada
    clock_gettime(CLOCK_MONOTONIC, &fim);
    fimCPU = clock();

    // Exibe o resultado esperado
    printf("Soma esperada: %d - Soma obtida: %d\n", NUM_THREADS * NUM_ITERACOES, soma);

    // Calcula o tempo decorrido em segundos
    tempo = (fim.tv_sec - inicio.tv_sec);
    tempo += (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("Tempo de execução: %.2f segundos\n", tempo);

    tempoCPU = ((double)(fimCPU - inicioCPU)) / CLOCKS_PER_SEC;
    printf("Tempo de CPU: %.2f segundos\n", tempoCPU);

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}
