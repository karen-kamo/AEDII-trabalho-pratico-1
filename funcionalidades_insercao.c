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

******************************FUNCIONALIDADE 8************************************

===================================================================================*/ 
void inserir_reg() {
   char nomeArqDados[100];
   char nomeArqIndice[100];
   int nInsercao; // quant de inserções que serão feitas

   scanf("%s %s %d", nomeArqDados, nomeArqIndice, &nInsercao);

   // 1. Abrir o arquivo binário para escrita e leitura
   FILE *arqBin = NULL; // inicializa o ponteiro
   RegistroCabecalho *h = abrir_e_validar_arq_bin(nomeArqDados, &arqBin, "rb+"); // para leitura e escrita
   if (h == NULL) return; // se der errado, só para

   // 2. Carrega o índice para a RAM - fiz primeiro, pois abre e fecha arq na função de carregar
   int nRegistrosIndice = 0; // para guardar quant de regs
   RegistroDadoIndice *listaIndice = carregar_indice_na_memoria(nomeArqIndice, &nRegistrosIndice);
   if (listaIndice == NULL){ // se deu errado
      free_reg_cab(h);
      fclose(arqBin);
      return; // quer dizer que não tem os regs 
   }

   // 3. Abrir o arquivo de índice para escrita e leitura
   FILE *arqInd = NULL; // inicializa o ponteiro
   RegistroCabecalhoIndice *hInd = abrir_e_validar_ind(nomeArqIndice, &arqInd, "rb+"); // para leitura e escrita
   if (hInd == NULL) return; // se der errado, só para

   // 4. Mudar status dos arquivos
   h->status = '0';
   escreve_reg_cab_bin(arqBin, h);

   hInd->status = '0';
   escreve_reg_cab_ind(arqInd, hInd);

   char buffer[1000]; // para guardar os strings digitados

   // loop de inserções
   for (int i = 0; i < nInsercao; i++) {
      // 5. Pegar os valores para inserção
      RegistroDado *r = (RegistroDado *) malloc(sizeof(RegistroDado));

      // leitura de dados pelo teclado para cada campo
      // codEstacao - lê para ver se é NULO, se não vai guardar o valor
      scanf("%s", buffer); // se é int, usa scanf
      r->codEstacao = verificar_nulo(buffer);

      // nomeEstacao - se é um valor para campo de tamanho variável usa ScanQuoteString
      ScanQuoteString(buffer);
      if (strcmp(buffer, "NULO") == 0) { // se é NULO
         r->nomeEstacao = NULL;
         r->tamNomeEstacao = 0;
      } else {
         r->nomeEstacao = malloc((strlen(buffer) + 1) * sizeof(char));
         strcpy(r->nomeEstacao, buffer);
         r->tamNomeEstacao = strlen(buffer);
      }

      // codLinha - lê para ver se é NULO, se não vai guardar o valor
      scanf("%s", buffer); // se é int, usa scanf
      r->codLinha = verificar_nulo(buffer);

      // nomeLinha - se é um valor para campo de tamanho variável usa ScanQuoteString
      ScanQuoteString(buffer);
      if (strcmp(buffer, "NULO") == 0) { // se é NULO
         r->nomeLinha = NULL;
         r->tamNomeLinha = 0;
      } else {
         r->nomeLinha = malloc((strlen(buffer) + 1) * sizeof(char));
         strcpy(r->nomeLinha, buffer);
         r->tamNomeLinha = strlen(buffer);
      }

      // codProxEstacao
      scanf("%s", buffer); // se é int, usa scanf
      r->codProxEstacao = verificar_nulo(buffer);

      // distProxEstacao
      scanf("%s", buffer); // se é int, usa scanf
      r->distProxEstacao = verificar_nulo(buffer);

      // codLinhaIntegra
      scanf("%s", buffer); // se é int, usa scanf
      r->codLinhaIntegra = verificar_nulo(buffer);

      // codEstIntegra
      scanf("%s", buffer); // se é int, usa scanf
      r->codEstIntegra = verificar_nulo(buffer);

      // verificar se a estação que estamos apagando era a última ativa com esse nome
      if (!existe_nome_estacao(arqBin, h, r->nomeEstacao)) {
         h->nroEstacoes++; 
      }

      // vrificar se esse par era o último ativo
      if (r->codProxEstacao != -1 && !existe_par(arqBin, h, r->codEstacao, r->codProxEstacao)) {
         h->nroParesEstacoes++;
      }

      // 6. Escrever o registro no arquivo de dados
      int rrnDestino;
      r->removido = '0'; // para o novo registro ficar ativo
      r->proximo = -1; // apontar que é o final
      
      if (h->topo == -1) { // se pilha vazia
         rrnDestino = h->proxRRN; // pega o próximo RRN
         int byteOffset = 17 + (rrnDestino * 80);
         fseek(arqBin, byteOffset, SEEK_SET);
         
         escreve_reg_dado_bin(arqBin, r);
         
         h->proxRRN++; // incrementa o próximo RRN disponível
      } else { //reaproveita espaço dos removidos
         rrnDestino = h->topo; // desempilha a pilha de removidos

         int byteOffset = 17 + (rrnDestino * 80);
         fseek(arqBin, byteOffset, SEEK_SET); // ir p/ posição a inserir

         char removido; // só p/ pular esse 1 byte 
         int proximoPilha; // p/ guardar p/ topo 
         fread(&removido, sizeof(char), 1, arqBin);
         fread(&proximoPilha, sizeof(int), 1, arqBin);
         
         // voltamos ao início do espaço e gravamos o registro novo por cima
         fseek(arqBin, byteOffset, SEEK_SET); 
         escreve_reg_dado_bin(arqBin, r);
         
         h->topo = proximoPilha; // atualizamos o topo da pilha
      }

      // 7. Inserir o novo registro naa lista de índices
      // realocar espaço para cada inserção nova
      listaIndice = realloc(listaIndice, (nRegistrosIndice + 1) * sizeof(RegistroDadoIndice));
      listaIndice[nRegistrosIndice].codEstacao = r->codEstacao;
      listaIndice[nRegistrosIndice].RRN = rrnDestino;
      nRegistrosIndice++;

      free_reg_dado(r);
      free(r);
   }

   // 8. Ordena a lista de índices da RAM
   heap(listaIndice, nRegistrosIndice); 

   // 9. Salvar os dados nos arquivos binários
  
   // salvando o status do arquivo binário
   h->status = '1';
   escreve_reg_cab_bin(arqBin, h);

   // para remover qualquer lixo do arquivo
   fclose(arqInd); // fecha o primeiro fluxo de rb+
   arqInd = fopen(nomeArqIndice, "wb"); // abre de novo para só escrita

   // salvando o status do arquivo de índice
   hInd->status = '1';
   escreve_reg_cab_ind(arqInd, hInd);

   // salvando a lista da RAM
   for (int i = 0; i < nRegistrosIndice; i++){
      if (listaIndice[i].RRN != -1) escreve_reg_dado_ind(arqInd, &listaIndice[i]);
   } 

   // 10. Liberações de memória e fechamento
   free(listaIndice);
   free_reg_cab(h);
   free(hInd);
   fclose(arqBin);
   fclose(arqInd);

   // 11. Rodar o BinarioNaTela
   BinarioNaTela(nomeArqDados);
   BinarioNaTela(nomeArqIndice);
}

/* ================================================================================

******************************FUNCIONALIDADE 9************************************

===================================================================================*/ 

//Procura e muda dados existentes

void atualizar_reg() {
   char nomeArqDados[100];
   char nomeArqIndice[100];
   int n;

   // Passo 1: Leitura inicial
   scanf("%s %s %d", nomeArqDados, nomeArqIndice, &n);

   FILE *arqBin = NULL;
   FILE *arqInd = NULL;

   // Passo 2: Abertura e Validação
   RegistroCabecalho *h = abrir_e_validar_arq_bin(nomeArqDados, &arqBin, "rb+");

   int nRegistrosIndice = 0; // para guardar quant de regs
   RegistroDadoIndice *listaIndice = carregar_indice_na_memoria(nomeArqIndice, &nRegistrosIndice);
   if (listaIndice == NULL){ // se deu errado
   free_reg_cab(h);
   fclose(arqBin);
   return; // quer dizer que não tem os regs 
   }

   RegistroCabecalhoIndice *hInd = abrir_e_validar_ind(nomeArqIndice, &arqInd, "rb+");

   if (h == NULL || hInd == NULL) {
      if (arqBin) fclose(arqBin);
      if (arqInd) fclose(arqInd);
      return;
   }

   // Passo 3: Proteção de Status
   h->status = '0';
   hInd->status = '0';
   
   escreve_reg_cab_bin(arqBin, h);    
   escreve_reg_cab_ind(arqInd, hInd);

   for (int i =0; i < n; i++){
      //qual é o rrn que está a estação filtrada?
      int posSequencial = 17;
      int rrnEncontrado = executar_busca_indexada(listaIndice, nRegistrosIndice, h, arqBin, &posSequencial);

      int quantAlt; //quantidades de alterações que serão executadas
      scanf("%d", &quantAlt); 
      //ex: 1 codEstacao 15
      // quantAlt = 1, muda o código da estação para 15

      //matrizes para armazenar as novas atualizações
      char nomeCampoNew[100][500]; // armazena os campos que foram alterados
      char valorCampoNew[100][500]; //armazena os valores que foram alterados
      //ex: nomeCampoNew = codEstacao e 15 = valorCampoNew

      for (int j=0; j < quantAlt; j++){
         scanf("%s", nomeCampoNew[j]);
         scanf(" \"%[^\"]\"", valorCampoNew[j]);
      }

      //achei o rrn
      //alterar o arquivo binário
      if (rrnEncontrado != -1){
         //calculando a pos do reg de dados
         long byteOffset = 17 + (rrnEncontrado * 80);
         fseek(arqBin, byteOffset, SEEK_SET);

         RegistroDado*r = ler_reg_dado_bin(arqBin);

         //o número de alterações armazenadas será rodada no loop da função
         atualizar_reg_pelo_filtro(r, nomeCampoNew, valorCampoNew, quantAlt, listaIndice, nRegistrosIndice);

         // volta a posição e grava a struct por cima
         // fseek(arqBin, byteOffset, SEEK_SET);
         escreve_reg_dado_bin(arqBin, r); // Escreve no binário

         //liberando na memória
         free_reg_dado(r);
         free(r);
      }
   }

   // ============================================================
   // CORREÇÃO: SALVAR OS ARQUIVOS DE VERDADE ANTES DE FECHAR TUDO
   // ============================================================

   // 1. Garantir que o índice na RAM está perfeitamente ordenado (caso chaves mudaram)
   heap(listaIndice, nRegistrosIndice); 

   // 2. Voltar ao início do arquivo de índice para reescrever
   fseek(arqInd, 0, SEEK_SET);
   
   // Gravar o cabeçalho do índice como consistente ('1')
   hInd->status = '1';
   fwrite(&hInd->status, sizeof(char), 1, arqInd);

   // Gravar a lista de registros atualizada logo após o cabeçalho
   for (int k = 0; k < nRegistrosIndice; k++) {
      fwrite(&listaIndice[k], sizeof(RegistroDadoIndice), 1, arqInd);
   }

   // 3. Voltar ao início do arquivo de dados para salvar o cabeçalho definitivo
   h->status = '1'; // Consistente
   fseek(arqBin, 0, SEEK_SET);
   escreve_reg_cab_bin(arqBin, h); 

   // 4. Agora sim, liberações de memória e fechamento seguros
   free(listaIndice);
   free(h);
   free(hInd);
   fclose(arqBin);
   fclose(arqInd);

   // 5. Chamar o BinarioNaTela para ambos os arquivos
   BinarioNaTela(nomeArqDados); 
   BinarioNaTela(nomeArqIndice); 
}