#include <stdio.h>
#include <stdlib.h>

#include "merge-sort.h"

void merge (int a[], long long int ini, long long int meio, long long int fim) {
    // Computa o tamanho das subsequencias
    long long int tam1 = meio - ini + 1;
    long long int tam2 = fim - meio;

    // Inicializa subsequencias da esq e dir
    // E[ini...meio], D[meio + 1...fim]
    long long int E[tam1], D[tam2];

    for (long long int i=0; i < tam1; i++)
        E[i] = a[ini + i];

    for (long long int j=0; j < tam2; j++)
        D[j] = a[meio + 1 + j];

    // Seleciona os menores elementos das sequencias
    long long int i = 0;
    long long int j = 0;
    long long int k = ini;

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

void merge_sort (int a[], long long int ini, long long int fim) {
    if (ini < fim) {
        // Divide
        long long int meio = (ini + fim)/2;

        // Conquista
        merge_sort(a, ini, meio);
        merge_sort(a, meio + 1, fim);

        // Combina
        merge(a, ini, meio, fim);
    }
}

void imprime (int a[], long long int dim) {
    for (long long int i=0; i<dim; i++)
        printf("%d ", a[i]);
    puts("");
}

void verificaCorretude (int a[], long long int dim) {
    for (long long int i=1; i<dim; i++)
        if (a[i - 1] > a[i]) {
            fprintf(stderr,
                    "--ERRO: a[%lld] = %d e a[%lld] = %d estao fora de ordem!\n",
                    i - 1, a[i - 1], i, a[i]);
            exit(1);
        }
}

void preenche (int a[], int b[], long long int dim) {
    for (long long int i=0; i<dim; i++) {
        // Gera um inteiro dentro do intervalo [0, dim]
        a[i] = rand() % (int) dim;
        b[i] = a[i];
    }
}
