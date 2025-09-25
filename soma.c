#include <stdio.h> 
#include <stdlib.h> 

#define N 2000
#define BLOCO 500 

int main(){ 
    int *vet = malloc(N * sizeof(int)); 
    if(!vet){
        printf("Erro ao alocar memoria\n"); 
        return 1; 
    } 
    for(int i = 0; i<N; i++){
        vet[i] = i;
    } 

    long soma = 0; 
    for(int rep = 0; rep < 200; rep++){
        for(int i = 0; i < BLOCO; i++){
            soma += vet[i];
        }
    } 

    printf("Soma = %ld\n", soma); 
    free(vet); 
    return 0;
} 

