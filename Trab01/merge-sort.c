#include <stdio.h>
#include <stdlib.h>
#include "merge-sort.h"

void merge (int a[], int ini, int meio, int fim) {
    // Computa o tamanho das subsequencias
    int tam1 = meio - ini + 1;
    int tam2 = fim - meio;

    // Inicializa subsequencias da esq e dir
    // E[ini...meio], D[meio + 1...fim]
    int E[tam1], D[tam2];

    for (int i=0; i < tam1; i++)
        E[i] = a[ini + i];

    for (int j=0; j < tam2; j++)
        D[j] = a[meio + 1 + j];

    // Seleciona os menores elementos das sequencias
    int i = 0;
    int j = 0;
    int k = ini;

    while (i < tam1 && j < tam2) {
        if (E[i] <= D[j]) {
            a[k] = E[i];
            i += 1;
        }

        else {
            a[k] = D[j];
            j += 1;
        }
        k += 1;
    }

    // Copia o restante dos elementos
    while (i < tam1) {
        a[k] = E[i];
        i += 1;
        k += 1;
    }

    while (j < tam2) {
        a[k] = D[j];
        j += 1;
        k += 1;
    }
}

void merge_sort (int a[], int ini, int fim) {
    if (ini < fim) {
        // Divide
        int meio = (ini + fim)/2;

        // Conquista
        merge_sort(a, ini, meio);
        merge_sort(a, meio + 1, fim);

        // Combina
        merge(a, ini, meio, fim);
    }
}

void imprime (int a[], int dim) {
    for (int i=0; i<dim; i++)
        printf("%d ", a[i]);
    puts("");
}

void verificaCorretude (int a[], int dim) {
    for (int i=1; i<dim; i++)
        if (a[i - 1] > a[i]) {
            fprintf(stderr,
                    "--ERRO: a[%d] = %d e a[%d] = %d estao fora de ordem!\n",
                    i - 1, a[i - 1], i, a[i]);
            exit(1);
        }
}