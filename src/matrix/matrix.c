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
