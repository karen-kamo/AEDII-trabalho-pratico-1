#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "ler_arq.h"
#include "escrever_arq.h"
#include "funcionalidades.h"
#include "uteis.h"

/* ================================================================================

******************************FUNCIONALIDADE 9************************************

===================================================================================*/ 

// Procura e muda dados existentes
void atualizar_reg() {
   char nomeArqDados[100];
   char nomeArqIndice[100];
   char buffer[500];  
   int n;

   // Passo 1: Leitura inicial
   scanf("%s %s %d", nomeArqDados, nomeArqIndice, &n);

   FILE *arqBin = NULL;
   FILE *arqInd = NULL;

   // Passo 2: Abertura e Validação
   RegistroCabecalho *h = abrir_e_validar_arq_bin(nomeArqDados, &arqBin, "rb+");
   if (h == NULL) return; 

   int nRegistrosIndice = 0; 
   RegistroDadoIndice *listaIndice = carregamento(nomeArqIndice, &nRegistrosIndice);
   if (listaIndice == NULL){ 
      free_reg_cab(h);
      fclose(arqBin);
      return; 
   }

   RegistroCabecalhoIndice *hInd = abrir_e_validar_ind(nomeArqIndice, &arqInd, "rb+");
   if (hInd == NULL) {
      free(listaIndice);
      free_reg_cab(h);
      fclose(arqBin);
      return;
   }

   // Passo 3: Proteção de Status
   h->status = '0';
   hInd->status = '0';
   
   escreve_reg_cab_bin(arqBin, h);   
   escreve_reg_cab_ind(arqInd, hInd);

   for (int i = 0; i < n; i++){
      // Garante que o arquivo binário esteja posicionado no começo dos dados para a busca
      fseek(arqBin, 17, SEEK_SET);
      int posSequencial = 17;
      
      // Realiza a busca pelo registro
      int rrnEncontrado = executar_busca_indexada(listaIndice, nRegistrosIndice, h, arqBin, &posSequencial);

      int quantAlt; 
      if (scanf("%d", &quantAlt) != 1) break; 

      char nomeCampoNew[100][500]; 
      char valorCampoNew[100][500]; 

      for (int j = 0; j < quantAlt; j++) {
         scanf("%s", nomeCampoNew[j]);
         
         // CORREÇÃO CRÍTICA: Consome espaços e quebras de linha pendentes do teclado (\n)
         scanf(" "); 
         ScanQuoteString(buffer);

         if (strcmp(buffer, "") == 0) {
            strcpy(valorCampoNew[j], "NULO");
         } else {
            strcpy(valorCampoNew[j], buffer);
         }
      }

      // Alterar o arquivo binário apenas se o RRN foi encontrado com sucesso
      if (rrnEncontrado != -1){
         long byteOffset = 17 + (rrnEncontrado * 80);
         
         // Vai para a posição correta antes de ler o registro original
         fseek(arqBin, byteOffset, SEEK_SET);
         RegistroDado *r = ler_reg_dado_bin(arqBin);

         // Verifica se o codEstacao vai mudar nesta rodada
         int mudouCodigo = 0;
         for (int j = 0; j < quantAlt; j++) {
             if (strcmp(nomeCampoNew[j], "codEstacao") == 0) {
                 mudouCodigo = 1;
                 break;
             }
         }

         // Modifica o registro na memória RAM
         atualizar_reg_pelo_filtro(r, nomeCampoNew, valorCampoNew, quantAlt, listaIndice, nRegistrosIndice);

         // Força o ponteiro a voltar para o local exato antes de reescrever por cima
         fseek(arqBin, byteOffset, SEEK_SET);
         escreve_reg_dado_bin(arqBin, r);

         // Se mudou a chave de busca (código), reordena o índice imediatamente
         if (mudouCodigo) {
             heap(listaIndice, nRegistrosIndice);
         }

         // Liberta a memória da struct de dados utilizada nesta iteração
         free_reg_dado(r);
         free(r); 
      }
   } 

   // ============================================================
   // SALVAR OS ARQUIVOS NO DISCO ANTES DE FECHAR
   // ============================================================

   // Garante a ordenação final do índice antes de gravar fisicamente
   heap(listaIndice, nRegistrosIndice);

   fseek(arqInd, 0, SEEK_SET);
   hInd->status = '1';
   fwrite(&hInd->status, sizeof(char), 1, arqInd);

   for (int k = 0; k < nRegistrosIndice; k++) {
      fwrite(&listaIndice[k], sizeof(RegistroDadoIndice), 1, arqInd);
   }

   h->status = '1'; 
   fseek(arqBin, 0, SEEK_SET);
   escreve_reg_cab_bin(arqBin, h);

   // Liberta a memória e fecha os arquivos com segurança
   free(listaIndice);
   free(h);
   free(hInd);
   fclose(arqBin);
   fclose(arqInd);

   // Exibe os checksums finais na tela
   BinarioNaTela(nomeArqDados);
   BinarioNaTela(nomeArqIndice);
}
