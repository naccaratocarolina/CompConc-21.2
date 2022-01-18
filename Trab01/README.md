# Merge Sort Multithread
Esta aplicacao implementa o algoritmo do Merge Sort na versao concorrente.

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
gcc -Wall main.c merge-sort.c p-merge-sort.c -o main -lpthread
```

## 4. Rode o executavel
```
./main <dimensao do vetor> <numero de threads>
```