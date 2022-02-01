#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "merge-sort.h"
#include "p-merge-sort.h"

/* ==========================================================
   Variaveis Globais
   ========================================================== */
int *a; // vetor de entrada para o merge sort sequencial
int *b; // vetor de entrado para o merge sort concorrente
long long int dim;
int nthreads;

struct timeval inicio, fim;
long tempo; // # milissegundos de tempo decorrido

int main (int argc, char *argv[]) {
    // Leitura e avaliacao dos parametros de entrada
    if (argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    if (!ehPotenciaDeDois(nthreads)) {
        fprintf(stderr, "--ERRO: Numero de threads deve ser potencia de 2.\n");
        return 1;
    }

    // Aloca o vetor de entrada
    a = (int *) malloc(sizeof(int) * dim);
    if (a == NULL) {
        perror("--ERRO: malloc");
        return 2;
    }

    b = (int *) malloc(sizeof(int) * dim);
    if (b == NULL) {
        perror("--ERRO: malloc");
        return 2;
    }

    // Preenche o vetor de entrada com inteiros aleatorios
    preenche(a, b, dim);

    /* ==========================================================
        Versao Sequencial
       ========================================================== */
    puts("Versão Sequencial");

    // Comeca contagem do tempo
    gettimeofday(&inicio, NULL);

    // Ordena vetor
    merge_sort(a, 0, dim);

    // Encerra contagem do tempo
    gettimeofday(&fim, NULL);

    // Verifica corretude
    verificaCorretude(a, dim);

    // Imprime tempo decorrido
    tempo = (fim.tv_sec*1000000 + fim.tv_usec) - (inicio.tv_sec*1000000 + inicio.tv_usec);
    printf("Tempo decorrido = %ld microssegundos\n", tempo);

    /* ==========================================================
        Versao Concorrente
       ========================================================== */
    puts("\nVersão Concorrente");

    // Comeca contagem do tempo
    gettimeofday(&inicio, NULL);

    // Ordena vetor
    p_ordena(b, dim, nthreads);

    // Encerra contagem do tempo
    gettimeofday(&fim, NULL);

    // Verifica corretude
    verificaCorretude(b, dim);

    // Imprime tempo decorrido
    tempo = (fim.tv_sec*1000000 + fim.tv_usec) - (inicio.tv_sec*1000000 + inicio.tv_usec);
    printf("Tempo decorrido = %ld microssegundos\n", tempo);

    // Desaloca e termina
    free(a);
    free(b);
}
