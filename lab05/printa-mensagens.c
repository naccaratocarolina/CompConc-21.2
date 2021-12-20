/* Disciplina: Computacao Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratorio: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */
/***** Condicao logica da aplicacao: A thread 5 deve sempre ser a primeira a imprimir sua mensagem, e a thread 1, a ultima.  ****/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Total de threads a serem criadas
#define NUM_THREADS 5

/* Variaveis globais */
int mensagens = 0; // Inicializa contador de mensagens impressas
pthread_mutex_t mutex;
pthread_cond_t cond_1, cond_5;

/* Thread 1: "Volte sempre!" */
void * t1 (void * arg)  {
  pthread_mutex_lock(&mutex);
  while (mensagens < 4) // Enquanto todas as threads nao tiverem terminado a sua execucao
    pthread_cond_wait(&cond_1, &mutex); // Entra em espera
  pthread_mutex_unlock(&mutex); // Sinaliza que pode avancar

  printf("Volte sempre!\n");

  pthread_exit(NULL);
}

/* Thread 2: "Fique a vontade." */
void * t2 (void * arg)  {
  pthread_mutex_lock(&mutex);
  if (mensagens == 0) // Se t5 nao tiver executado
    pthread_cond_wait(&cond_5, &mutex); // Entra em espera
  pthread_mutex_unlock(&mutex); // Sinaliza que pode avancar

  printf("Fique a vontade.\n");

  pthread_mutex_lock(&mutex);
  mensagens++; // Incrementa o total de mensagens impressas
  pthread_cond_signal(&cond_1); // Sinaliza a condicao
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

/* Thread 3: "Sente-se por favor." */
void * t3 (void * arg)  {
  pthread_mutex_lock(&mutex);
  if (mensagens == 0) // Se t5 nao tiver executado
    pthread_cond_wait(&cond_5, &mutex); // Entra em espera
  pthread_mutex_unlock(&mutex); // Sinaliza que pode avancar

  printf("Sente-se por favor.\n");

  pthread_mutex_lock(&mutex);
  mensagens++; // Incrementa o total de mensagens impressas
  pthread_cond_signal(&cond_1); // Sinaliza a condicao
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

/* Thread 4: "Aceita um copo d’agua?." */
void * t4 (void * arg)  {
  pthread_mutex_lock(&mutex);
  if (mensagens == 0) // Se t5 nao tiver executado
    pthread_cond_wait(&cond_5, &mutex); // Entra em espera
  pthread_mutex_unlock(&mutex); // Sinaliza que pode avancar

  printf("Aceita um copo d'agua?.\n");

  pthread_mutex_lock(&mutex);
  mensagens++; // Incrementa o total de mensagens impressas
  pthread_cond_signal(&cond_1); // Sinaliza a condicao
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

/* Thread 5: "Seja bem-vindo!" */
void * t5 (void * arg)  {
  printf("Seja bem-vindo\n");

  pthread_mutex_lock(&mutex);
  mensagens++; // Incrementa o total de mensagens impressas
  pthread_cond_broadcast(&cond_5); // Chamada de sincronizacao
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

/* Funcao principal */
int main (int argc, char *argv[]) {
  pthread_t tid_sistema[NUM_THREADS]; // Identificadores das threads no sistema

  // Inicializa o mutex (lock de exclusao mutua) e as variaveis de condicao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_1, NULL);
  pthread_cond_init(&cond_5, NULL);

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
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond_1);
  pthread_cond_destroy(&cond_5);

  return 0;
}
