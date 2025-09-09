// Comando de compilação: gcc -Wall ponteSemaforo.c -lpthread -o out
// Comando de execução: ./out

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>             // Necessário para a função sleep()

#define NUM_THREADS 6           // Número de carros que tentarão atravessar a ponte
#define NUM_FAIXAS 3            // Número de faixas da ponte

sem_t semaforo;                 // Semáforo para controlar o número de carros na ponte
pthread_mutex_t mutexPonte;     // Mutex para proteger o acesso ao array

int ponte[NUM_FAIXAS];          // Array que representa as faixas da ponte

// Exibe o estado atual das faixas da ponte.
void exibirPonte() {
    printf("Ponte: [ ");
    for (int i = 0; i < NUM_FAIXAS; i++) {
        if (ponte[i] == -1) {
            printf("- ");
        } 
        else {
            printf("C%d ", ponte[i]);
        }
    }
    printf("]\n");
}

// Função executada por cada thread (carro) para atravessar a ponte
void* atravessarPonte(void* id) {
    long threadId = (long)id;
    int minhaFaixa = -1;

    printf("Carro %ld chegou e está esperando para atravessar\n", threadId);

    // Espera por uma vaga na ponte (controlado pelo semáforo)
    sem_wait(&semaforo);

    // --- INÍCIO da seção crítica 1 ---
    // Escrita no vetor - indica que uma faixa foi ocupada (escreve o id da thread em uma faixa)
    // Trava o mutex para garantir que apenas um carro escolha a faixa por vez
    pthread_mutex_lock(&mutexPonte);

    // Encontra uma faixa livre para ocupar
    for (int i = 0; i < NUM_FAIXAS; i++) {
        if (ponte[i] == -1) {
            ponte[i] = threadId;
            minhaFaixa = i;
            printf("Carro %ld ENTROU na ponte pela faixa %d\n", threadId, minhaFaixa);
            exibirPonte();
            break;
        }
    }

    // --- FIM da seção crítica 1 ---    
    // Libera o mutex para que outros carros possam entrar e escrever no vetor se houver vagas
    pthread_mutex_unlock(&mutexPonte);

    // Simula o tempo que o carro leva para atravessar a ponte (1 segundo)
    sleep(1);

    // --- INÍCIO da seção crítica 2 ---
    // Escrita no vetor - indica que uma faixa foi liberada (escreve -1 na faixa)
    pthread_mutex_lock(&mutexPonte);

    // Libera a faixa
    ponte[minhaFaixa] = -1;
    printf("Carro %ld SAIU da ponte pela faixa %d\n", threadId, minhaFaixa);
    exibirPonte();

    // --- FIM da seção crítica 2 ---    
    // Libera o mutex para que outros carros possam indicar que estão saindo da ponte
    pthread_mutex_unlock(&mutexPonte);

    // Sinaliza ao semáforo que uma vaga na ponte foi liberada
    sem_post(&semaforo);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    // Inicializa as faixas da ponte como vazias (-1)
    for (int i = 0; i < NUM_FAIXAS; i++) {
        ponte[i] = -1;
    }

    // Threads criadas serão JOINABLE
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Inicializando o semáforo com o número de faixas disponíveis
    sem_init(&semaforo, 0, NUM_FAIXAS);
    
    // Inicializando o mutex
    pthread_mutex_init(&mutexPonte, NULL);

    printf("Simulação iniciada (%d faixas livres, %d carros)\n", NUM_FAIXAS, NUM_THREADS);

    // Criando as threads (carros)
    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], &attr, atravessarPonte, (void*) i) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Erro no join das threads");
            exit(1);
        }
    }
    
    printf("\nSimulação finalizada (todos os carros atravessaram)\n");

    // Libera os recursos
    sem_destroy(&semaforo);
    pthread_mutex_destroy(&mutexPonte);
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}
