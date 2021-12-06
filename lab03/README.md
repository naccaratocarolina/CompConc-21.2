# Laboratório 03 | Relatório
Análise de desempenho e cálculo da aceleração do programa concorrente que encontra a quantidade de valores de um vetor de numeros reais que estao dentro de uma 
faixa de valores dada.

## Índice
* [Informações sobre o hardware](#informações-sobre-o-hardware)
* [Como rodar o programa](#como-rodar-o-programa)
* [Tempos de execução](#tempos-de-execução)
* [Cálculo da aceleração](#cálculo-da-aceleração)

## Informações sobre o hardware
- Distro: Manjaro Linux 21.1.6
- Processador: AMD Ryzen 7 2700U with Radeon Vega Mobile Gfx
- Memória: 8GB RAM

## Como rodar o programa
:thinking: Compila e executa o programa passando como argumentos uma matriz de dimensao 10<sup>3</sup> com 1 thread e no intervalo 100 < x<sub>i</sub> < 500
```
gcc -Wall quant-elementos_intervalo.c -o quantElementosIntervalo -lpthread && ./quantElementosIntervalo 1000 1 100 500
```

## Tempos de execução
:warning: Ao realizar testes para verificar o tempo de execução do programa com os inputs dados, percebi que o tempo sofre pequenas alterações de acordo com o intervalo dado.
Por conta disso, todos os testes foram realizados usando como base o intervalo 100 < x<sub>i</sub> < 500.

### Dimensão 10<sup>5</sup>
| Solução | Tempo de execução |
| --- | --- |
| **Sequencial** | 0.000950s |                                                                                                                                                                                 
| **2 Threads** | 0.001215s |
| **4 Threads** | 0.000823s |

### Dimensão 10<sup>7</sup>
| Solução | Tempo de execução |
| --- | --- |
| **Sequencial** | 0.081077s |                                                                                                                                                                                 
| **2 Threads** | 0.071250s |
| **4 Threads** | 0.048729s |

### Dimensão 10<sup>8</sup>
| Solução | Tempo de execução |
| --- | --- |
| **Sequencial** | 0.819116s |                                                                                                                                                                                 
| **2 Threads** | 0.472208s |
| **4 Threads** | 0.281909s |

## Cálculo da aceleração
:abacus: Para realizar o cálculo da aceleração e poder estimar o ganho de desempenho, usaremos a Lei de Amdahl. O ganho de velocidade da execução é dado por: 
T<sub>sequencial</sub> / t<sub>s</sub> + t<sub>s</sub>, onde
- T<sub>sequencial</sub>: Tempo total de execução do programa sequencial
- t<sub>s</sub>: Tempo da parte sequencial do programa
- t<sub>s</sub>: Tempo da parte concorrente do programa usando P processadores

### Dimensão 10<sup>5</sup>
 | Threads | Aceleração |
 | --- | --- |
 | **2 Threads** | 1,14s |
 | **4 Threads** | 1,16s |
 
### Dimensão 10<sup>7</sup>
 | Threads | Aceleração |
 | --- | --- |
 | **2 Threads** | 1,45s |
 | **4 Threads** | 1,19s |
 
 ### Dimensão 10<sup>8</sup>
 | Threads | Aceleração |
 | --- | --- |
 | **2 Threads** | 1,12s |
 | **4 Threads** | 1,11s |
