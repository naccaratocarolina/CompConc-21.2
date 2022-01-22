/**
 * No caso de nthreads = 2, so teremos que executar a
 * etapa do merge uma unica vez.
 *
 * No caso de nthreads = 4, teremos que executar a
 * etapa do merge 2 vezes. Para o primeiro merge,
 * usaremos 2 iteracoes, onde cada iteracao combina 2
 * pares de subvetores. Apos esse passo, teremos 2
 * subvetores ordenados. Ja o segundo merge usa 1
 * iteracao e combina 2 subvetores.
 *
 * No caso de nthreads = 8, teremos que executar a
 * etapa do merge 3 vezes. Para o primeiro merge,
 * usaremos 4 iteracoes, onde cada iteracao combina
 * 2 pares de subvetores. Apos esse passo teremos 4
 * subvetores ordenados. No segundo merge, usaremos
 * 2 iteracoes e cada iteracao combina 2 pares de
 * subvetores. Apos esse passo, teremos 2 subvetores
 * ordenados. No ultimo merge, usaremos apenas uma
 * iteracao que combina 2 pares de subvetores.
 *
 * Esta funcao generaliza os sqrt(nthread) merges que
 * deverao ser feitos apos a chamada da funcao
 * p_merge_sort com nthreads threads.
 *
 * @param a vetor de entrada
 * @param dim dimensao do vetor de entrada
 * @param nthreads numero de threads
 */
void p_merge (int a[], long long dim, int nthreads);

/**
 * A implementacao do merge sort concorrente segue
 * a mesma logica do sequencial. A diferenca eh que
 * iremos particionar o vetor de entrada em nthreads
 * pedacos, de forma que cada thread computa a
 * ordenacao de um subvetor do vetor de entrada.
 *
 * @param arg ponteiro para void
 * @return ponteiro para void
 */
void * p_merge_sort (void * arg);

/**
 * Funcao responsavel por de fato implementar a solucao
 * concorrente. Cria uma variavel do tipo pthread_t que
 * ira armazenar os identificadores das threads no sistema.
 * Dispara nthreads que, por sua vez, irao executar a funcao
 * p_merge_sort. Apos aguardar o termino da ordenacao dos
 * subvetores realizada por p_merge_sort, combina os subvetores
 * gerados na etapa anterior para fornecer a resposta correta.
 *
 * @param a vetor de entrada
 * @param dim dimensao do vetor de entrada
 * @param nthreads numero de threads
 */
void p_ordena (int a[], long long int dim, int nthreads);

/**
 * Garante que o numero de threads eh uma potencia de 2.
 * @param nthreads numero de threads
 * @return 1, se nthreads for potencia de 2; 0, c.c.
 */
int ehPotenciaDeDois (int nthreads);