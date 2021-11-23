#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Total de threads a serem criadas
#define NUM_THREADS 2

// Tamanho do vetor
#define TAM_VETOR 10000

// Vetor de elementos (variavel global)
int vetor[TAM_VETOR];

/*
 * Handler das threads.
 * Funcao que eleva ao quadrado cada elemento de um vetor.
 *
 * Para dividir a tarefa de incrementar todos os elementos do vetor
 * entre duas threads, iremos dividir o mesmo em partes iguais, tomando
 * como referencia o tamanho do vetor e a quantidade de threads criadas.
 * Dessa forma, cada threads executa 1/NUM_THREADSth do vetor. Como temos
 * duas threads, a primeira executa do index 0 -> 4.999, e a outra do index
 * 5.000 -> 9999
 */
void * elevaAoQuadrado (void * arg) {
  int id_thread = * (int *) arg;

   for (int i=id_thread*(TAM_VETOR/NUM_THREADS); i<(id_thread+1)*(TAM_VETOR/NUM_THREADS); i++)
     vetor[i] = (i+1) * (i+1);

  pthread_exit(NULL);
}

/*
 * Funcao que verica se os valores finais no vetor estao corretos.
 *
 * @return 1 se os valores estiverem corretos, 0 c.c.
 */
int verificaResultado () {
  for (int i=0; i<TAM_VETOR; i++)
    if (!(vetor[i] == (i+1)*(i+1)))
      return 0;

  return 1;
}

// Funcao principal
int main () {
  pthread_t tid_sistema[NUM_THREADS]; // Identificador das threads no sistema
  int tid_local[NUM_THREADS]; // Identificadores locais das threads
  int thread; // Variavel auxiliar que guarda o id das threads

  // Inicializa o vetor de elementos
  for (thread=0; thread<TAM_VETOR; thread++)
    vetor[thread] = thread + 1; // vetor comeca do 1 

  // Cria as threads passando a funcao elevaAoQuadrado como referencia
  for (thread=0; thread<NUM_THREADS; thread++) {
    tid_local[thread] = thread;

    if (pthread_create(&tid_sistema[thread], NULL, elevaAoQuadrado, (void *) &tid_local[thread])) {
      printf("--ERRO: pthread_create\n"); exit(-1);
    }
  }

  // Espera as threads terminarem (sincronizacao)
  for (thread=0; thread<NUM_THREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
      printf("--ERRO: pthread_join"); exit(-1);
    }
  }

  // Verifica se o resultado esta correto
  if (!verificaResultado()) {
    printf("--ERRO: Valor incorreto no vetor\n"); exit(-1);
  }

  printf("--Thread principal terminou\n");
  pthread_exit(NULL);

  return 0;
}
