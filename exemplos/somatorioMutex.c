// Comando de compilação: gcc -Wall somatorioMutex.c -lpthread -o out
// Comando de execução: ./out

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 100             // Número de threads
#define NUM_ITERACOES 100000        // Número de iterações

int soma = 0;
pthread_mutex_t mutex;

// Variáveis para contagem de tempo REAL
struct timespec inicio, fim;
double tempo;

// Variáveis para contagem de tempo CPU
clock_t inicioCPU, fimCPU;
double tempoCPU;

void* threadComportamento(void* id) {   
    for (int i = 0; i < NUM_ITERACOES; i++) {
        // Seção crítica
        pthread_mutex_lock(&mutex);
        soma += 1;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    // Threads criadas serão JOINABLE
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Inicializando o mutex
    /* Parâmetros:
     * Parâmetro 1: ponteiro para a variável do tipo pthread_mutex_t que será inicializada
     * Parâmetro 2: NULL - indica que o mutex usará modo padrão (comportamento básico)
     *
     * Por padrão, o mutex está desbloqueado
     */
    pthread_mutex_init(&mutex, NULL); 

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
