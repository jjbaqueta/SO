/*
 * Demonstração de criação de processos com fork() e substituição de comportamento com execl()
 * O execl() sobrepõe o código do filho. As linhas que aparecem depois do execl, 
 * dentro do bloco do filho, nunca são executadas, e a mensagem "Processo finalizado!" 
 * só será impressa UMA VEZ (pelo processo pai).
 * 
 * Comando de compilação: gcc execl.c -o exe
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
        
        /* 
         * COMPORTAMENTO DO EXECL:
         * 1º argumento: Caminho absoluto do binário ("/bin/ls").
         * 2º argumento: Nome do comando por convenção ("ls").
         * 3º argumento: Parâmetro/opção enviado ao comando ("-l").
         * 4º argumento: NULL obrigatório indicando o fim da lista de argumentos.
         */
        execl("/bin/ls", "ls", "-l", NULL);

        /* 
         * CÓDIGO MORTO / INALCANSÁVEL:
         * Se o execl() tiver sucesso, as linhas abaixo deixam de existir na memória do filho.
         * Elas só rodariam se o execl() falhasse (se o caminho "/bin/ls" estivesse errado).
         */
        printf("Sou o processo FILHO: %5d, meu pai é: %5d\n", getpid(), getppid());
        sleep(3);
    }
    else {
        wait(NULL);
    }

    printf("Processo finalizado! PID: %5d\n", getpid());
    return 0;
}
