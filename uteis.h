/*
Karen Nanamy Kamo - NUSP: 15495932 
Rebeca de Oliveira Silva - NUSP: 11963923
*/

#ifndef UTEIS_H
#define UTEIS_H

#include "structs.h"

/*!
 * @brief Função que conta e printa na tela do usuário quantidade de binário na tela. Função disponibilizada pelo monitor
 *
 * @param arquivo O arquivo a ser lido.
 *
 */
void BinarioNaTela(char *arquivo);


/*!
 * @brief Serve para imprimir o conteúdo de um registro de dado.
 *
 * @param r O registro de dado.
 *
 */
void imprimir_reg_dado(RegistroDado *r);


/*!
 * @brief Função auxiliar para mostrar o registro de cabeçalho.
 *
 * @param h O registro de dado.
 *
 */
void imprimir_reg_cab(RegistroCabecalho *h);


/*!
 * @brief Função disponibilizada pelos monitores.
 *
 * @param str A string.
 *
 */
void ScanQuoteString(char *str);


/*!
 * @brief Função auxiliar para verificar se o valor inteiro recebido é nulo. 
 *
 * @param valor O valor que deve ser verificado.
 * 
 * @return Retorna -1, caso seja nulo ou o valor inteiro.
 */
int verificar_nulo(char *valor);



/*!
 * @brief Checa se é o fim do arquivo.
 *
 * @param arq Arquivo csv para checar. Ele precisa permitir leitura.
 *
 * @return Retorna 0 caso seja o fim do arquivo, 1 caso o contrario.
 */
char check_eof (FILE* arq);


/*!
 * @brief Função auxiliar para checa se o valor recebido do arquivo csv é nulo.
 *
 * @param string Valor recebido do csv.
 *
 * @return Retorna um int com o valor -1 se nulo, ou com o valor especificado.
 */
int verificar_nulo_fixo(char *string);


/*!
 * @brief Função para ordenar os registros de dados pelo codEstacao.
 *
 * @param array Vetor de registros de dados
 * 
 * @param n Número de registros de dados que tem no vetor.
 */
void heap (RegistroDadoIndice *array, int n);


/*!
 * @brief Função para carregar os registros de dados em memória primária
 *
 * @param nomeArqInd Nome do arquivo de índice
 * 
 * @param nRegistros Número de registros de dados que tem no vetor.
 * 
 * @return Retorna o ponteiro da memória primária que contém a lista de registros do índice
 */
RegistroDadoIndice *carregar_indice_na_memoria (char *nomeArqInd, int *nRegistros);


/*!
 * @brief Função para realizar a busca binária na lista de Registros de Dados do Índice.
 *
 * @param n Quantidade de registros.
 * 
 * @param codBuscado O codEstacao buscado pelo usuário.
 * 
 * @return Retorna o RRN do codEstacao buscado ou -1, caso não encontre.
 */
int busca_binaria_indice(RegistroDadoIndice *lista, int n, int codBuscado);


/*!
 * @brief Função auxiliar para buscas com filtro, para verificar as buscas filtro a filtro.
 *
 * @param r Registro de dado a ser analisado.
 * 
 * @param nomesCampos Vetor com os campos a serem buscados.
 * 
 * @param valoresCampos Vetor com os valores a serem buscados.
 * 
 * @return Retorna 1 caso algum filtro de certo e 0 se não der em algum.
 */
int verificacao_filtros(RegistroDado *r, char nomesCampos[100][500], char valoresCampos[100][500], int quantPar);


/*!
 * @brief Função auxiliar para abrir um arquivo binário para o modo desejado e verificar o status do arquivo.
 *
 * @param nomeArqBin Nome do arquivo binário
 * 
 * @param arqBin O arquivo binário.
 * 
 * @param modo O modo de abertura do arquivo.
 * 
 * @return Retorna o RegistroCabecalho do arquivo binário.
 */
RegistroCabecalho *abrir_e_validar_arq_bin (char *nomeArqBin, FILE **arqBin, char modo[3]);


/*!
 * @brief Função auxiliar para abrir um arquivo de índice para o modo desejado e verificar o status do arquivo.
 *
 * @param nomeArqInd Nome do arquivo de índice
 * 
 * @param arqInd O arquivo de índice.
 * 
 * @param modo O modo de abertura do arquivo.
 * 
 * @return Retorna o RegistroCabecalho do arquivo binário.
 */
RegistroCabecalhoIndice *abrir_e_validar_ind (char *nomeArqInd, FILE **arqInd, char modo[3]);



/*!
 * @brief Função auxiliar que faz a busca binária tradicional, retorna a posição ocupada pelo registro na lista do índice na memória.
 *
 * @param listaIndice Lista dinâmica do índice.
 * 
 * @param nRegistrosIndice Número de registros.
 * 
 * @param codBuscado O codEstacao buscado.
 * 
 * @return Retorna a posição do registro buscado na lista de índices da memória. O outro retorna o RRN.
 */
int busca_binaria_posicao_lista_indice(RegistroDadoIndice *listaIndice, int nRegistrosIndice, int codBuscado);
#endif