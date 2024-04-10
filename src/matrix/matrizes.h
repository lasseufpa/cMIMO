/// @file Main.H
#ifndef _H_MYCOMPLEX
#define _H_MYCOMPLEX

#include <stdlib.h>

/** A definição do código apresenta uma estrutura chamada `complexo` que é utilizada para representar números complexos. A estrutura possui dois campos: `real` e `img`, ambos do tipo `float`. 
*O campo `real` é responsável por armazenar a parte real do número complexo, enquanto o campo `img` é utilizado para armazenar a parte imaginária. Essa estrutura é útil para armazenar e manipular números complexos em programas escritos em linguagem C.
 Essa definição de estrutura pode ser incluída em programas C que envolvem cálculos com números complexos, permitindo uma organização clara e acessível dos dados relacionados a números complexos.
 */
typedef struct complexo {
    double real; ///< Parte real
    double img; ///< Parte imaginária
} complexo;
//Função: Teste de todas as funções de álgebra matricial do código.
void teste_todos(void);
//Função: Transposição de uma matriz.
complexo** transposta(complexo **mtx, int linhas, int colunas);
//Função: Conjugada de uma matriz complexa.
complexo** conjugada(complexo **mtx, int linhas, int colunas);
//Função: Hermitiana de uma matriz complexa.
complexo** hermitiana(complexo **mtx, int linhas, int colunas);
//Função: Soma entre duas matrizes complexas.
complexo** soma(complexo **mtx_a, complexo **mtx_b, int linhas, int colunas);
//Função: Subtração entre duas matrizes complexas.
complexo** subtracao(complexo **mtx_a, complexo **mtx_b, int linhas, int colunas);
//Função: Produto interno entre dois vetores complexos.
complexo produto_interno(complexo **v1, complexo **v2, int linhas, int colunas);
//Função: Produto matricial entre duas matrizes complexas.
complexo** produto_matricial(complexo **mtx_a, complexo **mtx_b, int linhas, int colunas);
//Função: Multiplicação por um escalar k.
complexo** produto_por_escalar(complexo **mtx, int linhas, int colunas, int k);
//Funções de teste.
void teste_transposta(void);
void teste_conjugada(void);
void teste_hermetiana(void);
void teste_soma(void);
void teste_subtracao(void);
void teste_produto_interno(void);
void teste_produto_matricial(void);
//Manipulação de memória.
complexo** allocateComplexMatrix(int linhas, int colunas);
void LiberarMatriz(complexo **mtx, int linhas);
//Manipulação de números complexos.
void printComplex(complexo c);
complexo soma_complexo(complexo c1, complexo c2);
complexo multcomp(complexo c1, complexo c2);
void calc_svd(complexo** mtx, int linhas, int colunas);
#endif
