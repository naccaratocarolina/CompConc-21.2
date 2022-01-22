# Merge Sort Multithread
Esta aplicação implementa o algoritmo do Merge Sort na versão concorrente. A ideia central é particionar o vetor de entrada em ```nthreads``` pedaços, de forma que cada thread fica responsável por ordenar uma parcela diferente do vetor. Como as threads não dividem as fronteiras dos subvetores a serem ordenados, não é necessário se preocupar com condição de corrida.

![Badge](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

## Setup
### 1. Clone o repositorio
```
https://github.com/naccaratocarolina/CompConc-21.2
```

## 2. Navegue para a pasta ```/Trab1```
```
cd /Trab1
```

## 3. Compile o arquivo principal
```
gcc -Wall main.c merge-sort.c p-merge-sort.c -o main -lpthread -lm
```

## 4. Rode o executavel
```
./main <dimensao do vetor> <numero de threads>
```