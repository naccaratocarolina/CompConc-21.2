/* Disciplina: Computacao Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratorio: 10 */
/* Codigo: Implementar uma variacao na implementacao do problema produtor/consumidor usando semaforos. */
/***** A cada execucao de um produtor, ele preenche o buffer inteiro. O consumidor consome um item de cada vez.  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // tamanho do buffer
int num_prod; // qtde de threads produtoras
int num_cons; // qtde de threads consumidoras

// Variaveis do problema
int Buffer[N]; // espaco de dados compartilhados
int count = 0, in = 0, out = 0; // variaveis de estado
 
// Variaveis para sincronizacao
sem_t sem_prod;
sem_t sem_cons;

// Imprime o buffer
void Imprime (int n) {
  int i;
  for (i=0; i<n; i++)
    printf("%d ", Buffer[i]);
  puts("");
}

// Insere um elemento no Buffer ate preenche-lo por inteiro. Se
// estiver cheio, permite que os consumidores executem.
void Insere (int item, int id) {
  // Espera ser liberado pelos produtores
  sem_wait(&sem_prod);

  // Preenche o buffer e atualiza variaveis de estado
  Buffer[in] = item;
  in = (in + 1) % N;
  count++;

  // Imprime mensagem na tela
  printf("Thread produtora %d inseriu %d\n", id, item);
  Imprime(N);
  
  // Se o buffer estiver cheio, sinaliza para os consumidores,
  // enquanto mantem os produtores bloqueados
  if (count == N) {
    sem_post(&sem_cons);
  }

  // Se o buffer nao estiver cheio, sinaliza para os produtores,
  // enquanto mantem os consumidores bloqueados
  else {
    sem_post(&sem_prod);
  }
}

void Retira (int id) {
  int item;

  // Espera ser liberado pelos consumidores
  sem_wait(&sem_cons);

  // Imprime mensagem na tela
  printf("Thread consumidora %d quer consumir buffer\n", id);

  // Consome o buffer e atualiza variaveis de estado
  item = Buffer[out];
  out = (out + 1) % N;
  count--;
  printf("Thread consumidora %d consumiu %d\n", id, item);
  Imprime(N);

  // Sinaliza threads produtoras, as liberando
  sem_post(&sem_prod);
}

// Threads Produtoras
void * produtor (void * args) {
  long int id_thread = (long int) args; // Identificador da thread

  printf("Sou a thread produtora %ld\n", id_thread);
  while (1) {
    // Produzindo o item
    Insere(id_thread, id_thread);
    sleep(1);
  }

  free(args);
  pthread_exit(NULL);
}

// Threads Consumidoras
void * consumidor (void * args) {
  long int id_thread = (long int) args; // Identificador da thread

  printf("Sou a thread consumidora %ld\n", id_thread);
  while (1) {
    // Consome item
    Retira(id_thread);
    sleep(1); // faz o processamento do item
  }

  free(args);
  pthread_exit(NULL);
}

// Funcao principal
int main (int argc, char* argv[]) {
  // Leitura e avaliacao dos parametros de entrada
  if (argc < 3) {
    printf("Digite: %s <numero de threads produtoras> <numero de threads consumidoras>\n", argv[0]);
    return 1;
  }
  num_prod = atoi(argv[1]);
  num_cons = atoi(argv[2]);

  // Cria identificadores das threads no sistema
  pthread_t tid_sistema[num_prod + num_cons];
  
  // Inicializa as variaveis de sincronizacao
  sem_init(&sem_prod, 0, 1);
  sem_init(&sem_cons, 0, 0);

  // Inicializa Buffer
  for (int i=0; i<N; i++)
    Buffer[i] = 0;

  // Cria as threads produtoras
  for (long i=0; i<num_prod; i++) {
    if (pthread_create(&tid_sistema[i], NULL, produtor, (void *) i)) {
      fprintf(stderr, "--ERRO: pthread_create\n");
      return 2;
    }
  }

  // Cria as threads consumidoras
  for (long i=0; i<num_cons; i++) {
    if (pthread_create(&tid_sistema[i], NULL, consumidor, (void *) i)) {
      fprintf(stderr, "--ERRO: pthread_create\n");
      return 2;
    }
  }

  // Aguarda o termino das threads
  for (int i=0; i<(num_prod + num_cons); i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
      fprintf(stderr, "--ERRO: pthread_join\n");
      return 2;
    }
  }
  
  // Desaloca variaveis e termina
  sem_destroy(&sem_prod);
  sem_destroy(&sem_cons);

  return 0;
}
