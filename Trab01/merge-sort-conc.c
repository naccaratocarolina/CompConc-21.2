#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

/* Variaveis globais */
int *vetor;
long long int dim;

/* Estrutura com os limites da sequencia */
typedef struct args Args;
struct args {
    int ini;
    int fim;
};

/**
 * Mescla duas subsequências ordenadas para produzir a resposta ordenada.
 * ini <= meio < fim
 * @param ini indice do primeiro elemento da sequencia
 * @param meio indice do elemento central da sequencia
 * @param indice do ultimo elemento da sequencia
 */
void merge(int ini, int meio, int fim) {
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
        if (E[i] <= D[j])
            vetor[k++] = E[i++];

        else
            vetor[k++] = D[j++];
    }

    // Copia o restante dos elementos
    while (i < tam1)
        vetor[k++] = E[i++];

    while (j < tam2)
        vetor[k++] = D[j++];
}

/**
 * Handler das threads.
 * Divide a sequencia de dim elementos a ser ordenada em duas subsequencias de n/2
 * elementos cada. Em seguida, ordena ambas sequencias ao mesmo tempo disparando
 * duas threads. Antes de avancar para a uniao das subsequencias ordenadas, se
 * assegura que ambas as threads terminaram a sua execucao (pthread_join). Isto feito,
 * une as duas subsequencias ordenadas para produzir o vetor ordenado.
 *
 * @param arg estrutura de dados contendo os limites da sequencia {0, dim - 1}
 */
void * merge_sort(void * arg) {
    Args args = *((Args *) arg);
    int ini = args.ini;
    int fim = args.fim;

    if (ini < fim) {
        // Divide
        int meio = ini+(fim - ini)/2;

        pthread_t t1;
        Args args1 = {ini, meio};

        pthread_t t2;
        Args args2 = {meio + 1, fim};

        // Conquista
        // Cria as threads
        if (pthread_create(&t1, NULL, merge_sort, &args1)) {
            fprintf(stderr, "--ERRO: pthread_create\n");
            exit(3);
        }

        if (pthread_create(&t2, NULL, merge_sort, &args2)) {
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
        merge(ini, meio, fim);
    }

    pthread_exit(NULL);
}

int main(int argc, char  *argv[]) {
    pthread_t t;

    // Leitura e avaliacao dos parametros de entrada
    if (argc < 2) {
        fprintf(stderr, "Digite: %s <dimensao do vetor>\n", argv[0]);
        return 1;
    }

    dim = atoll(argv[1]);

    // Aloca o vetor de entrada
    vetor = (int *) malloc(sizeof(int) * dim);
    if (vetor == NULL) {
        fprintf(stderr, "--ERRO: malloc\n");
        return 2;
    }

    // Preenche o vetor de entrada com inteiros aleatorios
    for (long int i=0; i<dim; i++)
        // Gera um inteiro dentro do intervalo [1, dim]
        vetor[i] = 1 + rand() % dim;

    // Inicializa estrutura de dados passando os limites da sequencia
    Args args = {0, dim - 1};

    // Cria a thread
    if (pthread_create(&t, NULL, merge_sort, &args)) {
        fprintf(stderr, "--ERRO: pthread_create\n");
        return 3;
    }

    // Aguarda o termino das threads
    if (pthread_join(t, NULL)) {
        fprintf(stderr, "--ERRO: pthread_join\n");
        return 3;
    }

    // Exibe resultados
    printf("Vetor ordenado: \n");
    for (int i=0; i < dim; i++)
        printf("%d ", vetor[i]);
    puts("");

    return 0;
}