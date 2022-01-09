/* Disciplina: Computacao Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratorio: 5 */
/* Codigo: Usar sincronizacao coletiva com barreira para somar elemento dos vetores */
/***** Condicao logica: A cada iteracao, cada thread soma todos os elementos do vetor e aguardam todas as threads terminarem suas somas *****/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Variaveis globais */
int bloqueadas = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

int N; // Tamanho do vetor e quantidade de threads a serem criadas
int *vetor;

/* Funcao barreira */
void barreira (int nthreads, int id_thread) {
  //--inicio SC
  pthread_mutex_lock(&mutex);

  if (bloqueadas == (nthreads - 1)) {
    // Ultima thread a chegar na barreira
    printf("Thread %d foi a ultima a chegar na barreira e esta liberando as demais que estao em espera ocupada\n", id_thread);
    pthread_cond_broadcast(&cond);
    bloqueadas = 0;
  } else {
    bloqueadas++;
    printf("Thread %d aguardando na barreira...\n", id_thread);
    pthread_cond_wait(&cond, &mutex);
  }

  //--fim SC
  pthread_mutex_unlock(&mutex);

}

/* Handler das threads */
void * somaElementos (void *arg) {
  long int id_thread = (long int) arg; // Identificador da thread
  int *somaLocal; // Variavel local que vai somar os elementos do vetor
  
  printf("Thread %ld executando...\n", id_thread);
  
  somaLocal = (int *) malloc(sizeof(int));
  if (somaLocal == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    exit(2);
  }
  *somaLocal = 0; // inicializando a var acumuladora local

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++)
      *somaLocal += vetor[j];

    // Aguardar todas as threads terminarem suas somas
    barreira(N, id_thread);

    // Gerar um novo valor aleatorio e escrever na posicao do
    // vetor de inteiros correspondente ao seu identificador
    vetor[id_thread] = rand() % 10;

    // Aguardar demais threads terminarem esse passo
    barreira(N, id_thread);
  }

  pthread_exit((void *) somaLocal);
}

/* Funcao principal */
int main (int argc, char* argv[]) {
  pthread_t *tid_sistema; // Identificadores das threads no sistema
  int *retorno; // Valor de retorno das threads
  int *somaGlobal; // Variavel acumuladora global

  // Leitura e avaliacao dos parametros de entrada
  if (argc < 2) {
    printf("Digite: %s <numero de threads>\n", argv[0]);
    return 1;
  }

  N = atoi(argv[1]);

  // Aloca o vetor de entrada
  vetor = (int *) malloc(sizeof(int) * N);
  if (vetor == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    return 2;
  }

  // Aloca soma global
  somaGlobal = (int *) malloc(sizeof(int) * N);
  if (somaGlobal == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    return 2;
  }

  // Preenche o vetor de entrada com valores aleatorios do tipo int
  for (int i=0; i<N; i++)
    vetor[i] = rand() % 10;

  // Inicializa o mutex, variavel de condicao e tid_sistema
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * N);
  if (tid_sistema == NULL) {
    fprintf(stderr, "--ERRO: malloc\n");
    return 2;
  }

  // Cria as threads
  for (long int i=0; i<N; i++) {
    if (pthread_create(tid_sistema+i, NULL, somaElementos, (void *) i)) {
      fprintf(stderr, "--ERRO: pthread_create\n");
      return 3;
    }
  }

  // Aguardar o termino das threads
  for (int i=0; i<N; i++) {
    if (pthread_join(*(tid_sistema+i), (void**) &retorno)) {
      fprintf(stderr, "--ERRO: pthread_join\n");
      return 3;
    }

    somaGlobal[i] = *retorno;
  }

  puts("");

  // Verificar corretude
  printf("Somas realizadas pelas threads:\n");
  for (int i=1; i<N; i++) {
    printf("Thread %d calculou %d\n", i, somaGlobal[i]);

    // Verifica se a mesma soma foi encontrada por todas as threads
    if (somaGlobal[i - 1] != somaGlobal[i]) {
      printf("--ERRO: Divergencia no indice %d: %d != %d", i, somaGlobal[i - 1], somaGlobal[i]);
      return 4;
    }
  }
  printf("Threads realizaram a soma corretamente.\n");

  // Desaloca variaveis e termina
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  free(vetor);
  free(retorno);
  free(somaGlobal);

  return 0;
}
