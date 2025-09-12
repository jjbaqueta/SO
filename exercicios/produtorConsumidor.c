// Comando de compilação: gcc -Wall produtorConsumidor.c -o out -lpthread
// Comando de execução: ./out

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define NUMERO_PRODUTORES 2
#define NUMERO_CONSUMIDORES 2
#define TAMANHO_LINHA 50
#define TAMANHO_BUFFER 2
#define ITEM_DE_CONTROLE "TERMINAR"  // Item especial que representa o fim das operações

// --- Semáforos e mutexs ---
sem_t semaforoVagas;
sem_t semaforoItens;
pthread_mutex_t mutexBuffer;

// --- Informação do buffer ---
char buffer[TAMANHO_BUFFER][TAMANHO_LINHA];
int indiceEscrita = 0;
int indiceLeitura = 0;

// Função que implementa os produtores
void* produtor(void* id) {
    long threadId = (long)id;

    // Abrindo o arquivo de texto
    char nomeArquivo[50];
    sprintf(nomeArquivo, "arquivo%ld.txt", threadId + 1);
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        pthread_exit(NULL);
    }
    
    // O produtor faz a leitura de cada linha do arquivo    
    char linha[TAMANHO_LINHA];

    while (fgets(linha, TAMANHO_LINHA, arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = 0;    // substitui o '\n' do final linha por um '\0'.

        // Espera por uma vaga no buffer
        sem_wait(&semaforoVagas);

        // Entra na seção crítica para inserir um item no buffer        
        pthread_mutex_lock(&mutexBuffer); 
        strcpy(buffer[indiceEscrita], linha);
        printf("[PRODUTOR %ld] inseriu '%s' no buffer (posição %d)\n", threadId + 1, linha, indiceEscrita);
        indiceEscrita = (indiceEscrita + 1) % TAMANHO_BUFFER;
        pthread_mutex_unlock(&mutexBuffer);
        
        // Avisa que um novo item foi adicionado no buffer
        sem_post(&semaforoItens);

        // Simula trabalho (espera 0.1 segundos)
        usleep(100000);
    }
    printf("[PRODUTOR %ld] terminou sua produção\n", threadId + 1);

    // Fecha o arquivo e finaliza a thread
    fclose(arquivo);
    pthread_exit(NULL);
}

// Função que implementa os consumidores
void* consumidor(void* id) {
    long threadId = (long)id;
    char item[TAMANHO_LINHA];

    while (1) {
        // Espera por um item no buffer
        sem_wait(&semaforoItens);

        // Entra na seção crítica para consumir um item do buffer
        pthread_mutex_lock(&mutexBuffer);
        strcpy(item, buffer[indiceLeitura]);
        printf("[CONSUMIDOR %ld] removeu '%s' do buffer (posição %d)\n", threadId + 1, item, indiceLeitura);
        indiceLeitura = (indiceLeitura + 1) % TAMANHO_BUFFER;
        pthread_mutex_unlock(&mutexBuffer);

        // Avisa que uma posição do buffer está livre
        sem_post(&semaforoVagas);

        // Se o item for o ITEM_DE_CONTROLE, o consumidor é finalizado.
        if (strcmp(item, ITEM_DE_CONTROLE) == 0) {
            printf("[CONSUMIDOR %ld] encontrou o item de controle - Encerrando ...\n", threadId + 1);
            break;
        }
        // Simula trabalho (espera 0.2 segundos)
        usleep(200000);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t produtores[NUMERO_PRODUTORES];
    pthread_t consumidores[NUMERO_CONSUMIDORES];

    // Inicializa os semáforos e mutexes
    sem_init(&semaforoVagas, 0, TAMANHO_BUFFER);
    sem_init(&semaforoItens, 0, 0);
    pthread_mutex_init(&mutexBuffer, NULL);

    // Criando as threads produtoras e consumidoras
    for (long i = 0; i < NUMERO_PRODUTORES; i++) {
        pthread_create(&produtores[i], NULL, produtor, (void*)i);
    }
    for (long i = 0; i < NUMERO_CONSUMIDORES; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, (void*)i);
    }

    // Sincronização: espera pela finalização de todos os produtores
    for (int i = 0; i < NUMERO_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }

    // Após os produtores terminarem, os consumidores que podem estar dormindo são acordados
    // Utilizando o padrão de design Poison Pill finalizar o processamento dos consumidores
    printf("\nMAIN: Produtores finalizaram. Acordando consumidores...\n");

    for (int i = 0; i < NUMERO_CONSUMIDORES; i++) {

        // Espera uma vaga como um produtor
        sem_wait(&semaforoVagas);

        // Entra na seção crítica para inserir o item terminador no buffer        
        pthread_mutex_lock(&mutexBuffer);
        strcpy(buffer[indiceEscrita], ITEM_DE_CONTROLE);
        indiceEscrita = (indiceEscrita + 1) % TAMANHO_BUFFER;
        pthread_mutex_unlock(&mutexBuffer);

        // Avisa que um novo item foi adicionado no buffer (item de controle)
        sem_post(&semaforoItens);     
    }

    // Sincronização: espera pela finalização de todos os consumidores
    for (int i = 0; i < NUMERO_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    printf("MAIN: Simulação finalizada\n");

    sem_destroy(&semaforoVagas);
    sem_destroy(&semaforoItens);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}
