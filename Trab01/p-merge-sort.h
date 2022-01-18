/**
 * A implementacao do merge sort concorrente segue
 * a mesma logica do sequencial. A diferenca eh que,
 * a cada chamada recursiva a p_merge_sort, disparamos
 * duas threads, cada uma com uma metade do vetor de
 * entrada.
 *
 * @param arg ponteiro para void
 * @return ponteiro para void
 */
void * p_merge_sort (void * arg);

/**
 * Se nthreads = 2, so temos que realizar a etapa do
 * p_merge_sort uma unica vez. Sendo assim, disparamos
 * apenas uma thread, que recebe como parametro os indices
 * inicial e final do vetor a ser ordenado.
 *
 * @param a vetor de entrada
 * @param dim dimensao do vetor de entrada
 */
void t2 (int a[], long long int dim);

/**
 * Se nthreads = 4, temos que realizar a etapa do
 * p_merge_sort 2 vezes: O primeiro merge usa 2 threads
 * e cada thread combina dois pares do vetor de entrada.
 * Apos esta etapa, teremos 2 pedacos ordenados maiores
 * do vetor de entrada. Em seguida, combinamos esses 2
 * pedacos usando a funcao merge.
 *
 * @param a vetor de entrada
 * @param dim dimensao do vetor de entrada
 */
void t4 (int a[], long long int dim) ;