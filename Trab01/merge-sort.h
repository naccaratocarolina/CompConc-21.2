/**
 * Combina duas subsequencias ordenadas para produzir
 * o vetor final ordenado (ini <= meio < fim).
 *
 * @param a vetor de entrada
 * @param ini indice inicial do vetor de entrada
 * @param meio indice central do vetor de entrada
 * @param fim indice final do vetor de entrada
 */
void merge (int a[], long long int ini, long long int meio, long long int fim);

/**
 * Realiza chamadas recursivas a si mesma, criando
 * subproblemas da mesma instancia. A cada recursao,
 * divide o vetor em duas partes iguais e as ordena.
 * Em seguida, chama a funcao merge para combinar as
 * sequencias ordenadas, dois a dois, ate que o vetor
 * de entrada esteja ordenado por completo.
 *
 * @param a vetor de entrada
 * @param ini indice inicial do vetor de entrada
 * @param fim indice final do vetor de entrada
 */
void merge_sort (int a[], long long int ini, long long int fim);

/**
 * Imprime na tela o vetor fornecido como parametro.
 *
 * @param a vetor de entrada
 * @param dim dimensao do vetor de entrada
 */
void imprime (int a[], long long int dim);

/**
 * Verifica corretude da ordenacao, se assegurando
 * que o vetor de entrada esta em ordem crescente.
 *
 * @param a vetor de entrada
 * @param dim dimensao do vetor de entrada
 */
void verificaCorretude (int a[], long long int dim);

/**
 * Preenche o vetor com numeros aleatorios.
 *
 * @param a
 * @param dim
 */
void preenche (int a[], long long int dim);