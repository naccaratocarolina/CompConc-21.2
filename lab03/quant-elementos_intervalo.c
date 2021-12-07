#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

// Numero de elementos do vetor
long long int N;

// Numero de threads a serem criadas
int nthreads;

// Vetor de entrada com dimensao N
float *vetor;

// Limiares inferior e superior
float limiarInferior;
float limiarSuperior;

void * contaOcorrenciasEmIntervalo (void *arg) {
  long int id_thread = (long int) arg; // Identificador da thread
  long int *contLocal; // Variavel local que vai contar as ocorrencias
  long int tmp = 0;

  contLocal = (long int *) malloc(sizeof(long int));
  if (contLocal == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    exit(1);
  }

  long int tamBloco = N/nthreads; // Tamanho do bloco de cada thread
  long int ini = id_thread * tamBloco; // Elemento inicial do bloco da thread
  long int fim; // Elemento final (nao inclusive) do bloco da thread

  if (id_thread == nthreads - 1) fim = N;
  else fim = ini + tamBloco; // Trata o resto se houver

  for (long int i=ini; i<fim; i++)
    if (limiarInferior < vetor[i] && vetor[i] < limiarSuperior) tmp++;

  *contLocal = tmp;

  // Retorna o resultado do contador local
  pthread_exit((void *) contLocal);
}

float geraFloatAleatorio () {
  return ((float) rand()) / ((float)RAND_MAX) * 1000.1;
}

void printaTempo (double inicio, char mensagem[]) {
  double fim, delta;
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo %s: %lf\n", mensagem, delta);
}

int main (int argc, char *argv[]) {
  long int contSeq = 0; // Contador de ocorrencias sequencial
  long int contConc = 0; // Contador de ocorrencias concorrente
  pthread_t *tid_sistema; // Identificadores das threads no sistema
  long int *retorno; // Valor de retorno das threads
  double inicio; // Variavel que marca o inicio da contagem do tempo

  // Recebe e valida os parametros de entrada
  if (argc < 5) {
    fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads> <limiar inferior> <limiar superior>\n", argv[0]);
    return 1;
  }
  N = atoll(argv[1]);
  nthreads = atoi(argv[2]);
  limiarInferior = atoi(argv[3]);
  limiarSuperior = atoi(argv[4]);
   
  // Aloca o vetor de entrada
  vetor = (float *) malloc(sizeof(float) * N);
  if (vetor == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    return 2;
  }
  
  // Preenche o vetor de entrada com valores aleatorios do tipo float
  for (long int i=0; i<N; i++)
    vetor[i] = geraFloatAleatorio();

  // Solucao sequencial
  GET_TIME(inicio);
  for (long int i=0; i<N; i++)
    if (limiarInferior < vetor[i] && vetor[i] < limiarSuperior) contSeq++;

  printaTempo(inicio, "sequencial");

  // Solucao concorrente
  GET_TIME(inicio);
  tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  if (tid_sistema == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    return 2;
  }

  // Criar as threads
  for (long int i=0; i<nthreads; i++) {
    if(pthread_create(tid_sistema+i, NULL, contaOcorrenciasEmIntervalo, (void *) i)) {
      fprintf(stderr, "--ERRO: pthread_create\n");
      return 3;
    }
  }

  // Aguardar o termino das threads
  for (long int i=0; i<nthreads; i++) {
    if(pthread_join(*(tid_sistema+i), (void**) &retorno)) {
      fprintf(stderr, "--ERRO: pthread_create\n");
      return 3;
    }

    // Contador global
    contConc += *retorno;
  }

  printaTempo(inicio, "concorrente");

  // Exibir os resultados
  /* 
  puts("Elementos do vetor: ");
  for (long int i=0; i<N; i++)
   printf("%.3lf ", vetor[i]);
  puts("");
  */

  printf("Ocorrencias sequencial: %ld\n", contSeq);
  printf("Ocorrencias concorrente: %ld\n", contConc);

  // Verificar corretude
  if (contSeq != contConc) {
    fprintf(stderr, "--ERRO: Soma incorreta.\n");
    return 4;
  }
  
  // Libera as areas de memoria alocadas
  free(vetor);
  free(tid_sistema);

  return 0;
}
