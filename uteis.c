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


void BinarioNaTela(char *arquivo) {
    FILE *fs;
    if (arquivo == NULL || !(fs = fopen(arquivo, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }

    fseek(fs, 0, SEEK_END);
    size_t fl = ftell(fs);

    fseek(fs, 0, SEEK_SET);
    unsigned char *mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    unsigned long cs = 0;
    for (unsigned long i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }

    printf("%lf\n", (cs / (double)100));

    free(mb);
    fclose(fs);
}

////////////////////////////////////////////

void imprimir_reg_dado(RegistroDado *r){
  // esses dados nunca podem ser nulos, por isso, não serão tratados
  printf("%d ", r->codEstacao);
  printf("%s ", r->nomeEstacao);

  //Verifica se o campo do Reg. Dados for igual a -1, caso seja deve printar "NULO"
  //Caso não for nulo, possui um valor que deve ser printado em tela. 

  if (r->codLinha == -1) printf("NULO ");
  else printf("%d ", r->codLinha);

  if (strcmp(r->nomeLinha, "") == 0) printf("NULO ");
  else printf("%s ", r->nomeLinha);

  if (r->codProxEstacao == -1) printf("NULO ");
  else printf("%d ", r->codProxEstacao);

  if (r->distProxEstacao == -1) printf("NULO ");
  else printf("%d ", r->distProxEstacao);

  if (r->codLinhaIntegra == -1) printf("NULO ");
  else printf("%d ", r->codLinhaIntegra);

  if (r->codEstIntegra == -1) printf("NULO\n");
  else printf("%d\n", r->codEstIntegra);
}

///////////////////////////////////

void imprimir_reg_cab(RegistroCabecalho *h){
  //Não precisava para o trabalho 1
  //Usado para debugar o código pelas estudantes 
  printf("%c %d %d %d %d\n", h->status, h->topo, h->proxRRN, h->nroEstacoes, h->nroParesEstacoes);
}

////////////////////////////////////

void ScanQuoteString(char *str) {
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n') { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();         // ignorar aspas fechando
    } else if (R != EOF) { // vc tá tentando ler uma string que não tá entre
                           // aspas! Fazer leitura normal %s então, pois deve
                           // ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else { // EOF
        strcpy(str, "");
    }
}

//////////////////////////////

int verificar_nulo(char *valor){
  if (strcmp(valor, "NULO") == 0) return -1;
  return atoi(valor);
}


///////////////////////////////

char check_eof (FILE* arq){
	char ch = 0;
	ch = fgetc(arq);
	fseek(arq, -1, SEEK_CUR);
	return (ch==EOF)?0:1;
}

////////////////////////////////

int verificar_nulo_fixo(char *string){
  if (strcmp(string, "") == 0){
      return -1;
    } else {
      return atoi(string);
    }
}

////////////////////////////////
// funções para ordenar pelo codEstacao
void heapify (RegistroDadoIndice *array, int p, int u){
  int f = 2 * p + 1; // 1º filho da esq

  if (f <= u){
    // se tiver um 2º filho (f+1), ver qual é o maior
    if (f + 1 <= u){
        // se o da dir for maior que o da esq, atualiza o f para a dir
      if (array[f+1].codEstacao > array[f].codEstacao){
        f++;
      }
    }

    //se o pai for menor, troca de valor com o maior filho
    if (array[p].codEstacao < array[f].codEstacao){ 
        // troca a struct inteira
        RegistroDadoIndice rInd = array[p];
        array[p] = array[f];
        array[f] = rInd;
        heapify(array, f, u); //para garantir que todos sejam heaps novamente
    }
  }
}

void heap (RegistroDadoIndice *array, int n){
  //o i = n / 2 é a história de começar, inicialmente, de trás para frente
  for (int i = n / 2 - 1; i >= 0; i--){
    heapify(array, i, n - 1);
  }

  // ordenação, pega o maior e joga para o fim
  for (int i = n - 1; i > 0; i--){
    RegistroDadoIndice rInd = array[i];
    array[i] = array[0];
    array[0] = rInd;
    heapify(array, 0, i - 1);
  }
}
