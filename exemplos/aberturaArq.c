// Comando de compilação: gcc -Wall aberturaArq.c -o out
// Comando de execução: ./out

#include <stdio.h>

int main() {
    FILE* arquivo = fopen("arquivo.txt", "w");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    int a = 5;
    int b = 10;

    fprintf(arquivo, "%d %d\n", a, b);
    fclose(arquivo);

    return 0;
}
