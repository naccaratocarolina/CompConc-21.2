#include <stdio.h>
#include <stdlib.h>

/* Variaveis globais */
int *vetor;
long long int dim;

/**
 * Mescla duas subsequências ordenadas para produzir a resposta ordenada.
 * ini <= meio < fim
 * @param ini indice do primeiro elemento da sequencia
 * @param meio indice do elemento central da sequencia
 * @param indice do ultimo elemento da sequencia
 */
void merge(int ini, int meio, int fim) {
    // Computa o tamanho das subsequencias
    int tam1 = meio - ini + 1;
    int tam2 = fim - meio;

    // Inicializa subsequencias da esq e dir
    // E[ini...meio], D[meio + 1...fim]
    int E[tam1], D[tam2];

    for (int i=0; i < tam1; i++)
        E[i] = vetor[ini + i];

    for (int j=0; j < tam2; j++)
        D[j] = vetor[meio + 1 + j];

    // Seleciona os menores elementos das sequencias
    int i = 0;
    int j = 0;
    int k = ini;

    while (i < tam1 && j < tam2) {
        if (E[i] <= D[j]) {
            vetor[k] = E[i];
            i += 1;
        }

        else {
            vetor[k] = D[j];
            j += 1;
        }
        k += 1;
    }

    // Copia o restante dos elementos
    while (i < tam1) {
        vetor[k] = E[i];
        i += 1;
        k += 1;
    }

    while (j < tam2) {
        vetor[k] = D[j];
        j += 1;
        k += 1;
    }
}

/**
 * Divide a sequência de n elementos a ser ordenada em duas subsequências de n/2
 * elementos cada. Em seguida, ordena duas subsequencias realizando chamadas
 * recursovas a merge_sort. Isto feito, une as duas subsequencias ordenadas para
 * produzir o vetor ordenado.
 * @param ini indice do primeiro elemento da sequencia
 * @param fim indice do ultimo elemento da sequencia
 */
void merge_sort (int ini, int fim) {
    if (ini < fim) {
        // Divide
        int middle = (ini + fim)/2;
        // Conquista
        merge_sort(ini, middle);
        merge_sort(middle + 1, fim);
        // Combina
        merge(ini, middle, fim);
    }
}

int main (int argc, char  *argv[]) {
    // Leitura e avaliacao dos parametros de entrada
    if (argc < 2) {
        fprintf(stderr, "Digite: %s <dimensao do vetor>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);

    // Aloca o vetor de entrada
    vetor = (int *) malloc(sizeof(int) * dim);
    if (vetor == NULL) {
        fprintf(stderr, "--ERRO: malloc\n");
        return 2;
    }

    // Preenche o vetor de entrada com inteiros aleatorios
    for (int i=0; i<dim; i++)
        // Gera um inteiro dentro do intervalo [1, dim]
        vetor[i] = 1 + rand() % dim;

    merge_sort(0, dim-1);

    puts("Vetor ordenado:");
    for (int i=0; i<dim; i++)
        printf("%.d ", vetor[i]);
    puts("");

    return 0;
}