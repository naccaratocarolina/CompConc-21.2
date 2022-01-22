#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <math.h>

#include "merge-sort.h"

typedef struct args Args;
struct args {
    int id_thread;
    int* a;
    long long int dim;
    int nthreads;
};

void p_merge (int a[], long long dim, int nthreads) {
    int NUMEROS_POR_THREAD = dim / nthreads;
    int OFFSET = dim % nthreads;

    for (int i = nthreads; i > 1; i = i/2) {
        for (int j = 0; j < i; j+=2) {
            long long int ini = j * NUMEROS_POR_THREAD * (nthreads/i);
            long long int fim = (j + 2) * NUMEROS_POR_THREAD * (nthreads/i) - 1;

            long long int meio = ini + (fim - ini) / 2;

            if ((j+2)*(nthreads/i) == nthreads) fim +=OFFSET;

            merge(a, ini, meio, fim);
        }
    }
}

void * p_merge_sort (void *arg) {
    Args args = * ((Args *) arg);

    int id_thread = args.id_thread;
    int* a = args.a;
    long long int dim = args.dim;
    int nthreads = args.nthreads;

    int NUMEROS_POR_THREAD = dim / nthreads;
    int OFFSET = dim % nthreads;

    long long int ini = id_thread * NUMEROS_POR_THREAD;
    long long int fim = (id_thread + 1) * NUMEROS_POR_THREAD - 1;

    if (id_thread == nthreads - 1) fim += OFFSET;

    if (ini < fim) {
        // Divide
        long long int meio = ini + (fim - ini) / 2;

        // Conquista
        merge_sort(a, ini, meio);
        merge_sort(a, meio + 1, fim);

        // Combina
        merge(a, ini, meio, fim);
    }

    free(arg);

    pthread_exit(NULL);
}

void p_ordena (int a[], long long int dim, int nthreads) {
    pthread_t *tid_sistema; // Identificadores das threads

    // Aloca espaco para vetor de identificadores
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) {
        perror("--ERRO: malloc");
        exit(2);
    }

    // Cria as threads
    for (long int i=0; i<nthreads; i++) {
        Args* args = (Args *) malloc(sizeof(Args));

        args->id_thread = i;
        args->nthreads = nthreads;
        args->dim = dim;
        args->a = a;

        if (pthread_create(tid_sistema+i, NULL, p_merge_sort, args)) {
            perror("--ERRO: pthread_create");
            exit(3);
        }
    }

    // Aguarda o termino
    for (long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid_sistema+i), NULL)) {
            perror("--ERRO: pthread_join");
            exit(3);
        }
    }

    // Combina os subvetores ordenados pelas threads
    p_merge(a, dim, nthreads);
}

int ehPotenciaDeDois (int nthreads) {
    return (ceil(log2(nthreads)) == floor(log2(nthreads)));
}