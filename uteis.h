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
#endif