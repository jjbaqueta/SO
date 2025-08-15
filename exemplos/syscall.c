#include <unistd.h> // Para as chamadas de sistema write() e _exit()

int main(){
    /*
     * A função write() escreve dados diretamente em um descritor de arquivo.
     * Argumento 1: '1' é o descritor da saída padrão (stdout), o terminal.
     * Argumento 2: "Olá Mundo!\n" é o ponteiro para os dados a serem escritos.
     * Argumento 3: '12' é o número exato de bytes a serem escritos.
     */
    write(1, "Olá Mundo!\n", 12);
    
    /*
     * A função _exit() termina o processo imediatamente, sem limpar os buffers
     * da biblioteca C padrão. É uma chamada direta ao kernel.
     * O '0' indica que o programa terminou com sucesso.
     */
    _exit(0);
}
