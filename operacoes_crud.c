#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define ITERATIONS 900

typedef struct node{
    struct node * anterior;
    int dado;
    struct node * proximo;
} node;

node* criarNo(int valor){
    node * Novo = (node *) malloc (sizeof(node));
    Novo->dado = valor;
    Novo->anterior = NULL;
    Novo->proximo=NULL;
    return Novo;
}

// void adicionarCabeca(int valor, node ** cabeca){    
//     node * Novo = criarNo(valor); 
//      // se a lista for vazia
//     if (*cabeca == NULL) {
//         *cabeca = Novo;
//         return;
//     }
//     Novo->proximo = *cabeca;
//     (*cabeca)->anterior = Novo;
//     *cabeca = Novo;
// }

void adicionarCauda(int valor, node **cabeca){
    node * Novo = criarNo(valor);
    // se a lista for vazia
    if (*cabeca == NULL) {
        *cabeca = Novo;
        return;
    }
    node* tmp = *cabeca;
    while (tmp->proximo != NULL) {
        tmp = tmp->proximo;
    }
    tmp->proximo = Novo;
    Novo->anterior = tmp;
    return;
}

void removerValor(int valor, node ** cabeca){
    
  if(*cabeca==NULL){ // se lista vazia
      printf("LISTA JA VAZIA, NAO TEM O QUE REMOVER");
      return;
  }

  node* tmp = *cabeca;

  if (tmp->dado == valor) { // percorre a lista
    if(tmp->anterior==NULL){ // se for o comeco
      *cabeca = tmp->proximo;
      free(tmp);
      return;
    }
    if(tmp->proximo==NULL){ // se for o fim
      tmp->anterior=NULL;
      free(tmp);
      return;
    }
    tmp->proximo->anterior = tmp->anterior;
    tmp->anterior->proximo = tmp->proximo;
    free(tmp);
  }
  
  tmp=tmp->proximo;
  if (tmp==NULL){
    printf("VALOR INEXISTENTE");
    return;
  }
}

void editarValor(int valor, int valornovo, node ** cabeca){
  if (*cabeca==NULL){
      printf("LISTA VAZIA, NAO TEM O QUE EDITAR");
      return;
  }

  //percorrer lista
  node* tmp = *cabeca;

  if (tmp->dado == valor) { // percorre a lista, se encontrar o valor 
    tmp->dado = valornovo;
  }

  tmp=tmp->proximo;

  if (tmp==NULL){
    printf("VALOR INEXISTENTE");
    return;
  }
}

void apagarLista(node **cabeca){
  if (*cabeca==NULL){
      printf("LISTA VAZIA, NAO TEM O QUE APAGAR");
      return;
  }
  node* tmp = *cabeca;

  while(tmp->proximo->proximo != NULL){
    tmp = tmp->proximo;
    free(tmp);
  }

}

//==========================================================MAIN===========================================================//

int main(){
  int opcao, valor;
  int vezes = ITERATIONS;
  node* cabeca = NULL;

  for(int i = 0; i<420; i++){ //adiciona dez mil elementos iniciais à lista
    valor = rand() % 1000000;
    adicionarCauda(valor, &cabeca);
  }

  while(vezes>0){ //executa n vezes
    opcao = rand() % 3; // decide por uma opção aleatória
    valor = rand() % 1000000; //gera valor aleatório 

    switch(opcao){
      case 0:
        adicionarCauda(valor, &cabeca);
        break;
      case 1:
        removerValor(valor, &cabeca);
        break;
      case 2:
        editarValor(valor, rand() % 1000000, &cabeca);
        break;
    }

    vezes--;
  }

  FILE *fptr; //Ponteiro para arquivo

  // Cria arquivo ou abre existente
  fptr = fopen("Lista.txt", "a");

  if (fptr == NULL) {
      printf("Erro de criacao!");
      exit(1);
  } 

  // Escreve a lista no arquivo
  for (node *tmp = cabeca; tmp != NULL; tmp = tmp->proximo) {
      fprintf(fptr, "%d, ", tmp->dado);
  }

  apagarLista(&cabeca);


}
