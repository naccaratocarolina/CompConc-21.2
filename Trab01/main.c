#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "merge-sort.h"
#include "p-merge-sort.h"

/* ==========================================================
   Variaveis Globais
   ========================================================== */
int *a;
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

    // Aloca o vetor de entrada e tmp
    a = (int *) malloc(sizeof(int) * dim);
    if (a == NULL) {
        perror("--ERRO: malloc");
        return 2;
    }

    /* ==========================================================
        Versao Sequencial
       ========================================================== */
    puts("Versão Sequencial");

    // Preenche o vetor de entrada com inteiros aleatorios
    preenche(a, dim);

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
    printf("Tempo decorrido = %ld milissegundos\n", tempo);

    /* ==========================================================
        Versao Concorrente
       ========================================================== */
    puts("\nVersão Concorrente");

    // Preenche o vetor de entrada com inteiros aleatorios
    preenche(a, dim);

    // Comeca contagem do tempo
    gettimeofday(&inicio, NULL);

    // Ordena vetor
    p_ordena(a, dim, nthreads);

    // Encerra contagem do tempo
    gettimeofday(&fim, NULL);

    // Verifica corretude
    verificaCorretude(a, dim);

    // Imprime tempo decorrido
    tempo = (fim.tv_sec*1000000 + fim.tv_usec) - (inicio.tv_sec*1000000 + inicio.tv_usec);
    printf("Tempo decorrido = %ld milissegundos\n", tempo);
}