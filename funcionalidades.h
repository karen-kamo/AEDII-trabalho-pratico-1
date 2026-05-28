/*
Karen Nanamy Kamo - NUSP: 15495932 
Rebeca de Oliveira Silva - NUSP: 11963923
*/

#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H


/*!
 * @brief Funcionalidade 1 para criação do arquivo binário a partir de um csv, com registro de cabeçalho e registros de dados.
 *
 */
void criar_arq_bin();


/*!
 * @brief Funcionalidade 2 para mostrar todos os dados dos registros de dados do arquivo binário
 *
 */
void buscar_todos_reg_bin();


/*!
 * @brief Funcionalidade 3 para mostrar o registro buscado por dados especificados na entrada.
 *
 */
void buscar_reg_filtro();


/*!
 * @brief Funcionalidade 4 para mostrar o registro buscado pelo RRN
 *
 */
void buscar_reg_RRN();


/*!
 * @brief Busca os valores do registro de cabeçalho.
 *
 */
void buscar_reg_cab_bin();

/*!
 * @brief Funcionalidade 5 para criação do arquivo de índice primário com registro de cabeçalho e registros de dados referentes às structs de índices.
 *
 */
void criar_indice();


/*!
 * @brief Funcionalidade 6 para mostrar o registro buscado por dados especificados na entrada, caso uma das buscas seja pelo codEstacao, a busca deve ser feita usando o arquivo de índice.
 *
 */
void busca_indexada();
#endif