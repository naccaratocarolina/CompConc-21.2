/* Disciplina: Computacao Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratorio: 8 */
/* Codigo: Programa concorrente em Java que aplica o padrao leitores/escritores */
/* A aplicacao tera como elemento central uma variavel inteira que sera lida e alterada pelas threads */

/**
 * Monitor
 * Implementa a logica do padrao leitores/escritores
 */
class Monitor {
    private int leitores, escritores;

    /**
     * Construtor de Monitor
     */
    Monitor () {
        this.leitores = 0; // leitores lendo (0 ou mais)
        this.escritores = 0; // escritor escrevendo (0 ou 1)
    }

    /**
     * Entrada para leitores
     * @param id identificador da thread
     */
    public synchronized void EntraLeitor (int id) {
        try {
            while (this.escritores > 0) {
                System.out.println ("le.leitorBloqueado(" + id + ")");
                wait();  // bloqueia pela condicao logica da aplicacao
            }
            this.leitores++;  // registra que ha mais um leitor lendo
            System.out.println ("le.leitorLendo(" + id + ")");
        }

        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * Saida para leitores
     * @param id identificador da thread
     */
    public synchronized void SaiLeitor (int id) {
        this.leitores--; // registra que um leitor saiu

        if (this.leitores == 0)
            this.notify(); // libera escritor (caso exista escritor bloqueado)
        System.out.println ("le.leitorSaindo(" + id + ")");
    }

    /**
     * Entrada para escritores
     * @param id identificador da thread
     */
    public synchronized void EntraEscritor (int id) {
        try {
            while ((this.leitores > 0) || (this.escritores > 0)) {
                System.out.println ("le.escritorBloqueado(" + id + ")");
                wait();  // bloqueia pela condicao logica da aplicacao
            }
            this.escritores++; // registra que ha um escritor escrevendo
            System.out.println ("le.escritorEscrevendo(" + id + ")");
        }

        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * Saida para escritores
     * @param id identificador da thread
     */
    public synchronized void SaiEscritor (int id) {
        this.escritores--; // registra que o escritor saiu
        notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println ("le.escritorSaindo(" + id + ")");
    }
}

/**
 * Thread Leitora
 * Le a variavel compartilhada e imprime na tela indicando
 * se eh um valor par ou impar
 */
class Leitora extends Thread {
    private int id_thread;
    private Monitor monitor;

    /**
     * Construtor de Leitora
     * @param id_thread identificador da thread
     * @param monitor instancia de Monitor para coordenar as threads
     */
    Leitora (int id_thread, Monitor monitor) {
        this.id_thread = id_thread;
        this.monitor = monitor;
    }

    /**
     * Verifica se a variavel global compartilhada entre as threads
     * eh par ou impar.
     * @return true se for par; false, c.c.
     */
    private static boolean ehPar () {
        return Principal.varGlobal % 2 == 0;
    }

    /**
     * Handler das threads Escritoras
     * Imprime na tela indicando se a variavel compartilhada eh par ou impar
     */
    @Override
    public void run () {
        this.monitor.EntraLeitor(this.id_thread);

        if (ehPar())
            System.out.println("Thread Leitora #" + this.id_thread + " leu " + Principal.varGlobal + ": é par");
        else
            System.out.println("Thread Leitora #" + this.id_thread + " leu " + Principal.varGlobal + ": é impar");

        this.monitor.SaiLeitor(this.id_thread);
    }
}

/**
 * Thread Escritora
 * Modifica a variavel compartilhada escrevendo o valor do
 * seu identificador de thread
 */
class Escritora extends Thread {
    private int id_thread;
    private Monitor monitor;

    /**
     * Construtor de Escritora
     * @param id_thread identificador da thread
     * @param monitor instancia de Monitor para coordenar as threads
     */
    Escritora (int id_thread, Monitor monitor) {
        this.id_thread = id_thread;
        this.monitor = monitor;
    }

    /**
     * Handler das threads Escritoras
     * Modifica a variavel compartilhada escrevendo na mesma
     * o valor do seu identificador de thread
     */
    @Override
    public void run () {
        monitor.EntraEscritor(this.id_thread);

        Principal.varGlobal = this.id_thread;
        System.out.println("Thread Escritora #" + this.id_thread + " escreveu " + Principal.varGlobal);

        monitor.SaiEscritor(this.id_thread);
    }
}

/**
 * Thread Leitora/Escritora
 * Le a variavel compartilhada e a imprime na tela; Em seguida,
 * faz um processamento bobo qualquer, e depois modifica a
 * variavel, a incrementando de 1
 */
class LeitoraEscritora extends Thread {
    private int id_thread;
    private Monitor monitor;

    /**
     * Construtor de LeitoraEscritora
     * @param id_thread identificador da thread
     * @param monitor instancia de Monitor para coordenar as threads
     */
    LeitoraEscritora (int id_thread, Monitor monitor) {
        this.id_thread = id_thread;
        this.monitor = monitor;
    }

    /**
     * Conta os divisores da variavel compartilhada
     */
    private static int contaDivisores () {
        int cnt = 0;

        for (int i=1; i<=Principal.varGlobal; i++)
            if (Principal.varGlobal % i == 0)
                cnt++;

        return cnt;
    }

    /**
     * Handler das threads Leitoras/Escritoras
     * Imprime a variavel compartilhada na tela;
     * Faz processamento bobo qualquer;
     * Incrementa a variavel compartilhada de 1.
     */
    @Override
    public void run () {
        // --Leitura
        this.monitor.EntraLeitor(this.id_thread);

        // Imprime variavel compartilhada
        System.out.println("Thread Leitora/Escritora #" + this.id_thread + " leu " + Principal.varGlobal);

        // Faz processamento bobo qualquer
        System.out.println(Principal.varGlobal + " possui " + contaDivisores() + " divisores");

        this.monitor.SaiLeitor(this.id_thread);

        // --Escrita
        this.monitor.EntraEscritor(this.id_thread);

        // Incrementa a variavel compartilhada de 1
        Principal.varGlobal++;
        System.out.println("Thread Leitora/Escritora #" + this.id_thread + " escreveu " + Principal.varGlobal);

        this.monitor.SaiEscritor(this.id_thread);
    }
}

/**
 * Classe Principal
 */
public class Principal {
    public static int varGlobal = 0;
    public static final int NUM_THREADS_L = 3;
    public static final int NUM_THREADS_E = 3;
    public static final int NUM_THREADS_LE = 3;

    public static void main (String[] args) {
        Monitor monitor = new Monitor();
        Escritora[] E = new Escritora[NUM_THREADS_E];
        Leitora[] L = new Leitora[NUM_THREADS_L];
        LeitoraEscritora[] LE = new LeitoraEscritora[NUM_THREADS_LE];

        // Cria as threads escritoras
        for (int i=0; i<NUM_THREADS_E; i++)
            E[i] = new Escritora(i, monitor);

        // Cria as threads leitoras
        for (int i=0; i<NUM_THREADS_L; i++)
            L[i] = new Leitora(i, monitor);

        // Cria as threads leitoras/escritoras
        for (int i=0; i<NUM_THREADS_LE; i++)
            LE[i] = new LeitoraEscritora(i, monitor);

        // Inicia as threads
        for (Escritora escritora : E) escritora.start();
        for (Leitora leitora : L) leitora.start();
        for (LeitoraEscritora leitoraEscritora : LE) leitoraEscritora.start();

        // Aguarda o termino das threads
        for (Escritora escritora : E)
            try { escritora.join(); } catch (InterruptedException e) { return; };

        for (Leitora leitora : L)
            try { leitora.join(); } catch (InterruptedException e) { return; };

        for (LeitoraEscritora leitoraEscritora : LE)
            try { leitoraEscritora.join(); } catch (InterruptedException e) { return; };
    }
}
