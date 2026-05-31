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
/* Funcionalidade [8]: Inserção de Registros e Atualização de Índice Primário

   1. Ler os argumentos da linha de comando/stdin:
      - Nome do arquivo de dados (ex: estacao.bin)
      - Nome do arquivo de índice (ex: indexaEstacao.bin)
      - Número de inserções 'n'

   2. Abrir os arquivos no modo de leitura e escrita binária ("rb+"):
      - Se algum arquivo não existir, tratar o erro ("Falha no processamento do arquivo.")

   3. Validar e carregar o Índice Primário para a Memória RAM:
      - Ler o cabeçalho do arquivo de índice e verificar o status ('1' = consistente)
      - Mudar o status nos cabeçalhos de ambos os arquivos para '0' (inconsistente) e gravar no disco
      - Carregar todas as chaves e offsets do arquivo de índice para um array dinâmico (RAM)

   4. Loop de Inserções (executar 'n' vezes):
      a. Ler os campos da estação do stdin (tratar aspas de strings e a palavra "NULO")
      b. Ir para o final do arquivo de dados utilizando fseek(..., 0, SEEK_END)
      c. Obter o Byte Offset atual utilizando ftell(...)
      d. Montar a struct do registro com os dados lidos e gravá-la no arquivo de dados (fwrite)
      e. Adicionar a nova chave (codEstacao) e o Byte Offset obtido no nosso array de índices na RAM

   5. Finalizar e Ordenar o Índice na RAM:
      - Ordenar o array de índices pelo 'codEstacao' (usando a função qsort do C)

   6. Gravar o Índice Atualizado de Volta no Disco:
      - Dar um fseek para o início do arquivo de índice
      - Atualizar o cabeçalho do índice (mudar status para '1' e atualizar contadores)
      - Gravar o array ordenado por cima do arquivo anterior

   7. Fechar o Arquivo de Dados de forma Consistente:
      - Atualizar o status do cabeçalho do arquivo de dados para '1'
      - Fechar ambos os arquivos (fclose)

   8. Chamar o BinarioNaTela para ambos os arquivos atualizados. */

void inserir_reg() {
    char nomeArqBin[100];
    char nomeArqInd[100];
    int n;

    // 1. Ler as entradas iniciais 
    scanf("%s %s %d", nomeArqBin, nomeArqInd, &n);

    // Ponteiros 
    FILE *arqBin = NULL;
    FILE *arqInd = NULL;

    // 2. Abrir e validar os arquivos existentes
    RegistroCabecalho *h = abrir_e_validar_arq_bin(nomeArqBin, &arqBin, "rb+");
    RegistroCabecalhoIndice *hInd = abrir_e_validar_ind(nomeArqInd, &arqInd, "rb+");

    if (h == NULL || hInd == NULL) {
        if (arqBin) fclose(arqBin);
        if (arqInd) fclose(arqInd);
        return;
    }

    // 3. Mudar o status dos cabeçalhos para '0' (inconsistente) e gravar no disco
    h->status = '0';
    hInd->status = '0';
    // [Escreva aqui as funções para salvar h e hInd no início dos arquivos]

    fseek(arqBin, 0, SEEK_SET);
    escreve_reg_cab_bin(arqBin, h);

    fseek(arqInd, 0, SEEK_SET);
    escreve_reg_cab_ind(arqInd, hInd);

    // 4. Carregar o índice atual que está no arquivo para a memória RAM
    // [Adicione aqui a lógica ou função para ler os índices do arquivo para a RAM]
    int nRegistrosIndice = h->nroEstacoes;
    int capTotal =  nRegistrosIndice + n;
    RegistroDadoIndice *listaIndice = malloc(capTotal *sizeof(RegistroDadoIndice));

    // fseek(arqInd, 0, SEEK_SET);
    // long tamArq = ftell(arqInd);
    //sem fseek

    for (int i = 0; i < nRegistrosIndice; i++) {
        fread(&listaIndice[i], sizeof(RegistroDadoIndice), 1, arqInd);
    }

    long byteOffSetInicial = 17 + (h->proxRRN * 80);
    fseek(arqBin, byteOffSetInicial, SEEK_SET);

    // 5. Loop para realizar as 'n' inserções
    for (int i = 0; i < n; i++) {
            RegistroDado r;

        // a. Ler os campos do teclado (stdin) tratando "NULO" e usando ScanQuoteString
        
        // b. Descobrir a posição de escrita baseada no h->proxRRN
        // Fórmula: byte_offset = 17 + (h->proxRRN * 80)
        
        // c. Usar fseek para ir até a posição e escreve_reg_dado_bin para salvar no arquivo
        
        // d. Adicionar a nova chave (codEstacao) e o RRN usado no seu array de índices na RAM
        
        // e. Atualizar os contadores do cabeçalho de dados na RAM (proxRRN e nroEstacoes)
        // f. Limpar as strings alocadas para 'r' usando free_reg_dado(&r)
    }


    // 6. Ordenar a lista de índices na RAM
    // Dica: Vocês já têm a função heap(lista, tamanho) pronta no projeto!


    // 7. Salvar tudo e fechar com consistência
    h->status = '1';
    hInd->status = '1';

    // a. Gravar o cabeçalho de dados atualizado no início do arquivo de dados
    // b. Limpar o arquivo de índice antigo (reabrir com "wb"), gravar o hInd e depois a lista da RAM ordenada
    
    // 8. Desalocar memórias e fechar arquivos (fclose)


    // 9. Chamar o BinarioNaTela para ambos os arquivos
    BinarioNaTela(nomeArqBin);
    BinarioNaTela(nomeArqInd);
}