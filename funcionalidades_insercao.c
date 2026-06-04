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
    int n;

    scanf("%s", nomeArqDados);
    scanf("%s", nomeArqIndice);
    if (scanf("%d", &n) != 1) return;

    FILE *arqBin = NULL;
    RegistroCabecalho *h = abrir_e_validar_arq_bin(nomeArqDados, &arqBin, "rb+");
    if (h == NULL) return; 

    FILE *arqInd = NULL;
    RegistroCabecalhoIndice *hInd = abrir_e_validar_ind(nomeArqIndice, &arqInd, "rb+");
    if (hInd == NULL) {
        free_reg_cab(h);
        fclose(arqBin);
        return;
    }

    // Mudar o status de ambos para inconsistente ('0') no início
    h->status = '0';
    fseek(arqBin, 0, SEEK_SET);
    fwrite(&h->status, sizeof(char), 1, arqBin);

    hInd->status = '0';
    fseek(arqInd, 0, SEEK_SET);
    fwrite(&hInd->status, sizeof(char), 1, arqInd);

    int nRegistrosIndice = 0;

    fclose(arqInd); 
    RegistroDadoIndice *listaIndice = carregar_indice_na_memoria(nomeArqIndice, &nRegistrosIndice);
    if (listaIndice == NULL) {
        free_reg_cab(h);
        fclose(arqBin);
        return;
    }
    // Reabrimos o arquivo de índice em modo de escrita/atualização binária
    arqInd = fopen(nomeArqIndice, "rb+");

    // Redimensionamos a lista para aguentar os novos 'n' registros que vão entrar
    listaIndice = realloc(listaIndice, (nRegistrosIndice + n) * sizeof(RegistroDadoIndice));

    // loop de inserções
    char buffer[500];
    for (int i = 0; i < n; i++) {
        RegistroDado *r = malloc(sizeof(RegistroDado));

        // leitura de dados pelo teclado
        // codEstacao 
        ScanQuoteString(buffer);
        r->codEstacao = atoi(buffer);

        // nomeEstacao
        ScanQuoteString(buffer);
        if (strcmp(buffer, "") == 0) {
            r->nomeEstacao = NULL;
            r->tamNomeEstacao = 0;
        } else {
            r->nomeEstacao = malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(r->nomeEstacao, buffer);
            r->tamNomeEstacao = strlen(buffer);
        }

        // codLinha
        ScanQuoteString(buffer);
        r->codLinha = verificar_nulo(buffer);

        // nomeLinha
        ScanQuoteString(buffer);
        if (strcmp(buffer, "") == 0) {
            r->nomeLinha = NULL;
            r->tamNomeLinha = 0;
        } else {
            r->nomeLinha = malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(r->nomeLinha, buffer);
            r->tamNomeLinha = strlen(buffer);
        }

        // codProxEstacao
        ScanQuoteString(buffer);
        r->codProxEstacao = verificar_nulo(buffer);

        // distProxEstacao
        ScanQuoteString(buffer);
        r->distProxEstacao = verificar_nulo(buffer);

        // codLinhaIntegra
        ScanQuoteString(buffer);
        r->codLinhaIntegra = verificar_nulo(buffer);

        // codEstIntegra
        ScanQuoteString(buffer);
        r->codEstIntegra = verificar_nulo(buffer);

        // escrita do arquivo de dados
        int rrnDestino;
        
        if (h->topo == -1) {
            // pilha vazia, escreve no fim do arquivo
            rrnDestino = h->proxRRN;
            long byteOffset = 17 + (rrnDestino * 80);
            fseek(arqBin, byteOffset, SEEK_SET);
            
            escreve_reg_dado_bin(arqBin, r);
            
            h->proxRRN++; // Incrementa o próximo RRN disponível
        } else {
            //reaproveita espaço da pilha
            rrnDestino = h->topo;
            long byteOffset = 17 + (rrnDestino * 80);
            
            // Vamos até o prox da pilha
            fseek(arqBin, byteOffset, SEEK_SET);
            char removido;
            int proximoPilha;
            fread(&removido, sizeof(char), 1, arqBin);
            fread(&proximoPilha, sizeof(int), 1, arqBin);
            
            // Voltamos ao início do espaço e gravamos o registro novo por cima
            fseek(arqBin, byteOffset, SEEK_SET);
            escreve_reg_dado_bin(arqBin, r);
            
            // Atualizamos o topo da pilha com o encadeamento que coletamos
            h->topo = proximoPilha;
        }

        listaIndice[nRegistrosIndice].codEstacao = r->codEstacao;
        listaIndice[nRegistrosIndice].RRN = rrnDestino;
        nRegistrosIndice++;

        h->nroEstacoes++;
        if (r->codEstIntegra != -1) {
            h->nroParesEstacoes++;
        }

        free_reg_dado(r);
        free(r);
    }

    // mudanças no arquivo de índice
    heap(listaIndice, nRegistrosIndice);

    // Grava o cabeçalho do índice atualizado
    fseek(arqInd, 0, SEEK_SET);
    char statusConsistente = '1';
    fwrite(&statusConsistente, sizeof(char), 1, arqInd);

    // Grava todos os dados ordenados no arquivo de índices
    for (int i = 0; i < nRegistrosIndice; i++) {
        fwrite(&listaIndice[i], sizeof(RegistroDadoIndice), 1, arqInd);
    }

    // mudanças no registro de cabeçalho
    h->status = '1';
    fseek(arqBin, 0, SEEK_SET);
    escreve_reg_cab_bin(arqBin, h); 

    free(listaIndice);
    free_reg_cab(h);
    fclose(arqBin);
    fclose(arqInd);

   //binário na tela
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