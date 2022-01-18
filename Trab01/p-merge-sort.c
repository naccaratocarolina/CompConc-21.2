#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#include "merge-sort.h"

typedef struct indices Indices;
struct indices {
    int* a; // vetor a ser ordenado
    int ini, fim; // indices de inicio e fim
};

void * p_merge_sort (void *arg) {
    Indices indices = * ((Indices *) arg);
    int* a = indices.a;
    int ini = indices.ini;
    int fim = indices.fim;

    if (ini < fim) {
        // Divide
        int meio = ini + (fim - ini) / 2;

        pthread_t t1;
        Indices indices1 = {a, ini, meio};

        pthread_t t2;
        Indices indices2 = {a, meio + 1, fim};

        // Conquista
        if (pthread_create(&t1, NULL, p_merge_sort, &indices1)) {
            fprintf(stderr, "--ERRO: pthread_create\n");
            exit(3);
        }

        if (pthread_create(&t2, NULL, p_merge_sort, &indices2)) {
            fprintf(stderr, "--ERRO: pthread_create\n");
            exit(3);
        }

        // Combina
        if (pthread_join(t1, NULL)) {
            fprintf(stderr, "--ERRO: pthread_join\n");
            exit(3);
        }

        if (pthread_join(t2, NULL)) {
            fprintf(stderr, "--ERRO: pthread_join\n");
            exit(3);
        }

        merge(a, ini, meio, fim);
    }

    pthread_exit(NULL);
}

void t2 (int a[], int dim) {
    pthread_t t; // Identificador da thread principal

    Indices indices = {a, 0, dim - 1};

    // Cria a thread
    if (pthread_create(&t, NULL, p_merge_sort, &indices)) {
        fprintf(stderr, "--ERRO: pthread_create\n");
        exit(3);
    }

    // Aguarda o termino da thread
    if (pthread_join(t, NULL)) {
        fprintf(stderr, "--ERRO: pthread_join\n");
        exit(3);
    }
}

void t4 (int a[], int dim) {
    int ini = 0;
    int fim = dim - 1;
    int meio = ini + (fim - ini) / 2;

    pthread_t t1;
    Indices indices1 = {a, ini, meio};

    pthread_t t2;
    Indices indices2 = {a, meio + 1, fim};

    // Cria as threads
    if (pthread_create(&t1, NULL, p_merge_sort, &indices1)) {
        fprintf(stderr, "--ERRO: pthread_create\n");
        exit(3);
    }

    if (pthread_create(&t2, NULL, p_merge_sort, &indices2)) {
        fprintf(stderr, "--ERRO: pthread_create\n");
        exit(3);
    }

    // Aguarda o termino das threads
    if (pthread_join(t1, NULL)) {
        fprintf(stderr, "--ERRO: pthread_join\n");
        exit(3);
    }

    if (pthread_join(t2, NULL)) {
        fprintf(stderr, "--ERRO: pthread_join\n");
        exit(3);
    }

    // Combina
    merge(a, ini, meio, fim);
}