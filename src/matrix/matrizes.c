/// @file matrizes.c
#include <stdio.h>
#include <stdlib.h>
#include "matrizes.h"
#include <gsl/gsl_linalg.h>

/**
 * @brief Executa cada função de teste uma vez.
 *
 * Esta função realiza o teste de todas as funções implementadas na biblioteca Matrizes.
 * Cada teste é realizado para garantir o correto funcionamento das funções.
 */
void teste_todos(void)
{	
	printf("\n===============================Teste da Operação Transposta======================\n");
	complexo** c1,**c2; 
	int nlinhas,ncolunas;
    nlinhas=3;
    ncolunas=3;
    //Alocação de memoria para a matriz:
    c1 = allocateComplexMatrix(nlinhas,ncolunas);
	
        for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c1[l][c].real = (2*l)-c;
				c1[l][c].img = c-1+l;
			}
    }
        printf("\nOperando A:\n");
        for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(c1[l][c]);
			}
        printf("\n");
    }
	int nlinhas_trans = ncolunas;
	int ncolunas_trans = nlinhas;
	
	printf("\nResultado:\n");
    c2 = transposta(c1,nlinhas_trans,ncolunas_trans);
        for (int l =0 ; l < nlinhas_trans; l++)
		{
			for (int c = 0; c < ncolunas_trans; c++)
			{
				printComplex(c2[l][c]);
			}
			printf("\n");
		}
		
	printf("\n===============================Teste da Operação Conjugada===============================\n");
//Kauan (06.05.23)
	complexo **c10, **conj_c10;

	//Alocação de memória. 
	c10 = allocateComplexMatrix(nlinhas, ncolunas);

	//Preenchimento da(s) matriz(es).
	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			c10[l][c].real = l + c;
			c10[l][c].img = 1 + l + c;
		}
	}

	//Chamada da função conjugada.
	conj_c10 = conjugada(c10, nlinhas, ncolunas);

	printf("\nOperando A:\n");
	//Impressão da Matriz A.
	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(c10[l][c]);
		}
		printf("\n");
	}

	printf("\nResultado:\n");
	//Impressão da(s) matriz(es) conjugada(s)

	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(conj_c10[l][c]);
		}
		printf("\n");
	}


	printf("\n===============================Teste da operação hermitiana===============================\n");

	complexo **mtx_a, **mtx_h;

    mtx_a = allocateComplexMatrix(nlinhas, ncolunas);

    for (int l = 0; l < ncolunas; l++)
    {
        for (int c = 0; c < ncolunas; c++)
        {
            mtx_a[l][c].real = l + c;
            mtx_a[l][c].img = l - 2*c;
        }
    }

    printf("\nOperando A:\n");

    for (int l = 0 ; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_a[l][c]);
		}
        printf("\n");
	}

    mtx_h = hermitiana(mtx_a, nlinhas, ncolunas);

	printf("\nResultado:\n");

    for (int l = 0 ; l < nlinhas_trans; l++)
	{
		for (int c = 0; c < ncolunas_trans; c++)
		{
			printComplex(mtx_h[l][c]);
		}
        printf("\n");
	}

	printf("\n\n===============================Teste da Operação Soma===========================\n");
	
	complexo **c3,**c4,**somado1;
    //Alocação de memoria para a matriz:
    c3 = allocateComplexMatrix(nlinhas,ncolunas);
	c4 = allocateComplexMatrix(nlinhas,ncolunas);
	for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c3[l][c].real = l+1;
				c3[l][c].img = (2*l)-c;
				c4[l][c].real = (5*l)+c;
				c4[l][c].img = l-c;
			}
		}
	printf("\nOperando A:\n");
	for (int l =0 ; l < nlinhas; l++)
	{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(c3[l][c]);
			}
		printf("\n");
	}
	printf("\nOperando B:\n");
	for (int l =0 ; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(c4[l][c]);
		}
		printf("\n");
	}
	printf("\nResultado:\n");
	somado1 = soma(c3,c4,nlinhas,ncolunas);
	for (int l =0 ; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(somado1[l][c]);
		}
		printf("\n");
	}
	
	printf("\n\n===============================Teste da operação Subtracao===========================\n");
	complexo **c5,**c6,**sub1;
    //Alocação de memoria para a matriz:
    c5 = allocateComplexMatrix(nlinhas,ncolunas);
	c6 = allocateComplexMatrix(nlinhas,ncolunas);
	for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c5[l][c].real = l-1;
				c5[l][c].img = (3*l)-2*c;
				c6[l][c].real = (4*l)-c;
				c6[l][c].img = -l-c;
			}
    }
	
	printf("\nOperando A:\n");
		for (int l =0 ; l < nlinhas; l++)
		{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c5[l][c]);
				}
			printf("\n");
		}
	printf("\nOperando B:\n");
		for (int l =0 ; l < nlinhas; l++)
			{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c6[l][c]);
				}
			printf("\n");
		}
	printf("\nResultado:\n");
	sub1 = subtracao(c5,c6,nlinhas,ncolunas);
	for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(sub1[l][c]);
			}
        printf("\n");
	}
	
	printf("\n======================Teste do Produto Interno======================\n");
	complexo **c13, **c14;
    int colunasvetor=1;
    c13 = allocateComplexMatrix(nlinhas, colunasvetor);
    c14 = allocateComplexMatrix(nlinhas, colunasvetor);
	
    for (int l=0; l<nlinhas; l++)
    {
        c13[l][0].real=2*l-3;
        c13[l][0].img=3*l+4;
        c14[l][0].real=4*l-5;
        c14[l][0].img=5*l+6;
    }
   
    printf("\nOperando A:\n");
    for (int l=0; l<nlinhas; l++)
    {
        printComplex(c13[l][0]);
        printf("\n");
    }
    printf("\nOperando B:\n");
    for (int l=0; l<nlinhas; l++)
    {
        printComplex(c14[l][0]);
        printf("\n");
    }
    printf("\nResultado:\n");
    printComplex(produto_interno(c13,c14,nlinhas,colunasvetor));
	printf("\n");


	printf("\n===============================Teste da operação Produto Matricial===========================\n");

	complexo **mtx_7, **mtx_8, **mtx_produto;
	
	int nlinhas_7 = 3, ncolunas_7 = 3, nlinhas_8 = 3, ncolunas_8 = 3;
	
	//Alocação de memória.
	mtx_7 = allocateComplexMatrix(nlinhas_7, ncolunas_7);
	mtx_8 = allocateComplexMatrix(nlinhas_8, ncolunas_8);

	//Preenchimento das matrizes operandas.
	for (int l = 0; l < nlinhas_7; l++)
	{
		for (int c = 0; c < ncolunas_7; c++)
		{
 			mtx_7[l][c].real = 1 + l;
			mtx_7[l][c].img = 1 + c;
		}
	}

	for (int l = 0; l < nlinhas_8; l++)
	{
		for (int c = 0; c < ncolunas_8; c++)
		{
			mtx_8[l][c].real = 0;
			mtx_8[l][c].img = 1 + c;
		}
	}
	//Impressão dos Operandos:
	printf("\nOperando A:\n");
	//Impressão da matriz A.
	for (int l = 0 ; l < nlinhas_7; l++)
	{
		for (int c = 0; c < ncolunas_7; c++)
		{
			printComplex(mtx_7[l][c]);
		}
        printf("\n");
	}
	
	printf("\nOperando B:\n");
	//Impressão da matriz B.
	for (int l = 0 ; l < nlinhas_8; l++)
	{
		for (int c = 0; c < ncolunas_8; c++)
		{
			printComplex(mtx_8[l][c]);
		}
        printf("\n");
	}
	//Chamada da função produto_matricial
	mtx_produto = produto_matricial(mtx_7, mtx_8, ncolunas_7, nlinhas_8);
	//Impressão do resultado
	printf("\nResultado:\n");

	for (int l = 0 ; l < nlinhas_7; l++)
	{
		for (int c = 0; c < ncolunas_8; c++)
		{
			printComplex(mtx_produto[l][c]);
		}
        printf("\n");
	}

	//Finalização do teste Hermetiano.
	printf("\n===============================Teste Multiplicação por um escalar===============================\n");
	complexo **mtx_12, **mtx_k12;
	int k = 5;
	//Alocação de memória.
	mtx_12 = allocateComplexMatrix(nlinhas, ncolunas);


	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			mtx_12[l][c].real = 1 + l + c;
			mtx_12[l][c].img = l*c - 2;
		}
	}
	//Impressão da Matriz A.
	printf("\nOperando A:\n");

	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_12[l][c]);
		}
		printf("\n");
	}
	
	printf("\nOperando k = %d\n",k);
	//Chamada da função produto_por_escalar.
	mtx_k12 = produto_por_escalar(mtx_12, nlinhas, ncolunas, k); 
	//Impressão do resultado.
	printf("\nResultado de %d*A:\n",k);

	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_k12[l][c]);
		}
		printf("\n");
	}

	printf("\n===============================Teste Cálculo de SVD===============================\n");

	complexo **mtx_13;
	int l_13 = 4, c_13 = 3;

	mtx_13 = allocateComplexMatrix(l_13, c_13);

	for (int l = 0; l < l_13; l++)
	{
		for (int c = 0; c < c_13; c++)
		{
			mtx_13[l][c].real = 1 + l + c;
			mtx_13[l][c].img = 0;
		}
	}

	printf("\n\nOperando MTX\n");

	for (int l = 0; l < l_13; l++)
	{
		for (int c = 0; c < c_13; c++)
		{
			printComplex(mtx_13[l][c]);
		}
		printf("\n");
	}

	calc_svd (mtx_13, l_13, c_13); 
}
/**###Função Transposta: 
 * A função `transposta` implementa a operação de transposição de matriz. Essa operação consiste em trocar as linhas pelas colunas da matriz de entrada.
- A função `transposta` recebe três parâmetros: `mtx` (uma matriz de números complexos), `linhas` (o número de linhas da matriz) e `colunas` (o número de colunas da matriz).
- Ela declara uma variável do tipo `complexo**` chamada `matriz`, que será usada para armazenar a matriz transposta.
- Em seguida, a função aloca memória para a matriz `matriz` usando a função `allocateComplexMatrix`. A matriz `matriz` tem o tamanho `colunas x linhas`, ou seja, o número de colunas da matriz de entrada se torna o número de linhas da matriz transposta, e o número de linhas da matriz de entrada se torna o número de colunas da matriz transposta.
- A função usa dois loops aninhados para percorrer cada elemento da matriz `mtx`. O loop externo itera pelas linhas e o loop interno itera pelas colunas.
- Dentro do loop, a função atribui o valor real do elemento da matriz de entrada `mtx[c][l].real` ao elemento correspondente da matriz `matriz[l][c].real`.
- A função atribui também o valor da parte imaginária do elemento da matriz de entrada `mtx[c][l].img` ao elemento correspondente da matriz `matriz[l][c].img`.
- Depois que todos os elementos da matriz `mtx` são processados, a função retorna a matriz `matriz`, que contém a matriz transposta resultante.
 * @param[in] mtx, linhas, colunas
 * @param[out] mtx_transposta
 * */
complexo **transposta(complexo** mtx,int linhas, int colunas)
{
        complexo** matriz;

        matriz = allocateComplexMatrix(colunas,linhas);
        for (int l = 0; l < linhas; l++)
		{
			for (int c = 0; c < colunas; c++)
			{
				matriz[c][l].real = mtx[l][c].real;
                matriz[c][l].img = mtx[l][c].img;
			}
		}
    return matriz;
}
/**###Função Conjugada:
 *A função `conjugada` implementa a operação de matriz conjugada. Essa operação consiste em inverter o sinal da parte imaginária de cada elemento da matriz de entrada.
- A função `conjugada` recebe três parâmetros: `mtx` (uma matriz de números complexos), `linhas` (o número de linhas da matriz) e `colunas` (o número de colunas da matriz).
- Ela declara uma variável do tipo `complexo**` chamada `matrix`, que será usada para armazenar a matriz conjugada.
- Em seguida, a função aloca memória para a matriz `matrix` usando a função `allocateComplexMatrix`. A matriz `matrix` tem o mesmo tamanho da matriz de entrada `mtx`.
- A função usa dois loops aninhados para percorrer cada elemento da matriz `mtx`. O loop externo itera pelas linhas e o loop interno itera pelas colunas.
- Dentro do loop, a função atribui o valor real do elemento da matriz de entrada `mtx[l][c].real` ao elemento correspondente da matriz `matrix[l][c].real`, preservando o mesmo valor.
- A função inverte o sinal da parte imaginária do elemento da matriz de entrada, multiplicando-a por -1: `matrix[l][c].img = -mtx[l][c].img`.
- Depois que todos os elementos da matriz `mtx` são processados, a função retorna a matriz `matrix`, que contém a matriz conjugada resultante. 
 * @param[in] mtx, linhas, colunas
 * @param[out] mtx_conjugada
 * */
complexo **conjugada(complexo **mtx, int linhas, int colunas) 
{
    complexo ** matrix;
    //alocação da matriz 
    matrix = allocateComplexMatrix (linhas, colunas);

    for (int l = 0; l < linhas; l++)
    {
        for (int c = 0; c < colunas; c++)
        {
            matrix[l][c].real = mtx[l][c].real;
            matrix[l][c].img = -mtx[l][c].img;
        }
    }

    return matrix;
}
/**###Função Hermitiana: 
 * A função `hermitiana` implementa a operação de matriz hermitiana. Essa operação consiste em obter a matriz conjugada da matriz de entrada e, em seguida, calcular a transposta dessa matriz conjugada. A matriz resultante é uma matriz hermitiana.																																							
- A função hermitiana recebe três parâmetros: mtx (uma matriz de números complexos), linhas (o número de linhas da matriz) e colunas (o número de colunas da matriz).
Ela declara duas variáveis do tipo complexo**: matriz e matriz_h.
- Em seguida, a função aloca memória para duas matrizes usando a função allocateComplexMatrix. A primeira matriz (matriz) tem o mesmo tamanho da matriz de entrada, enquanto a segunda matriz (matriz_h), também tem o mesmo tamanho.
- A função chama a função conjugada passando a matriz de entrada mtx, linhas e colunas. Essa função retorna a matriz conjugada da matriz de entrada.
- O resultado da função conjugada é atribuído à matriz matriz.
- Em seguida, a função chama a função transposta passando a matriz matriz, linhas e colunas. Essa função retorna a transposta da matriz passada como argumento.
O resultado da função transposta é atribuído à matriz matriz_h.
- Por fim, a função retorna a matriz matriz_h, que é a matriz hermitiana resultante.
 @param[in] mtx, linhas, colunas
 @param[out] matriz_h.
*/
complexo **hermitiana(complexo** mtx, int linhas, int colunas)
{
    complexo **matriz, **matriz_h;

    matriz = allocateComplexMatrix(linhas, colunas);
    matriz_h = allocateComplexMatrix(linhas, colunas);

    matriz = conjugada(mtx, linhas, colunas);
    matriz_h = transposta(matriz, linhas, colunas);

    return matriz_h;
}
/**###Função Soma: 
 * A função `soma` implementa a operação de soma de matriz(es). Essa operação consiste em somar duas ou mais matrizes, entretanto no exemplo a seguir só com duas matrizes.
- A função `soma` recebe quatro parâmetros: `mtx_a` (uma matriz "a" de números complexos),`mtx_b` (uma matriz "b" de números complexos), `linhas` (o número de linhas da matriz) e `colunas` (o número de colunas da matriz).
- Ela declara uma variável do tipo `complexo**` chamada `matriz`, que será usada para armazenar a matriz da operação de soma.
- Em seguida, a função aloca memória para a matriz `matriz` usando a função `allocateComplexMatrix`. A matriz `matriz` tem o mesmo tamanho das matrizes "a" e "b".
- A função usa dois loops aninhados para percorrer cada elemento da matriz `mtx`. O loop externo itera pelas linhas e o loop interno itera pelas colunas.
- Dentro do loop, a função atribui o valor real da soma dos elementos da matriz de entrada `mtx_a[c][l].real` e `mtx_b[c][l].real`, ao elemento correspondente da matriz `matriz[l][c].real`.
- A função atribui também de maneira análoga a soma do valor da parte imaginária dos elementos das matrizes de entrada `mtx_a[c][l].img` e `mtx_b[c][l].real`, ao elemento correspondente da matriz `matriz[l][c].img`.	
- Depois que todos os elementos das matrizes `mtx_a` e 'mtx_b' são processados, a função retorna a matriz `matriz`, que contém a matriz da função soma.
 * @param[in] mtx_a, mtx_b, linhas, colunas
 * @param[out] matriz
 * */
complexo **soma(complexo** mtx_a, complexo** mtx_b, int linhas, int colunas)
{
	complexo** matriz;
	
	matriz = allocateComplexMatrix(linhas,colunas);
		for (int l = 0; l < linhas; l++)
		{
			for (int c = 0; c < colunas; c++)
			{
				matriz[l][c].real = mtx_a[l][c].real + mtx_b[l][c].real;
                matriz[l][c].img = mtx_a[l][c].img + mtx_b[l][c].img;
			}
		}
	return matriz;
}
/**###Função Subtração: 
 * A função `subtração` implementa a operação de soma de matriz(es). Essa operação consiste em subtrair duas ou mais matrizes, entretanto no exemplo a seguir como no acima só com duas matrizes.
- A função `subtração` recebe quatro parâmetros: `mtx_a` (uma matriz "a" de números complexos),`mtx_b` (uma matriz "b" de números complexos), `linhas` (o número de linhas da matriz) e `colunas` (o número de colunas da matriz).
- Ela declara uma variável do tipo `complexo**` chamada `matriz`, que será usada para armazenar a matriz da operação de subtração.
- Em seguida, a função aloca memória para a matriz `matriz` usando a função `allocateComplexMatrix`. A matriz `matriz` tem o mesmo tamanho das matrizes "a" e "b".
- A função usa dois loops aninhados para percorrer cada elemento da matriz `mtx`. O loop externo itera pelas linhas e o loop interno itera pelas colunas.
- Dentro do loop, a função atribui o valor real da subtração dos elementos da matriz de entrada `mtx_a[c][l].real` e `mtx_b[c][l].real`, ao elemento correspondente da matriz `matriz[l][c].real`.
- A função atribui também de maneira análoga a subtração do valor da parte imaginária dos elementos das matrizes de entrada `mtx_a[c][l].img` e `mtx_b[c][l].real`, ao elemento correspondente da matriz `matriz[l][c].img`.	
- Depois que todos os elementos das matrizes `mtx_a` e 'mtx_b' são processados, a função retorna a matriz `matriz`, que contém a matriz da função subtração.
 * @param[in] mtx_a, mtx_b, linhas, colunas
 * @param[out] matriz
 * */
complexo **subtracao(complexo** mtx_a, complexo** mtx_b, int linhas, int colunas)
{
	complexo** matriz;
	
	matriz = allocateComplexMatrix(linhas,colunas);
		for (int l = 0; l < linhas; l++)
		{
			for (int c = 0; c < colunas; c++)
			{
				matriz[l][c].real = mtx_a[l][c].real - mtx_b[l][c].real;
                matriz[l][c].img = mtx_a[l][c].img - mtx_b[l][c].img;
			}
		}
	return matriz;
}
/**###Função Produto Interno: 
 * A função `produto interno` implementa a operação de produto de dois vetores complexos.
- A função `produto interno` recebe quatro parâmetros: `v1` (um vetor "v1" de números complexos),`v2` (um vetor "v2" de números complexos), `linhas` (o número de linhas dos vetores) e `colunas` (o número de colunas dos vetores).
- Ela declara uma variável do tipo `complexo` chamada `resultado` com valores iniciais (0,0), que será usada para armazenar o resulado do produto interno.
- Em seguida, a função verifica se o número de colunas seja igual a 1. Caso contrário, exibi-se uma mensagem de erro que o produto não pode ser realizado pela a incompatibilidade de vetores, a função encerra com a função 'exit(1)'
- A função após a verificação inicia um loop que percorre as linhas dos vetores, no loop é feita a multiplicação de 'v1[l][0]' e 'v2[l][0]' por meio da função 'multcomp' que é armazenada na variável 'temp'.
- Após a multiplicação, a variável 'temp' é somada à variável 'resultado' usando a função 'multcomp'.
 * @param[in] v1, v2, linhas, colunas
 * @param[out] resultado
 * */
complexo produto_interno(complexo **v1, complexo **v2, int linhas, int colunas)
{
    complexo resultado = {0, 0};
    if(colunas != 1)
	{
		printf("\nErro: O produto não pode ser realizado (incompatibilidade entre vetores)\n");
		exit(1);
	}
    
    for (int l = 0; l< linhas; l++)
    {
        complexo temp = multcomp(v1[l][0], v2[l][0]);
        resultado = soma_complexo(resultado, temp);
    }

    return resultado;

}
/**###Função Produto Matricial: 
 * A função `produto matricial` implementa a operação de produto de duas matrizes complexos.
- A função `produto matricial` recebe quatro parâmetros: `mtx_a` (uma matriz "a" de números complexos),`mtx_b` (uma matriz "b" de números complexos), `linhas` (o número de linhas das matrizes) e `colunas` (o número de colunas das matrizes).
- Ela declara uma variável do tipo `complexo` chamada `matriz`, que será usada para armazenar o resulado do produto matricial.
- Em seguida, a função valida a operação verificando se o número de colunas da matriz 'a' é igual ao número de linhas da matriz 'b' seja igual. Caso não, exibi-se uma mensagem de erro que o produto não pode ser realizado pela a incompatibilidade de matrizes e encerra o programa com a função 'exit(1)'.
- A função após a verificação inicia um loop para percorrer as células da matriz resultante, para cada célula um acumulador é iniciado com o valor zero.
- Dentro do loop, é realizado outro loop para calcular o somatório do produto da matriz a e matriz b, tal operação é feita a partir da função 'multcomp'.
- Em seguida a função 'soma_complexo' é chamada para adiconar o resultado ao acumulador.
 * @param[in] mtx_a, mtx_b, linhas, colunas
 * @param[out] matriz
 * */
complexo **produto_matricial(complexo** mtx_a, complexo **mtx_b, int linhas, int colunas)  
{
	//Validação da operação de multiplicação (colunas_a == linhas_b).
	if(linhas != colunas)
	{
		printf("\nErro: O produto não pode ser realizado (incompatibilidade entre matrizes)\n");
		exit(1);
	}
	complexo** matriz;
	matriz = allocateComplexMatrix(linhas, colunas);
	int linhas_b = colunas; 
	for (int l = 0; l < linhas; l++)
	{
		for (int c = 0; c < colunas; c++)
		{
			complexo acumulador;
			acumulador.real = 0;
			acumulador.img = 0;
			for (int i = 0; i < linhas_b; i ++)
			{
				acumulador = soma_complexo(acumulador, multcomp(mtx_a[l][i],mtx_b[i][c]));
			}
			matriz[l][c].real = acumulador.real;
			matriz[l][c].img = acumulador.img;
		}
	}
	return matriz;
}

/*complexo** produto_matricial_plus(complexo** mtx_a, complexo** mtx_b, int linhas_a, int colunas_a, int linhas_b, int colunas_b)
{
    // Validação da operação de multiplicação (colunas_a == linhas_b).
    if (colunas_a != linhas_b)
    {
        printf("\nErro: O produto não pode ser realizado (incompatibilidade entre matrizes)\n");
        exit(1);
    }
    
    complexo** matriz;
    matriz = allocateComplexMatrix(linhas_a, colunas_b);
    
    for (int l = 0; l < linhas_a; l++)
    {
        for (int c = 0; c < colunas_b; c++)
        {
            complexo acumulador;
            acumulador.real = 0;
            acumulador.img = 0;
            
            for (int i = 0; i < colunas_a; i++)
            {
                acumulador = soma_complexo(acumulador, multcomp(mtx_a[l][i], mtx_b[i][c]));
            }
            
            matriz[l][c].real = acumulador.real;
            matriz[l][c].img = acumulador.img;
        }
    }
    
    return matriz;
}*/
/**###Função Produto por Escalar: 
 * A função `produto por escalar` implementa a operação de matriz por um escalar qualquer. 
- A função produto por escalar recebe quatro parâmetros: mtx (uma matriz de números complexos), linhas (o número de linhas da matriz), colunas (o número de colunas da matriz) e k(número inteiro escalar).
Ela declara uma variável do tipo complexo**: matriz.
- Em seguida, a função aloca memória para a matriz usando a função allocateComplexMatrix. 
- A função usa dois loops aninhados para percorrer cada elemento da matriz `mtx`. O loop externo itera pelas linhas e o loop interno itera pelas colunas.
- Dentro do loop, a função atribui o valor real do elemento da matriz de entrada `mtx[c][l].real`, ao elemento correspondente da matriz `matriz[l][c].real` que é multiplicado pelo o inteiro 'k'.
- A função atribui também de maneira análoga, o valor da multplicação do escalar pela a parte imaginária do elemento das matriz de entrada `mtx[c][l].img` , ao elemento correspondente da matriz `matriz[l][c].img`.	
- Depois que todos os elementos das matriz`mtx` são processados, a função retorna a matriz `matriz

 @param[in] mtx, linhas, colunas, k
 @param[out] matriz.
*/
complexo** produto_por_escalar(complexo **mtx, int linhas, int colunas, int k)
{
	complexo **matriz;

	matriz = allocateComplexMatrix(linhas, colunas);

	for (int l = 0; l < linhas; l++)
	{
		for (int c = 0; c < colunas; c++)
		{
			matriz[l][c].real = k*mtx[l][c].real;
			matriz[l][c].img = k*mtx[l][c].img;
		}
	}
	return matriz;
}
/**### Teste da função Transposta.
 * 
 */ 
void teste_transposta(void)
{
	complexo** c1,**c2,**c3,**c4; 
	int nlinhas,ncolunas;
    nlinhas=2; 
    ncolunas=3; 
    c1 = allocateComplexMatrix(nlinhas,ncolunas);
	printf("\n===============================Teste da operação Transposta======================\n");
	
        for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c1[l][c].real = (3*l)-c;
				c1[l][c].img = c+l;
			}
    }
        printf("\nOperando A:\n");
        for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(c1[l][c]);
			}
        printf("\n");
    }
	int nlinhas_trans = ncolunas;
	int ncolunas_trans = nlinhas;
	
	printf("\nResultado R:\n");
    c2 = transposta(c1,nlinhas_trans,ncolunas_trans);
        for (int l =0 ; l < nlinhas_trans; l++)
		{
			for (int c = 0; c < ncolunas_trans; c++)
			{
				printComplex(c2[l][c]);
			}
        printf("\n");
		}
   
    c3 = allocateComplexMatrix(nlinhas,ncolunas);
	
        for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c3[l][c].real = (3*l)+c;
				c3[l][c].img = -c+l;
			}
    }
        printf("\nOperando B:\n");
        for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(c3[l][c]);
			}
        printf("\n");
    }
	
	printf("\nResultado R:\n");
    c4 = transposta(c3,nlinhas_trans,ncolunas_trans);
        for (int l =0 ; l < nlinhas_trans; l++)
		{
			for (int c = 0; c < ncolunas_trans; c++)
			{
				printComplex(c4[l][c]);
			}
        printf("\n");
		}
}

/**###Função Calculo do SVD: 
 * A função `Calculo SVD` implementa o operador SVD.
- A função `Calculo SVD` recebe três parâmetros: `mtx` (uma matriz de números complexos),`linhas` (número de linhas da matriz), `colunas` (o número de colunas da matriz).
- O SVD foi implementado somente para matrizes reais, portanto, antes de iniciar o calculo do SVD, a função testa se há parte imaginaria, caso houver um aviso aparece na tela informando que não há suporte para matrizes complexas.
- Não há retorno na função, ela imprime na tela o valor do SVD da matriz `mtx`.
 * @param[in] mtx, linhas, colunas
 * @param[out] void
 * */


void calc_svd(complexo** mtx, int linhas, int colunas)
{
	for (int l = 0; l < linhas; l++)
	{
		for (int c = 0; c < colunas; c++)
		{
			if (mtx[l][c].img != 0){
				printf("Warning: complex matrix injected as parameter, fuction will use only real part from matrix\n");
				break;
			}
		}
	}

    gsl_matrix * A = gsl_matrix_alloc(linhas, colunas);
    gsl_matrix * V = gsl_matrix_alloc(colunas, colunas);
    gsl_vector * S = gsl_vector_alloc(colunas);
    gsl_vector * work = gsl_vector_alloc(colunas);

    printf("\n\nMatriz A\n");
    for(int l=0; l<linhas; l++)
	{
        for(int c=0; c<colunas; c++)
		{
            printf("%f ", mtx[l][c].real);
            gsl_matrix_set(A, l, c, mtx[l][c].real);
        }
        printf("\n");
    }

    gsl_linalg_SV_decomp(A, V, S, work);

    printf("\n\nMatriz U\n");
    for(int l=0; l<linhas; l++)
	{
        for(int c=0; c<colunas; c++)
		{
            printf("%f ", gsl_matrix_get(A, l, c));
        }
        printf("\n");
    }

    printf("\n\nVetor S\n");
    for(int c=0;c<colunas;c++)
	{
        printf("%f", gsl_vector_get(S,c));
        printf("\n");
    }

    printf("\n\nMatriz V\n");
    for(int l=0; l<colunas; l++)
	{
        for(int c=0; c<colunas; c++)
		{
            printf("%f ", gsl_matrix_get(V, l, c));
        }
        printf("\n");
    }
}

void teste_conjugada(void) //Kauan (06.05.23)
/**
 * Teste da Função conjugada 
*/ 
{
	 printf("\n======================Teste da Operação Conjugada======================\n");
	complexo **mtx_a, **mtx_conj_a, **mtx_b, **mtx_conj_b, **mtx_c, **mtx_conj_c; 	

	int nlinhas = 3, ncolunas = 3;

	mtx_a = allocateComplexMatrix(nlinhas, ncolunas); 	 	
	mtx_b = allocateComplexMatrix(nlinhas, ncolunas);   
	mtx_c = allocateComplexMatrix(nlinhas, ncolunas);	   		

		for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			mtx_a[l][c].real = (1+l)*c;
			mtx_a[l][c].img = 4*l - 2*c;

			mtx_b[l][c].real = 2 + l +c; 
			mtx_b[l][c].img = l - 3*c;

			mtx_c[l][c].real = -2*l + 3*c;
			mtx_c[l][c].img = -l - 2*c;
		}
	}
   
	
	mtx_conj_a = conjugada(mtx_a, nlinhas, ncolunas);
	mtx_conj_b = conjugada(mtx_b, nlinhas, ncolunas);
	mtx_conj_c = conjugada(mtx_c, nlinhas, ncolunas);
	
	printf("\nOperando A:\n");
		for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_a[l][c]);
		}
		printf("\n");
	}
	
	printf("\nConjugada de A:\n");

	for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_conj_a[l][c]);
		}
		printf("\n");
	}
	printf("\nOperando B:\n");

		for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_b[l][c]);
		}
		printf("\n");
	}

	printf("\nConjugada de B:\n");

		for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_conj_b[l][c]);
		}
		printf("\n");
	}
	
	printf("\nOperando C:\n");
		for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_c[l][c]);
		}
		printf("\n");
	}
	

	printf("\nConjugada de C:\n");

			for (int l = 0; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_conj_c[l][c]);
		}
		printf("\n");
	}
	//Finalização do teste.
}
/** Teste da função Hermitiana. */
void teste_hermitiana(void)

{
    complexo **mtx_a, **mtx_a_h, **mtx_b, **mtx_b_h, **mtx_c, **mtx_c_h; 

    int nlinhas =3, ncolunas = 3; 
	int nlinhas_trans = ncolunas;
	int ncolunas_trans = nlinhas;

    mtx_a = allocateComplexMatrix(nlinhas, ncolunas);
	mtx_b = allocateComplexMatrix(nlinhas, ncolunas);
	mtx_c = allocateComplexMatrix(nlinhas, ncolunas);
	printf("\n======================Teste da Operação hermitiana======================\n");
    for (int l = 0; l < ncolunas; l++)
    {
        for (int c = 0; c < ncolunas; c++)
        {

            mtx_a[l][c].real = l + c;
            mtx_a[l][c].img = l - 2*c;

			mtx_b[l][c].real = 1 + l - 3*c;
			mtx_b[l][c].img = 2*l + 4 - c;

			mtx_c[l][c].real = l - 3*c;
			mtx_c[l][c].img = 1 - l + 2*c;
        }
    }
    mtx_a_h = hermitiana(mtx_a, nlinhas, ncolunas);
	mtx_b_h = hermitiana(mtx_b, nlinhas, ncolunas);
	mtx_c_h = hermitiana(mtx_c, nlinhas, ncolunas);
	

	printf("\nOperando A:\n");

    for (int l = 0 ; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_a[l][c]);
		}
        printf("\n");
	}
	printf("\nMatriz hermitiana de A:\n");

    for (int l = 0 ; l < nlinhas_trans; l++)
	{
		for (int c = 0; c < ncolunas_trans; c++)
		{
			printComplex(mtx_a_h[l][c]);
		}
        printf("\n");
	}
	
	printf("\nOperando B:\n");

    for (int l = 0 ; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_b[l][c]);
		}
        printf("\n");
	}
	
	printf("\nMatriz hermitiana de B:\n");

    for (int l = 0 ; l < nlinhas_trans; l++)
	{
		for (int c = 0; c < ncolunas_trans; c++)
		{
			printComplex(mtx_b_h[l][c]);
		}
        printf("\n");
	}
	
	printf("\nOperando C:\n");

    for (int l = 0 ; l < nlinhas; l++)
	{
		for (int c = 0; c < ncolunas; c++)
		{
			printComplex(mtx_c[l][c]);
		}
        printf("\n");
	}	

	printf("\nMatriz hermitiana de C:\n");

    for (int l = 0 ; l < nlinhas_trans; l++)
	{
		for (int c = 0; c < ncolunas_trans; c++)
		{
			printComplex(mtx_c_h[l][c]);
		}
        printf("\n");
	}
	//Fim do teste da função hermitiana.
}
/**
 * Teste da Função Soma 
*/ 
void teste_soma(void)
{	

	complexo **c1,**c2,**somado1,**c3,**c4,**somado2;
	int nlinhas,ncolunas;
    nlinhas=3;
    ncolunas=3;
    //Alocação de memoria para a matriz:
    c3 = allocateComplexMatrix(nlinhas,ncolunas);
	c4 = allocateComplexMatrix(nlinhas,ncolunas);
	 for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c3[l][c].real = l+1;
				c3[l][c].img = (2*l)-c;
				c4[l][c].real = (5*l)+c;
				c4[l][c].img = l-c;
			}
    }
	printf("\n\n===============================Teste da operação Soma===========================\n");
	printf("\nOperando A:\n");
		for (int l =0 ; l < nlinhas; l++)
		{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c3[l][c]);
				}
			printf("\n");
		}
	printf("\nOperando B:\n");
		for (int l =0 ; l < nlinhas; l++)
			{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c4[l][c]);
				}
			printf("\n");
		}
	printf("\nResultado:\n");
	somado1 = soma(c3,c4,nlinhas,ncolunas);
	for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(somado1[l][c]);
			}
        printf("\n");
	}
	c1 = allocateComplexMatrix(nlinhas,ncolunas);
	c2 = allocateComplexMatrix(nlinhas,ncolunas);
	 for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c1[l][c].real = l-2;
				c1[l][c].img = (2*l)+c;
				c2[l][c].real = (5*l)-c;
				c2[l][c].img = -l-c;
			}
    }
	printf("\nOperando C:\n");
		for (int l =0 ; l < nlinhas; l++)
		{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c1[l][c]);
				}
			printf("\n");
		}
	printf("\nOperando D:\n");
		for (int l =0 ; l < nlinhas; l++)
			{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c2[l][c]);
				}
			printf("\n");
		}
	printf("\nResultado:\n");
	somado2 = soma(c3,c4,nlinhas,ncolunas);
	for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(somado2[l][c]);
			}
        printf("\n");
	}	
}
/**
 * Teste da Função Subtração 
*/ 
void teste_subtracao(void)
{
	complexo **c1,**c2,**sub1,**c3,**c4,**sub2;
	int nlinhas,ncolunas;
    nlinhas=3;
    ncolunas=3;
    //Alocação de memoria para a matriz:
    c1 = allocateComplexMatrix(nlinhas,ncolunas);
	c2 = allocateComplexMatrix(nlinhas,ncolunas);
	 for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c1[l][c].real = l+1;
				c1[l][c].img = (2*l)-c;
				c2[l][c].real = (5*l)+c;
				c2[l][c].img = l-c;
			}
    }
	printf("\n\n===============================Teste da operação Subtracao===========================\n");
	printf("\nOperando A:\n");
		for (int l =0 ; l < nlinhas; l++)
		{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c1[l][c]);
				}
			printf("\n");
		}
	printf("\nOperando B:\n");
		for (int l =0 ; l < nlinhas; l++)
			{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c2[l][c]);
				}
			printf("\n");
		}
	printf("\nResultado:\n");
	sub1 = subtracao(c1,c2,nlinhas,ncolunas);
	for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(sub1[l][c]);
			}
        printf("\n");
	}
	c3 = allocateComplexMatrix(nlinhas,ncolunas);
	c4 = allocateComplexMatrix(nlinhas,ncolunas);
	 for (int l = 0; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				c3[l][c].real = l-2;
				c3[l][c].img = (2*l)+c;
				c4[l][c].real = (5*l)-c;
				c4[l][c].img = -l-c;
			}
    }
	printf("\nOperando C:\n");
		for (int l =0 ; l < nlinhas; l++)
		{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c3[l][c]);
				}
			printf("\n");
		}
	printf("\nOperando D:\n");
		for (int l =0 ; l < nlinhas; l++)
			{
				for (int c = 0; c < ncolunas; c++)
				{
					printComplex(c4[l][c]);
				}
			printf("\n");
		}
	printf("\nResultado:\n");
	sub2 = subtracao(c3,c4,nlinhas,ncolunas);
	for (int l =0 ; l < nlinhas; l++)
		{
			for (int c = 0; c < ncolunas; c++)
			{
				printComplex(sub2[l][c]);
			}
        printf("\n");
	}	
}
/**
 * Teste da Função Produto Interno 
*/ 
void teste_produto_interno(void)
{
    complexo **v1, **v2,**v3,**v4;
    int linhas=3;
    int colunas=1;
    v1 = allocateComplexMatrix(linhas, colunas);
    v2 = allocateComplexMatrix(linhas, colunas);
    for (int l=0; l<linhas; l++)
    {
        v1[l][0].real=3*l+2;
        v1[l][0].img=2*l+4;
        v2[l][0].real=3*l+1;
        v2[l][0].img=2*l-3;
    }
    printf("\n======================Teste do Produto Interno======================\n");
    printf("\nOperando A:\n");
    for (int l=0; l<linhas; l++)
    {
        printComplex(v1[l][0]);
        printf("\n");
    }
    printf("\nOperando B:\n");
    for (int l=0; l<linhas; l++)
    {
        printComplex(v2[l][0]);
        printf("\n");
    }
    printf("\nResultado:\n");
    printComplex(produto_interno(v1,v2,linhas,colunas));
	printf("\n");
    
    v3 = allocateComplexMatrix(linhas, colunas);
    v4 = allocateComplexMatrix(linhas, colunas);
    for (int l=0; l<linhas; l++)
    {
        v3[l][0].real=-2-l;
        v3[l][0].img=2*l-12;
        v4[l][0].real=3*l+7;
        v4[l][0].img=2*l-4;
    }
    printf("\nOperando C:\n");
    for (int l=0; l<linhas; l++)
    {
        printComplex(v3[l][0]);
        printf("\n");
    }
    printf("\nOperando D:\n");
    for (int l=0; l<linhas; l++)
    {
        printComplex(v4[l][0]);
        printf("\n");
    }
    printf("\nResultado:\n");
    printComplex(produto_interno(v3,v4,linhas,colunas));

}
/**
 * Teste da Função Produto Matricial 
*/ 
void teste_produto_matricial(void)
{	
	printf("\n===============================Teste da operação Produto Matricial===========================\n");
	complexo **mtx_a, **mtx_b, **mtx_produto_axb, **mtx_c, **mtx_d, **mtx_produto_cxd, **mtx_e, **mtx_f, **mtx_produto_exf;
	
	int nlinhas_1 = 3, ncolunas_1 = 3, nlinhas_2 = 3, ncolunas_2 = 3;
	
	//Alocação de memória.
	mtx_a = allocateComplexMatrix(nlinhas_1, ncolunas_1);
	mtx_c = allocateComplexMatrix(nlinhas_1, ncolunas_1);
	mtx_e = allocateComplexMatrix(nlinhas_1, ncolunas_1);
	mtx_b = allocateComplexMatrix(nlinhas_2, ncolunas_2);
	mtx_d = allocateComplexMatrix(nlinhas_2, ncolunas_2);
	mtx_f = allocateComplexMatrix(nlinhas_2, ncolunas_2);

	//Preenchimento das matrizes operandas.
	//Matriz A.
	for (int l = 0; l < nlinhas_1; l++)
	{
		for (int c = 0; c < ncolunas_1; c++)
		{
 			mtx_a[l][c].real = 1 + l;
			mtx_a[l][c].img = 1 + c;
		}
	}
	//Matriz B.
	for (int l = 0; l < nlinhas_2; l++)
	{
		for (int c = 0; c < ncolunas_2; c++)
		{
			mtx_b[l][c].real = 0;
			mtx_b[l][c].img = 1 + c;
		}
	}
	//Matriz C.
	for (int l = 0; l < nlinhas_1; l++)
	{
		for (int c = 0; c < ncolunas_1; c++)
		{
 			mtx_c[l][c].real = 1 + 2*l;
			mtx_c[l][c].img = 1 - 2*c;
		}
	}
	//Matriz D.
	for (int l = 0; l < nlinhas_2; l++)
	{
		for (int c = 0; c < ncolunas_2; c++)
		{
			mtx_d[l][c].real = l - c;
			mtx_d[l][c].img = -1 - c;
		}
	}
	//Matriz E.
	for (int l = 0; l < nlinhas_1; l++)
	{
		for (int c = 0; c < ncolunas_1; c++)
		{
 			mtx_e[l][c].real = 2 + l;
			mtx_e[l][c].img = c - 2;
		}
	}
	//Matriz F.
	for (int l = 0; l < nlinhas_2; l++)
	{
		for (int c = 0; c < ncolunas_2; c++)
		{
			mtx_f[l][c].real = 2 + c - l;
			mtx_f[l][c].img = c*l;
		}
	}
	//Primeira Operação.
	
	//Impressão dos Operandos:
	printf("\n\nOperando A:\n");
	//Impressão da matriz A.
	for (int l = 0 ; l < nlinhas_1; l++)
	{
		for (int c = 0; c < ncolunas_1; c++)
		{
			printComplex(mtx_a[l][c]);
		}
        printf("\n");
	}
	
	printf("\n\nOperando B:\n");
	//Impressão da matriz B.
	for (int l = 0 ; l < nlinhas_2; l++)
	{
		for (int c = 0; c < ncolunas_2; c++)
		{
			printComplex(mtx_b[l][c]);
		}
        printf("\n");
	}
	//Chamada da função produto_matricial.
	mtx_produto_axb = produto_matricial(mtx_a, mtx_b, ncolunas_1, nlinhas_2);
	//Impressão do resultado.
	printf("\n\nResultado A x B:\n");

	for (int l = 0 ; l < ncolunas_1; l++)
	{
		for (int c = 0; c < nlinhas_2; c++)
		{
			printComplex(mtx_produto_axb[l][c]);
		}
        printf("\n");
	}

	//Segunda Operação.
	
	//Impressão dos Operandos:
	printf("\n\nOperando C:\n");
	//Impressão da matriz C.
	for (int l = 0 ; l < nlinhas_1; l++)
	{
		for (int c = 0; c < ncolunas_1; c++)
		{
			printComplex(mtx_c[l][c]);
		}
        printf("\n");
	}
	
	printf("\n\nOperando D:\n");
	//Impressão da matriz D.
	for (int l = 0 ; l < nlinhas_2; l++)
	{
		for (int c = 0; c < ncolunas_2; c++)
		{
			printComplex(mtx_d[l][c]);
		}
        printf("\n");
	}
	//Chamada da função produto_matricial
	mtx_produto_cxd = produto_matricial(mtx_c, mtx_d, ncolunas_1, nlinhas_2);
	//Impressão do resultado
	printf("\n\nResultado C x D:\n");

	for (int l = 0 ; l < ncolunas_1; l++)
	{
		for (int c = 0; c < nlinhas_2; c++)
		{
			printComplex(mtx_produto_cxd[l][c]);
		}
        printf("\n");
	}

	//Terceira Operação.

	//Impressão dos Operandos:
	printf("\n\nOperando E:\n");
	//Impressão da matriz E.
	for (int l = 0 ; l < nlinhas_1; l++)
	{
		for (int c = 0; c < ncolunas_1; c++)
		{
			printComplex(mtx_e[l][c]);
		}
        printf("\n");
	}
	
	printf("\n\nOperando F:\n");
	//Impressão da matriz F.
	for (int l = 0 ; l < nlinhas_2; l++)
	{
		for (int c = 0; c < ncolunas_2; c++)
		{
			printComplex(mtx_f[l][c]);
		}
        printf("\n");
	}
	//Chamada da função produto_matricial.
	mtx_produto_exf = produto_matricial(mtx_e, mtx_f, ncolunas_1, nlinhas_2);
	//Impressão do resultado.
	printf("\n\nResultado E x F:\n");

	for (int l = 0 ; l < ncolunas_1; l++)
	{
		for (int c = 0; c < nlinhas_2; c++)
		{
			printComplex(mtx_produto_exf[l][c]);
		}
        printf("\n");
	}
}
/**
 * Impressao de um número complexo
*/ 
void printComplex(complexo c)
{
    printf("%+.6lf %+.6lfj ", c.real, c.img);
}
/**Função: Alocação de memória para uma matriz complexa. */
complexo **allocateComplexMatrix (int linhas, int colunas)
{
    
    complexo** matrix;
	
    matrix = (complexo **) malloc(linhas*sizeof(complexo*));
	
    if (matrix == NULL)
    {
        printf("Memory alocation failed.");
        exit(1);
    }
    //Alocação de memória para cada linha da matriz
    for (int i = 0; i < linhas; i++)
    {
        matrix[i] = (complexo *) malloc(colunas*sizeof(complexo));
        if (matrix[i] == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    return matrix;
}
/**###Função Soma Complexo: 
 A função `soma_complexo` recebe dois números complexos `c1` e `c2` como parâmetros e retorna o resultado da soma desses dois números complexos.
- Dentro da função, uma variável chamada `soma` do tipo `complexo` é declarada para armazenar o resultado da soma.

- A parte real do número resultante é calculada somando as partes reais dos números complexos `c1` e `c2`, ou seja, `soma.real = c1.real + c2.real`.

- Da mesma forma, a parte imaginária do número resultante é calculada somando as partes imaginárias dos números complexos `c1` e `c2`, ou seja, `soma.img = c1.img + c2.img`.

- Por fim, o número complexo `soma`, contendo a parte real e a parte imaginária calculadas, é retornado pela função como resultado da soma dos números complexos `c1` e `c2`.
 * @param[in] c1, c2
 * @param[out] resultado
 * */
complexo soma_complexo(complexo c1, complexo c2)
{
	complexo soma;
	soma.real = c1.real + c2.real;
	soma.img = c1.img + c2.img;
	return soma;
}


void teste_calc_svd(void)
{
	complexo **mtx_a, **mtx_b, **mtx_c, **mtx_d;
	
	int la = 3, ca = 2;
	int lb = 4, cb = 4;
	int lc = 6, cc = 5;
	int ld = 5, cd = 6;
	
	mtx_a = allocateComplexMatrix(la, ca);
	mtx_b = allocateComplexMatrix(lb, cb);
	mtx_c = allocateComplexMatrix(lc, cc);
 	mtx_d = allocateComplexMatrix(ld, cd);

	for (int l = 0; l < la; l++)
	{
		for (int c = 0; c < ca; c++)
		{
			mtx_a[l][c].real = l + c;
			mtx_a[l][c].img = 0;
		}
	}

	for (int l = 0; l < lb; l++)
	{
		for (int c = 0; c < cb; c++)
		{
			mtx_b[l][c].real = l*c + l;
			mtx_b[l][c].img = 0;
		}
	}

	for (int l = 0; l < lc; l++)
	{
		for (int c = 0; c < cc; c++)
		{
			mtx_c[l][c].real = 1 + l - 2*c;
			mtx_c[l][c].img = 0;
		}
	}

	for (int l = 0; l < ld; l++)
	{
		for (int c = 0; c < cd; c++)
		{
			mtx_d[l][c].real = 1 + l - c;
			mtx_d[l][c].img = l - 1 + 3*c;
		}
	}

	calc_svd (mtx_a, la, ca);
	calc_svd (mtx_b, lb, cb);
	calc_svd (mtx_c, lc, cc);
	calc_svd (mtx_d, ld, cd);
}

/**###Função Multicomp: 
 * A função `multcomp` recebe dois números complexos `c1` e `c2` como parâmetros e retorna o resultado da multiplicação desses dois números complexos.
- Dentro da função, uma variável chamada `multiplicado` do tipo `complexo` é declarada para armazenar o resultado da multiplicação. 

- A parte real do número resultante é calculada pela fórmula `(c1.real * c2.real) - (c1.img * c2.img)`, onde `c1.real` e `c2.real` representam as partes reais dos números complexos `c1` e `c2`, e `c1.img` e `c2.img` representam as partes imaginárias dos números complexos `c1` e `c2`. 

- Da mesma forma, a parte imaginária do número resultante é calculada pela fórmula `(c1.real * c2.img) + (c1.img * c2.real)`. 

- Por fim, o número complexo `multiplicado`, contendo a parte real e a parte imaginária calculadas, é retornado pela função como resultado da multiplicação dos números complexos `c1` e `c2`.
 * @param[in] c1, c2
 * @param[out] multiplicado
 * */
complexo multcomp(complexo c1, complexo c2)
{
	complexo multiplicado;
	multiplicado.real = (c1.real*c2.real)-(c1.img*c2.img);
	multiplicado.img = (c1.real*c2.img)+(c1.img*c2.real);
	return multiplicado;
}
