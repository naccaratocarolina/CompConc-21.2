#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

// Variaveis globais
float *M1; // Primeira matriz de entrada
float *M2; // Segunda matriz de entrada
float *saida; // Matriz de saida (resultado da multiplicacao)
float *resultado; // Matriz aux para verificar o resultado final
int dim; // Dimensao das matrizes
int nthreads; // Numero de threads a serem criadas

void * multiplicaMatrizes (void * arg) {
  int id_thread = * (int *) arg;
  
  printf("Trhead %d\n", id_thread);
  for (int i=id_thread; i<dim; i=+nthreads) // Linhas de M1
    for (int j=0; j<dim; j++) // Colunas de M2
      for (int k=0; k<dim; k++) // Linhas de M1 e colunas de M2
        saida[i*dim+j] += M1[i*dim+j] * M2[i*dim+j];

  pthread_exit(NULL);
}

int verificaResultado (void) {
  for (int i=0; i<dim; i++)
    for (int j=0; j<dim; j++)
      for (int k=0; k<dim; k++) {
        if (resultado[i] != saida[i]) {
          printf("resultado[%.1f] != saida[%.1f]\n", resultado[i], saida[i]);
          return 0;
        }
      }

  return 1;
}

void printaTempo (double inicio, char mensagem[]) {
  double fim, delta;
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo %s: %lf\n", mensagem, delta);
}

int main (int argc, char* argv[]) {
  double inicio;

  GET_TIME(inicio);
  // Leitura e avaliacao dos parametros de entrada
  if (argc < 3) { // Nome do programa + dimensao das matrizes + numero de threads
    printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
    return 1;
  }

  dim = atoi(argv[1]);
  nthreads = atoi(argv[2]);

  // Inicializacao das variaveis de controle do programa
  pthread_t tid_sistema[nthreads]; // Identificadores das threads no sistema
  int tid_local[nthreads]; // Identificadores locais das threads
  int thread; // Variavel auxiliar

  // Alocacao dinamica de memoria para as estruturas de dados
  M1 = (float *) malloc(sizeof(float) * dim * dim);
  if (M1 == NULL) { printf("ERRO -- malloc\n"); return 2; }
  
  M2 = (float *) malloc(sizeof(float) * dim * dim);
  if (M2 == NULL) { printf("ERRO -- malloc\n"); return 2; }
  
  saida = (float *) malloc(sizeof(float) * dim * dim);
  if (saida == NULL) { printf("ERRO -- malloc\n"); return 2; }

  resultado = (float *) malloc(sizeof(float) * dim * dim);
  if (resultado == NULL) { printf("ERRO -- malloc\n"); return 2; }
  
  // Inicializacao das estruturas de dados
  for (int i=0; i<dim; i++)
    for (int j=0; j<dim; j++) {
      M1[i*dim+j] = rand() % 10;
      M2[i*dim+j] = rand() % 10;
      saida[i*dim+j] = 0;
      resultado[i*dim+j] = 0;
    }

  printaTempo(inicio, "inicializacao");
  
  // Multiplicacao matriz*matriz (sequencial)
  for (int i=0; i<dim; i++)
    for (int j=0; j<dim; j++)
      for (int k=0; k<dim; k++)
        resultado[i*dim+j] += M1[i*dim+j] * M2[i*dim+j];

  printaTempo(inicio, "multiplicacao sequencial");

  // Criacao das threads
  for (thread=0; thread<nthreads; thread++) {
    tid_local[thread] = thread;

    if (pthread_create(&tid_sistema[thread], NULL, multiplicaMatrizes, (void *) &tid_local[thread])) {
      printf("--ERRO: pthread_create\n"); exit(-1);
    }
  }

  // Espera as threads terminarem (sincronizacao)
  for (thread=0; thread<nthreads; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
      printf("--ERRO: pthread_join"); exit(-1);
    }
  }
  
  printaTempo(inicio, "multiplicacao concorrente");

  // Exibicao dos resultados
  /*
  puts("Matriz 1 de entrada:");
  for (int i=0; i<dim; i++) {
    for (int j=0; j<dim; j++)
      printf("%.1f ", M1[i*dim+j]);
    puts("");
  }

  puts("Matriz 2 de entrada:");
  for (int i=0; i<dim; i++) {
    for (int j=0; j<dim; j++)
      printf("%.1f ", M2[i*dim+j]);
    puts("");
  }
  
  puts("Matriz de saida:");
  for (int i=0; i<dim; i++) {
    for (int j=0; j<dim; j++)
      printf("%.1f ", saida[i*dim+j]);
    puts("");
  }
  
  puts("Matriz resultado:");
  for (int i=0; i<dim; i++) {
    for (int j=0; j<dim; j++)
      printf("%.1f ", resultado[i*dim+j]);
    puts("");
  }
  */

  // Verifica se o resultado esta correto
  if (!verificaResultado()) {
    printf("--ERRO: Valor incorreto no vetor\n"); exit(-1);
  }

  // Liberacao da memoria
  free(M1);
  free(M2);
  free(saida);
  free(resultado);
  
  printf("--Thread principal terminou\n");
  printaTempo(inicio, "finalizacao");

  return 0;
}
