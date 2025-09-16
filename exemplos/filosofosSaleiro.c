// Comando de compilação: gcc -Wall filosofosSaleiro.c -lpthread -o out
// Comando de execução: ./out

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_FILOSOFOS 5                 // Número de threads

pthread_t filosofos[NUM_FILOSOFOS];     // Uma thread por filósofo
sem_t hashis[NUM_FILOSOFOS];            // Um semáforo para cada hashi
sem_t saleiro;                          // Para que o filósofo pegue os hashis, ele precisa primeiro pegar o saleiro

// Configuração das colunas de impressão
char* formatacao[] = {"", "\t", "\t\t", "\t\t\t", "\t\t\t\t"};

// Faz a thread esperar por n segundos
void esperar(int n){
    sleep(n);
}

// Filósofo comendo 
void comer(int i) {
    printf("%sCOMENDO...\n", formatacao[i]);
//    esperar(2);
}

// Filósofo meditando 
void meditar(int i) {
    printf("%smeditando...\n", formatacao[i]);
//    esperar(2);
}

// Filósofo solicita o hashi h
void pegarHashi(int i, int h) {
    printf("%squer HASHI %d\n", formatacao[i], h);
    sem_wait(&hashis[h]);
    printf("%spegou HASHI %d\n", formatacao[i], h);
}

// Filósofo libera o hashi h
void largarHashi(int i, int h) {
    printf("%slarga HASHI %d\n", formatacao[i], h);
    sem_post(&hashis[h]);
}

// Comportamento do filósofo
void* filosofo(void* id) {
    int threadId = (long int) id;

    while(1){
        meditar(threadId);
        sem_wait(&saleiro);
        pegarHashi(threadId, threadId);
        pegarHashi(threadId, (threadId + 1) % NUM_FILOSOFOS);
        sem_post(&saleiro);
        comer(threadId);
        largarHashi(threadId, threadId);
        largarHashi(threadId, (threadId + 1) % NUM_FILOSOFOS);
    }
    pthread_exit(NULL);
}

// Função principal
int main(){

    // Desativa o buffer de stdout, garantindo que cada printf seja exibido imediatamente
    setvbuf(stdout, 0, _IONBF, 0);

    // Inicializando o saleiro
    sem_init(&saleiro, 0, 1);

    // Os hashi são inicializados
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&hashis[i], 0, 1);
    }    

    // Os filósofos são criados
    for (long i = 0; i < NUM_FILOSOFOS; i++) {
        if (pthread_create(&filosofos[i], NULL, filosofo, (void*) i) != 0) {
            perror("Erro ao criar os filósofos");
            exit(1);
        }
    }

    pthread_exit(NULL);
}
