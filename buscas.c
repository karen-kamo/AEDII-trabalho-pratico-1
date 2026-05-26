/*
Karen Nanamy Kamo - NUSP: 15495932 
Rebeca de Oliveira Silva - NUSP: 11963923
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "structs.h"
#include "ler_arq.h"
#include "escrever_arq.h"
#include "funcionalidades.h"
#include "uteis.h"

/* ================================================================================

******************************FUNCIONALIDADE 2************************************

===================================================================================*/ 

/* Funcionalidades: 

  1. Abrir o arquivo.bin para leitura
  2. Ler o arquivo até o final, registro por registro (Linha por linha)
  3. Caso o registro exista, deve imprimir campo a campo

*/

void buscar_todos_reg_bin(){
  char nomeArqBin[100];
  scanf("%s", nomeArqBin); // leitura do input do usuário

  // abrir arquivo binário para leitura
  FILE *arqBin = fopen(nomeArqBin, "rb");
  if (arqBin == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  RegistroCabecalho *h = ler_reg_cab_bin(arqBin);
  // dá erro caso o ponteiro seja NULL ou o arquivo esteja inconsistente
  if (h == NULL || h->status == '0') {
    printf("Falha no processamento do arquivo.\n");
    fclose(arqBin);
    return;
  }

  //Enquanto não terminar o arquivo.bin, continue lendo os valores dos registros de dados
  while (check_eof(arqBin)){
    RegistroDado *r = ler_reg_dado_bin(arqBin);

    // verificando se o registro está logicamente removido
    // apenas ignora o removido e continua
    if (r == NULL){
      continue;
    } 
    
    imprimir_reg_dado(r);
    
    free_reg_dado(r);
    free(r);
  }

  free_reg_cab(h);
  fclose(arqBin);
}

//===============================================================================================================================//


// Função para buscar o Registro de Cabeçalho
//Não precisava para o trabalho 1
//Usado para debugar o código pelas estudantes 
void buscar_reg_cab_bin(){
  char nomeArqBin[100];
  scanf("%s", nomeArqBin);

  // abrir arquivo binário para leitura
  FILE *arqBin = fopen(nomeArqBin, "rb");
  if (arqBin == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  // mover cursor para o início do arquivo
  fseek(arqBin, 0, SEEK_SET);

  RegistroCabecalho *h = ler_reg_cab_bin(arqBin);
  if (h == NULL) printf("Falha no processamento do arquivo.\n");
  imprimir_reg_cab(h);

  free_reg_cab(h);
  fclose(arqBin);
}

/* ================================================================================

******************************FUNCIONALIDADE 3************************************

===================================================================================*/ 

/* Funcionalidades: 

  1. Abrir o arquivo.bin para leitura
  2. Quantidade de buscas pelo usuário
  3. Quantidade de pares de busca pelo usuário 
  4. Entrada dos valores desejadas pelo usuário
  5. Compara os valores fornecido pelo usuário com os resgistros de dados
  6. Imprime, caso exista
*/
void buscar_reg_filtro(){
  char nomeArqBin[100];
  int quantBusca; 
  int quantPar; // armazena quantos pares o usuário quer
  
  // arrays para guardar os dados que devem ser filtrados
  char nomesCampos[100][100];
  char valoresCampos[100][100];

  scanf("%s %d", nomeArqBin, &quantBusca); // leitura dos inputs do usuário

  // abrir arquivo binário para leitura
  FILE *arqBin = fopen(nomeArqBin, "rb");
  if (arqBin == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }

  RegistroCabecalho *h = ler_reg_cab_bin(arqBin);
  // dá erro caso o ponteiro seja NULL ou o arquivo esteja inconsistente
  if (h == NULL || h->status == '0') {
    printf("Falha no processamento do arquivo.\n");
    fclose(arqBin);
    return;
  }

  // loop para buscar de acordo com a quantidade desejada
  for (int i = 0; i < quantBusca; i++){
    scanf("%d", &quantPar); // input de quantos pares vai desejar buscar

    // loop para a quantidade de pares desejadas
    for (int j = 0; j < quantPar; j++){
      scanf("%s", nomesCampos[j]); // lê o nome do campo desajado

      if (strcmp(nomesCampos[j], "nomeEstacao") == 0 ||
          strcmp(nomesCampos[j], "nomeLinha") == 0
      ){
        ScanQuoteString(valoresCampos[j]); // se for string, para ler "" 
      } else {
        scanf("%s", valoresCampos[j]);  // se for int, lê normalmente
      }

    }

    // mover para o byte 17 para buscar nos dados
    fseek(arqBin, 17, SEEK_SET);

    int naoEhInexistente = 0; // para ver se o registro foi printado ou não

    // fazer busca até o final do arquivo
    while (check_eof(arqBin)){
      RegistroDado *r = ler_reg_dado_bin(arqBin);

      // verificando se o registro está logicamente removido
      // se for removido, apenas ignora e continua
      if (r == NULL){
        continue;
      }
      
      int achou = 1; // para verificar se bate com todos os dados especificados, se não mudar, achou

      //Verificação se existe valor para cada campo.
      //Se achou = 1, corresponde que os valores buscados foram encontrados.
      // loop para a quantidade de pares que estão armazenados nas arrays
      for (int b = 0; b < quantPar; b++){

        
        if (strcmp(nomesCampos[b], "codEstacao") == 0){ // se o valor da array corresponde ao nome do campo
          if (verificar_nulo(valoresCampos[b]) != r->codEstacao) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }

        // nomeEstacao nçao pode ser nulo
        else if (strcmp(nomesCampos[b], "nomeEstacao") == 0){ // se o valor da array corresponde ao nome do campo
          if (strcmp(valoresCampos[b], r->nomeEstacao) != 0) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }

        else if (strcmp(nomesCampos[b], "codLinha") == 0){ // se o valor da array corresponde ao nome do campo
          if (verificar_nulo(valoresCampos[b]) != r->codLinha) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }

        else if (strcmp(nomesCampos[b], "nomeLinha") == 0){ // se o valor da array corresponde ao nome do campo
          // se usuário buscou por NULO
          if (strcmp(valoresCampos[b], "") == 0){
            // se o registro tem nome, não serve pois é NULO a busca
            if (r->nomeLinha != NULL) achou = 0;
          }
          // se buscou por um nome real
          else {
            if (r->nomeLinha == NULL || strcmp(valoresCampos[b], r->nomeLinha) != 0) // se o valor da array  não corresponde ao valor armazenado na struct ou não possui valor
              achou = 0;
          }
        }

        else if (strcmp(nomesCampos[b], "codProxEstacao") == 0){ // se o valor da array corresponde ao nome do campo
          if (verificar_nulo(valoresCampos[b]) != r->codProxEstacao) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }

        else if (strcmp(nomesCampos[b], "distProxEstacao") == 0){ // se o valor da array corresponde ao nome do campo
          if (verificar_nulo(valoresCampos[b]) != r->distProxEstacao) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }

        else if (strcmp(nomesCampos[b], "codLinhaIntegra") == 0){ // se o valor da array corresponde ao nome do campo
          if (verificar_nulo(valoresCampos[b]) != r->codLinhaIntegra) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }
          
        else if (strcmp(nomesCampos[b], "codEstIntegra") == 0){ // se o valor da array corresponde ao nome do campo
          if (verificar_nulo(valoresCampos[b]) != r->codEstIntegra) achou = 0; // se o valor da array  não corresponde ao valor armazenado na struct
        }

        // se já não atende a algum filtro, já para
        if (!achou) break;
      }

      if (achou){
        imprimir_reg_dado(r);
        naoEhInexistente = 1;
      }

      free_reg_dado(r);
      free(r);
    }

    if (!naoEhInexistente) printf("Registro inexistente.\n");

    // pular linha entre as buscas, menos no final
    if (i < quantBusca - 1) printf("\n");
  }
  
  free_reg_cab(h);
  fclose(arqBin);
}


/* ================================================================================

******************************FUNCIONALIDADE 4************************************

===================================================================================*/ 

/* Funcionalidades: 

  1.Abrir o arquivo.bin para leitura
  2. Calcula o byteoffset apartir do RRN de entrada
  3. Move o ponteiro para a posição desejada
  4. Tratamento de casos de registros removidos

*/
void buscar_reg_RRN(){
  char nomeArqBin[100];
  int rrn;

  // entrada dos dados desejados
  scanf("%s %d", nomeArqBin, &rrn);

  // abrir arquivo bin para leitura
  FILE *arqBin = fopen(nomeArqBin, "rb");
  if (arqBin == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return;
  }
  
  RegistroCabecalho *h = ler_reg_cab_bin(arqBin);
  // dá erro caso o ponteiro seja NULL ou o arquivo esteja inconsistente
  if (h == NULL || h->status == '0') {
    printf("Falha no processamento do arquivo.\n");
    fclose(arqBin);
    return;
  }
  
  // verificando se esse RRN existe nos registros
  if (rrn >= h->proxRRN || rrn < 0) {
    printf("Registro inexistente.\n");
    fclose(arqBin);
    return;
  }

  // calculando o byte que deve ser buscado
  int byteOffSet = 80 * rrn + 17;
  fseek(arqBin, byteOffSet, SEEK_SET);

  RegistroDado *r = ler_reg_dado_bin(arqBin);

  // verificando se o registro está logicamente removido
  if (r == NULL || r->removido == '1'){
    printf("Registro inexistente.\n");
  } else {
    imprimir_reg_dado(r);
  }

  free_reg_dado(r);
  free(r);
  free_reg_cab(h);

  fclose(arqBin);
}