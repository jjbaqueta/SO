/*
 * Demonstração de criação de processos com fork() e sincronização com wait()
 * O programa cria um processo filho usando fork(), fazendo com que o pai 
 * fique bloqueado em wait() aguardando o término do filho. 
 * O filho exibe suas informações e dorme por 3 segundos. 
 * Por fim, ambos os processos imprimem uma mensagem de encerramento e finalizam.
 * 
 * Comando de compilação: gcc fork.c -o exe
 * Comando de execução: ./exe
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    printf("Sou o processo PAI: %5d, meu pai é: %5d\n", getpid(), getppid());

    int retorno = fork();

    if (retorno < 0) {
        fprintf(stderr, "Erro ao criar o processo filho!\n");
        exit(1);
    }
    else if (retorno == 0) {
        printf("Sou o processo FILHO: %5d, meu pai é: %5d\n", getpid(), getppid());
        sleep(3);
    }
    else {
        wait(NULL);
    }

    printf("Processo finalizado! PID: %5d\n", getpid());
    return 0;
}
