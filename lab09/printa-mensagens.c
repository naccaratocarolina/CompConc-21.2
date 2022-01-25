/* Disciplina: Computacao Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratorio: 9 */
/* Codigo: Uso de semaforos para implementar exclusao mutua e sincronizacao condicional. */
/***** Condicao logica da aplicacao: A thread 5 deve sempre ser a primeira a imprimir sua mensagem, e a thread 1, a ultima. ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

/* Variaveis globais */
#define NUM_THREADS 5
sem_t sem_1, sem_5;

/* Thread 1: "Volte sempre!" */
void * t1 (void * arg) {
  // Por ser a ultima thread a executar, espera
  // ser liberada pelas threads 2, 3 e 4
  sem_wait(&sem_1);
  sem_wait(&sem_1);
  sem_wait(&sem_1);

  // Imprime a mensagem
  printf("Volte sempre!\n");

  pthread_exit(NULL);
}

/* Thread 2: "Fique a vontade." */
void * t2 (void * arg)  {
  // Espera a thread 5 executar
  sem_wait(&sem_5);

  // Imprime a mensagem
  printf("Fique a vontade.\n");

  // Libera a thread 1, incrementando a sua
  // variavel semaforo
  sem_post(&sem_1);

  pthread_exit(NULL);
}

/* Thread 3: "Sente-se por favor." */
void * t3 (void * arg)  {
  // Espera a thread 5 executar
  sem_wait(&sem_5);

  // Imprime a mensagem
  printf("Sente-se por favor.\n");

  // Libera a thread 1, incrementando a sua
  // variavel semaforo
  sem_post(&sem_1);

  pthread_exit(NULL);
}

/* Thread 4: "Aceita um copo d’agua?." */
void * t4 (void * arg)  {
  // Espera a thread 5 executar
  sem_wait(&sem_5);

  // Imprime a mensagem
  printf("Aceita um copo d'agua?\n");

  // Libera a thread 1, incrementando a sua
  // variavel semaforo
  sem_post(&sem_1);

  pthread_exit(NULL);
}

/* Thread 5: "Seja bem-vindo!" */
void * t5 (void * arg)  {
  // Imprime a mensagem sem nenhuma restricao
  // pois eh a primeira thread a ser executada
  printf("Seja bem-vindo!\n");

  // Libera  as threads 2, 3 e 4, incrementando
  // a sua variavel semaforo
  sem_post(&sem_5);
  sem_post(&sem_5);
  sem_post(&sem_5);

  pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
  pthread_t tid_sistema[NUM_THREADS]; // Identificadores das threads no sistema
  
  // Inicializa semaforo
  sem_init(&sem_1, 0, 0);
  sem_init(&sem_5, 0, 0);

  // Cria as threads
  if (pthread_create(&tid_sistema[0], NULL, t1, NULL)) {
    fprintf(stderr, "--ERRO: pthread_create\n");
    return 1;
  }

  if (pthread_create(&tid_sistema[1], NULL, t2, NULL)) {
    fprintf(stderr, "--ERRO: pthread_create\n");
    return 1;
  }

  if (pthread_create(&tid_sistema[2], NULL, t3, NULL)) {
    fprintf(stderr, "--ERRO: pthread_create\n");
    return 1;
  }

  if (pthread_create(&tid_sistema[3], NULL, t4, NULL)) {
    fprintf(stderr, "--ERRO: pthread_create\n");
    return 1;
  }

  if (pthread_create(&tid_sistema[4], NULL, t5, NULL)) {
    fprintf(stderr, "--ERRO: pthread_create\n");
    return 1;
  }

  // Espera todas as threads terminarem
  for (int i=0; i<NUM_THREADS; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
      fprintf(stderr, "--ERRO: pthread_join\n");
      return 1;
    }
  }

  // Desaloca variaveis e termina
  sem_destroy(&sem_1);
  sem_destroy(&sem_5);

  return 0;
}
