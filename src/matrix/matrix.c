/// @file matrix.c
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
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

/**###Transpose Function: 
 * The `transpose` function implements the matrix transpose operation. This operation consists of swapping 
 * the rows and columns of the input matrix.
- The `transpose` function takes three parameters: `mtx` (a matrix of complex numbers), `linhas` 
(the number of rows in the matrix), and `colunas` (the number of columns in the matrix).
- It declares a variable of type `complexo**` called `matriz`, which will be used to store the transposed matrix.
- Then, the function allocates memory for the `matriz` matrix using the `allocateComplexMatrix` 
function. The `matriz` matrix has the size `colunas x linhas`, that is, the number of columns 
in the input matrix becomes the number of rows in the transposed matrix, and the number of rows 
in the input matrix becomes the number of columns in the transposed matrix.
- The function uses two nested loops to go through each element of the `mtx` matrix. The outer 
loop iterates over the rows and the inner loop iterates over the columns.
- Inside the loop, the function assigns the real value of the input matrix element `mtx[c][l].real` 
to the corresponding element of the `matriz[l][c].real`.
- The function also assigns the value of the imaginary part of the input matrix element `mtx[c][l].img` to 
the corresponding element of the `matriz[l][c].img`.
- After all elements of the `mtx` matrix are processed, the function returns the `matriz` matrix, which 
contains the resulting transposed matrix.
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

/**###Conjugate Function:
 *The `conjugate` function implements the operation of conjugate matrix. This operation consists of inverting the sign 
 of the imaginary part of each element of the input matrix.
- The `conjugate` function takes three parameters: `mtx` (a matrix of complex numbers), `linhas` (the number of 
rows in the matrix), and `colunas` (the number of columns in the matrix).
- It declares a variable of type `complexo**` called `matrix`, which will be used to store the conjugate matrix.
- Then, the function allocates memory for the `matrix` using the `allocateComplexMatrix` function. The `matrix` 
has the same size as the input matrix `mtx`.
- The function uses two nested loops to go through each element of the `mtx` matrix. The outer loop iterates 
over the rows and the inner loop iterates over the columns.
- Inside the loop, the function assigns the real value of the input matrix element `mtx[l][c].real` to the 
corresponding element of the `matrix[l][c].real`, preserving the same value.
- The function inverts the sign of the imaginary part of the input matrix element, multiplying it by -1: 
`matrix[l][c].img = -mtx[l][c].img`.
- After all elements of the `mtx` matrix are processed, the function returns the `matrix`, which contains 
the resulting conjugate matrix. 
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

/**###Hermitian Function: 
 * The `hermitian` function implements the operation of hermitian matrix. This operation consists of obtaining the conjugate matrix of the input matrix and then calculating the transpose of this conjugate matrix. The resulting matrix is a hermitian matrix.
- The `hermitian` function takes three parameters: `mtx` (a matrix of complex numbers), `linhas` (the number of rows in the matrix), and `colunas` (the number of columns in the matrix).
It declares two variables of type `complexo**`: `matriz` and `matriz_h`.
- Then, the function allocates memory for two matrices using the `allocateComplexMatrix` function. The first matrix (`matriz`) has the same size as the input matrix, while the second matrix (`matriz_h`), also has the same size.
- The function calls the `conjugate` function passing the input matrix `mtx`, `linhas`, and `colunas`. This function returns the conjugate matrix of the input matrix.
- The result of the `conjugate` function is assigned to the `matriz` matrix.
- Then, the function calls the `transpose` function passing the `matriz` matrix, `linhas`, and `colunas`. This function returns the transpose of the matrix passed as an argument.
The result of the `transpose` function is assigned to the `matriz_h` matrix.
- Finally, the function returns the `matriz_h` matrix, which is the resulting hermitian matrix.
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
/**###Sum Function: 
 * The `sum` function implements the operation of matrix addition. This operation consists of adding two or more matrices, however in the following example only with two matrices.
- The `sum` function takes four parameters: `mtx_a` (a "a" matrix of complex numbers),`mtx_b` (a "b" matrix of complex numbers), `linhas` (the number of rows in the matrix), and `colunas` (the number of columns in the matrix).
- It declares a variable of type `complexo**` called `matriz`, which will be used to store the matrix of the sum operation.
- Then, the function allocates memory for the `matriz` matrix using the `allocateComplexMatrix` function. The `matriz` matrix has the same size as the "a" and "b" matrices.
- The function uses two nested loops to go through each element of the `mtx` matrix. The outer loop iterates over the rows and the inner loop iterates over the columns.
- Inside the loop, the function assigns the real value of the sum of the input matrix elements `mtx_a[c][l].real` and `mtx_b[c][l].real`, to the corresponding element of the `matriz[l][c].real`.
- The function also assigns in an analogous way the sum of the value of the imaginary part of the input matrix elements `mtx_a[c][l].img` and `mtx_b[c][l].real`, to the corresponding element of the `matriz[l][c].img`.
- After all elements of the `mtx_a` and 'mtx_b' matrices are processed, the function returns the `matriz` matrix, which contains the matrix of the sum function.
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
/**###Subtraction Function: 
 * The `subtraction` function implements the operation of matrix subtraction. This operation consists of subtracting two or more matrices, however in the following example as in the above only with two matrices.
- The `subtraction` function takes four parameters: `mtx_a` (a "a" matrix of complex numbers),`mtx_b` (a "b" matrix of complex numbers), `linhas` (the number of rows in the matrix), and `colunas` (the number of columns in the matrix).
- It declares a variable of type `complexo**` called `matriz`, which will be used to store the matrix of the subtraction operation.
- Then, the function allocates memory for the `matriz` matrix using the `allocateComplexMatrix` function. The `matriz` matrix has the same size as the "a" and "b" matrices.
- The function uses two nested loops to go through each element of the `mtx` matrix. The outer loop iterates over the rows and the inner loop iterates over the columns.
- Inside the loop, the function assigns the real value of the subtraction of the input matrix elements `mtx_a[c][l].real` and `mtx_b[c][l].real`, to the corresponding element of the `matriz[l][c].real`.
- The function also assigns in an analogous way the subtraction of the value of the imaginary part of the input matrix elements `mtx_a[c][l].img` and `mtx_b[c][l].real`, to the corresponding element of the `matriz[l][c].img`.
- After all elements of the `mtx_a` and 'mtx_b' matrices are processed, the function returns the `matriz` matrix, which contains the matrix of the subtraction function.
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
/**###Inner Product Function: 
 * The `inner product` function implements the operation of the product of two complex vectors.
- The `inner product` function takes four parameters: `v1` (a "v1" vector of complex numbers),`v2` (a "v2" vector of complex numbers), `linhas` (the number of rows of the vectors), and `colunas` (the number of columns of the vectors).
- It declares a variable of type `complexo` called `resultado` with initial values (0,0), which will be used to store the result of the inner product.
- Then, the function checks if the number of columns is equal to 1. Otherwise, it displays an error message that the product cannot be performed due to the incompatibility of vectors, the function ends with the 'exit(1)' function
- After the check, the function starts a loop that goes through the rows of the vectors, in the loop the multiplication of 'v1[l][0]' and 'v2[l][0]' is done through the 'multcomp' function which is stored in the 'temp' variable.
- After the multiplication, the 'temp' variable is added to the 'resultado' variable using the 'multcomp' function.
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
/**###Matrix Product Function: 
 * The `matrix product` function implements the operation of the product of two complex matrices.
- The `matrix product` function takes four parameters: `mtx_a` (a "a" matrix of complex numbers),`mtx_b` (a "b" matrix of complex numbers), `linhas` (the number of rows of the matrices), and `colunas` (the number of columns of the matrices).
- It declares a variable of type `complexo` called `matriz`, which will be used to store the result of the matrix product.
- Then, the function validates the operation by checking if the number of columns of the 'a' matrix is equal to the number of rows of the 'b' matrix. If not, it displays an error message that the product cannot be performed due to the incompatibility of matrices and ends the program with the 'exit(1)' function.
- After the check, the function starts a loop to go through the cells of the resulting matrix, for each cell an accumulator is started with the value zero.
- Inside the loop, another loop is performed to calculate the sum of the product of matrix a and matrix b, such operation is done from the 'multcomp' function.
- Then the 'soma_complexo' function is called to add the result to the accumulator.
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
/**###Scalar Product Function: 
 * The `scalar product` function implements the operation of a matrix by any scalar. 
- The scalar product function takes four parameters: `mtx` (a matrix of complex numbers), `linhas` (the number of rows in the matrix), `colunas` (the number of columns in the matrix), and `k` (integer scalar).
- It declares a variable of type `complexo**`: `matriz`.
- Then, the function allocates memory for the matrix using the `allocateComplexMatrix` function. 
- The function uses two nested loops to go through each element of the `mtx` matrix. The outer loop iterates over the rows and the inner loop iterates over the columns.
- Inside the loop, the function assigns the real value of the input matrix element `mtx[c][l].real`, multiplied by the integer 'k', to the corresponding element of the `matriz[l][c].real`.
- The function also assigns in an analogous way, the value of the multiplication of the scalar by the imaginary part of the input matrix element `mtx[c][l].img`, to the corresponding element of the `matriz[l][c].img`.
- After all elements of the `mtx` matrix are processed, the function returns the `matriz` matrix.

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

/**###SVD Calculation Function: 
 * The `SVD Calculation` function implements the SVD operator.
- The `SVD Calculation` function takes three parameters: `mtx` (a matrix of complex numbers),`linhas` (number of rows in the matrix), `colunas` (the number of columns in the matrix).
- The SVD was implemented only for real matrices, therefore, before starting the SVD calculation, the function tests if there is an imaginary part, if there is a warning appears on the screen informing that there is no support for complex matrices.
- There is no return in the function, it prints on the screen the value of the SVD of the matrix `mtx`.
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
/**###Complex Sum Function: 
 The `complex_sum` function takes two complex numbers `c1` and `c2` as parameters and returns the result of the sum of these two complex numbers.
- Inside the function, a variable named `sum` of type `complex` is declared to store the result of the sum.

- The real part of the resulting number is calculated by adding the real parts of the complex numbers `c1` and `c2`, that is, `sum.real = c1.real + c2.real`.

- Similarly, the imaginary part of the resulting number is calculated by adding the imaginary parts of the complex numbers `c1` and `c2`, that is, `sum.img = c1.img + c2.img`.

- Finally, the complex number `sum`, containing the calculated real and imaginary parts, is returned by the function as the result of the sum of the complex numbers `c1` and `c2`.
 * @param[in] c1, c2
 * @param[out] result
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

/**###Multicomp Function: 
 * The `multcomp` function takes two complex numbers `c1` and `c2` as parameters and returns the result of the multiplication of these two complex numbers.
- Inside the function, a variable named `multiplied` of type `complex` is declared to store the result of the multiplication. 

- The real part of the resulting number is calculated by the formula `(c1.real * c2.real) - (c1.img * c2.img)`, where `c1.real` and `c2.real` represent the real parts of the complex numbers `c1` and `c2`, and `c1.img` and `c2.img` represent the imaginary parts of the complex numbers `c1` and `c2`. 

- Similarly, the imaginary part of the resulting number is calculated by the formula `(c1.real * c2.img) + (c1.img * c2.real)`. 

- Finally, the complex number `multiplied`, containing the calculated real and imaginary parts, is returned by the function as the result of the multiplication of the complex numbers `c1` and `c2`.
 * @param[in] c1, c2
 * @param[out] multiplied
 * */
complexo multcomp(complexo c1, complexo c2)
{
	complexo multiplicado;
	multiplicado.real = (c1.real*c2.real)-(c1.img*c2.img);
	multiplicado.img = (c1.real*c2.img)+(c1.img*c2.real);
	return multiplicado;
}
