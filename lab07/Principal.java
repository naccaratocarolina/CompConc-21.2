/* Disciplina: Computacao Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratorio: 7 */
/* Codigo: Programa concorrente em Java para somar todos os elementos de um vetor de inteiros */

import java.util.Random;

class Soma {
    private int soma; // variavel acumuladora compartilhada com todas as threads

    /**
     * Construtor de Soma
     */
    public Soma () {
        this.soma = 0;
    }

    /**
     * Getter de soma
     * @return soma dos elementos de um vetor
     */
    public synchronized int getSoma () {
        return this.soma;
    }

    /**
     * Incrementa a variavel soma com o inteiro fornecido
     * @param i inteiro a ser somado
     */
    public synchronized void setSoma (int i) {
        this.soma += i;
    }
}

class SomaInteiros extends Thread {
    private int id_thread;
    private int nthreads;
    private int[] vetor;
    private Soma soma;

    /**
     * Construtor de SomaInteiros
     * @param id_thread Identificador da thread
     * @param nthreads Total de threads a serem criadas
     * @param vetor Vetor de inteiros
     * @param soma Var acumuladora compartilhada com todas as threads
     */
    public SomaInteiros(int id_thread, int nthreads, int[] vetor, Soma soma) {
        this.id_thread = id_thread;
        this.nthreads = nthreads;
        this.vetor = vetor;
        this.soma = soma;
    }

    /**
     * Handler das threads
     */
    public void run () {
        for (int i = this.id_thread; i<this.vetor.length; i+=this.nthreads)
            this.soma.setSoma(this.vetor[i]);
    }
}

public class Principal {
    static final int TAMANHO = 10000; // Tamanho do vetor
    static final int NUM_THREADS = 10; // Numero de threads

    /**
     * Funcao que verifica se a soma dos elementos de um vetor
     * foi computada corretamente pelas threads
     * @param soma soma concorrente
     * @param vetor vetor de inteiros
     * @return true, se a soma foi calculada corretamente; false, c.c.
     */
    private static boolean verificaResultado(Soma soma, int[] vetor) {
        int somaSeq = 0;
        int somaConc = soma.getSoma();

        for (int i : vetor) somaSeq += i;

        return somaSeq == somaConc;
    }

    public static void main (String[] args) {
        int[] vetor = new int[TAMANHO];
        Thread[] threads = new Thread[NUM_THREADS];

        // Preenche vetor de entrada com valores aleatorios do tipo int
        Random random = new Random();
        for (int i=0; i<TAMANHO; i++)
            // Gera numeros aleatorios entre 1 e 1000
            vetor[i] = 1 + random.nextInt(1000);

        // Cria uma nova instancia do recurso compartilhado
        Soma soma = new Soma();

        // Cria as threads
        for (int i=0; i<threads.length; i++)
            threads[i] = new SomaInteiros(i, NUM_THREADS, vetor, soma);

        // Inicia as threads
        for (Thread thread : threads) thread.start();

        // Espera fim das threads
        for (Thread thread : threads)
            try { thread.join(); } catch (InterruptedException e) { return; };

        // Verifica corretude
        if (verificaResultado(soma, vetor)) System.out.println("Soma foi calculada corretamente");
        else System.out.println("--ERRO: Soma nao foi calculada corretamente");
    }
}
