// Comando de compilação: gcc -Wall leituraArq.c -o out
// Comando de execução: ./out

#include <stdio.h>

#define TAMANHO_LINHA 50

int main() {
    FILE* arquivo = fopen("arquivo.txt", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    char linha[TAMANHO_LINHA];

    while(fgets(linha, TAMANHO_LINHA, arquivo) != NULL){
        printf("%s", linha);
    }
    
    fclose(arquivo);
    return 0;
}
