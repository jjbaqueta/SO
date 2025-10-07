#include <stdio.h>
#include "Fila.h"

 /* O arquivo 'referencias20.txt' contém uma cadeia de referências para teste.
  * Esse arquivo pode ser baixado em: https://github.com/jjbaqueta/SO/blob/main/arquivos/referencias20.txt
  */
int main()
{
    // Cria uma fila com capacidade igual a 3
    Fila* fila = criar(3);

    // Abre o arquivo com a cadeia de referências
    FILE* arquivo = fopen("referencias20.txt", "r");

    int valor, acertos = 0, falhas = 0;

    // Leitura do arquivo
    while(fscanf(arquivo, "%d", &valor) != EOF){
        if (buscar(fila, valor)) {
            acertos++;
        }
        else {
            if (!inserir(fila, valor)) {
                remover(fila);
                inserir(fila, valor);
            }
            falhas++;
        }
        exibir(fila);
    }
    // Fecha o arquivo
    fclose(arquivo);

    // Exibe os resultados
    printf("acertos: %d\n", acertos);
    printf("falhas: %d\n", falhas);

    destruir(fila);
    return 0;
}
