/// @file pds_telecom.c

#include <stdio.h>
#include <stdlib.h>
#include "../matrix/matrix.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <libgen.h> 
#include <stdbool.h> 
#include <unistd.h>

double calculate_capacity(double snr_dB) {
    double snr = pow(10,snr_dB/10);
    return log2(1 + snr);  // Capacity in bits per symbol
}

double calculate_EVM(complexo **original_signal, complexo **received_signal, int Nstream, long int Nsymbol) {
    double error_power = 0.0;
    double signal_power = 0.0;

    for (int i = 0; i < Nstream; i++) {
        for (int j = 0; j < Nsymbol/Nstream; j++) {
            double real_diff = original_signal[i][j].real - received_signal[i][j].real;
            double img_diff = original_signal[i][j].img - received_signal[i][j].img;
            error_power += real_diff * real_diff + img_diff * img_diff;
            signal_power += received_signal[i][j].real * received_signal[i][j].real + received_signal[i][j].img * received_signal[i][j].img;
        }
    }

    if (signal_power == 0) {
        return INFINITY; // If there's no signal, return infinity
    } else {
        double evm = sqrt(error_power / signal_power); // Calculate EVM
        return 20 * log10(evm); // Convert EVM to dB
    }
}

double calculate_SNR(complexo **original_signal, complexo **received_signal, int Nstream, long int Nsymbol) {
    double signal_power = 0.0;
    double noise_power = 0.0;

    for (int i = 0; i < Nstream; i++) {
        for (int j = 0; j < Nsymbol/Nstream; j++) {
            double real_diff = original_signal[i][j].real - received_signal[i][j].real;
            double img_diff = original_signal[i][j].img - received_signal[i][j].img;
            noise_power += real_diff * real_diff + img_diff * img_diff;
            signal_power += received_signal[i][j].real * received_signal[i][j].real + received_signal[i][j].img * received_signal[i][j].img;
        }
    }

    if (noise_power == 0) {
        return INFINITY; // If there's no noise, return infinity
    } else {
        return 10 * log10(signal_power / noise_power); // Calculate SNR in dB
    }
}

void getUserInput(int* Nr, int* Nt, int* r) {
    printf("Digite o valor para Nr: ");
    scanf("%d", Nr);

    printf("Digite o valor para Nt: ");
    scanf("%d", Nt);

    printf("Digite o valor para r: ");
    scanf("%d", r);
}
/**
 * @brief Lê os dados de um arquivo e os converte em um array de inteiros.
 *
 * Esta função lê os dados binários de um arquivo e os converte em um array de inteiros,
 * em que cada inteiro representa 2 bits de dados. A função aloca memória dinamicamente
 * para o array de inteiros e retorna um ponteiro para o início do array.
 *
 * @param fp Ponteiro para o arquivo a ser lido.
 * @param numBytes O número de bytes a serem lidos do arquivo.
 * @return Um ponteiro para o array de inteiros que contém os dados convertidos, ou NULL
 *         em caso de erro na alocação de memória.
 *
 * @note O chamador é responsável por liberar a memória alocada para o array de inteiros
 *       quando não precisar mais dele, usando a função free().
 */
int * tx_data_read(FILE *fp, long int numBytes){
    // Aloca memória para o array de inteiros
    int *s = (int *)malloc(numBytes * 4 * sizeof(int));
    if (s == NULL) {
        printf("Erro na alocação de memória\n");
        fclose(fp);
        return (int *)1;
    }
    // Lê os bytes do arquivo e converte em inteiros de 2 bits
    for (int i = 0; i < numBytes; i++) {
        char byte;
        fread(&byte, sizeof(byte), 1, fp);

        for (int j = 0; j <= 7; j=j+2) {
            int bit = (byte >> j) & 3;
            s[(i*4) + (j/2)]= bit;
        }
        
    }
    return s;
}

/**
 * @brief Realiza o preenchimento (padding) dos dados com zeros.
 *
 * Esta função realiza o preenchimento (padding) dos dados com zeros para garantir
 * que o tamanho do array de inteiros seja um múltiplo inteiro do número de streams (Nstream).
 * Se o número de bytes (numBytes) for um múltiplo exato do número de streams, a função
 * retorna o array de inteiros original sem alterações. Caso contrário, a função realoca
 * memória para o array de inteiros e preenche o espaço adicional com zeros.
 *
 * @param s Ponteiro para o array de inteiros contendo os dados.
 * @param numBytes O número de bytes original antes do preenchimento.
 * @param Nstream O número de streams para o qual o tamanho do array deve ser um múltiplo.
 * @return Um ponteiro para o array de inteiros com o preenchimento realizado, ou NULL
 *         em caso de erro na alocação de memória.
 */
int * tx_data_padding(int* s, long int numBytes, int Npadding){
    // Verifica se o número de bytes é um múltiplo do número de streams.
    if(Npadding == 0){
        return s;
    }
    else{
        // Alocação de memória para o array de inteiros com o espaço adicional necessário.
        int *resized_s = (int*) malloc((numBytes*4 + Npadding)*sizeof(int));
        // Preenche o espaço adicional com nulos - representados pelo inteiro 4 -.
        for (long int i = 0; i < numBytes*4; i++)
            resized_s[i] = s[i];
        for(long int i = numBytes*4; i < ((numBytes*4) + Npadding); i++){
            resized_s[i] = 4;
        }
        return resized_s;
    }     
}

/**
 * @brief Mapeia os dados binários em uma sequência de símbolos QAM.
 *
 * Esta função mapeia os dados binários em uma sequência de símbolos QAM (Quadrature
 * Amplitude Modulation) representados por números complexos. A função aloca memória
 * dinamicamente para o vetor de complexos e retorna um ponteiro para o vetor.
 *
 * @param s Ponteiro para o array de inteiros contendo os dados binários.
 * @param numBytes O número de bytes contidos no array de inteiros.
 * @return Um ponteiro para o vetor de complexos que contém os símbolos QAM mapeados, ou NULL
 *         em caso de erro na alocação de memória.
 */

complexo* tx_qam_mapper(int *s, long int numQAM){
    // Aloca memória para o vetor de complexos
    complexo *c1 = (complexo *)malloc(numQAM * sizeof(complexo));   
    if (c1 == NULL) {
        printf("Erro na alocação de memória\n");
        return NULL;
    }
    for(int i= 0; i<numQAM;i++){
        switch(s[i]){
            case 0:
                c1[i].real = -1;
                c1[i].img = 1;
                break;
            case 1:
                c1[i].real = -1;
                c1[i].img = -1;
                break;
            case 2:
                c1[i].real = 1;
                c1[i].img = 1;
                break;
            case 3:
                c1[i].real = 1;
                c1[i].img = -1;
                break;
            default:
                c1[i].real = 0;
                c1[i].img = 0;
        }
    }
    return c1;
}

/**
 * @brief Mapeia os dados de um vetor para uma matriz de complexos.
 *
 * Esta função mapeia os dados de um vetor de complexos em uma matriz de complexos,
 * onde cada posição da matriz representa um stream de transmissão. A função aloca
 * memória dinamicamente para a matriz de complexos e retorna um ponteiro para a matriz.
 *
 * @param v Ponteiro para o vetor de complexos contendo os dados a serem mapeados.
 * @param Nstream O número de streams de transmissão.
 * @param numBytes O número de bytes contidos no vetor de complexos.
 * @return Um ponteiro para a matriz de complexos que contém os dados mapeados, ou NULL
 *         em caso de erro na alocação de memória.
 */

complexo ** tx_layer_mapper(complexo *v, int Nstream, long int Nsymbol){
    // Aloca memória para a matriz de complexos
    complexo **mtx_stream;
    mtx_stream = (complexo**) malloc(Nstream*sizeof(complexo*));

    for(int i = 0; i < Nstream; i++){
        mtx_stream[i] = (complexo *) malloc((Nsymbol/Nstream)*sizeof(complexo));
    }
    // Mapeia os dados do vetor para a matriz de complexos
    for (int i = 0; i < Nsymbol; i++){
        mtx_stream[i%Nstream][i/Nstream] = v[i];
    }
    return mtx_stream;
}
/**
 * @brief Mapeia os dados de um vetor para uma matriz de complexos.
 *
 * Esta função mapeia os dados de um vetor de complexos em uma matriz de complexos,
 * onde cada posição da matriz representa um stream de transmissão. A função aloca
 * memória dinamicamente para a matriz de complexos e retorna um ponteiro para a matriz.
 *
 * @param mtx_stream Ponteiro para o vetor de complexos contendo os dados a serem mapeados.
 * @param Nstream O número de streams de transmissão.
 * @param numBytes O número de bytes contidos no vetor de complexos.
 * @return Um ponteiro para a matriz de complexos que contém os dados mapeados, ou NULL
 *         em caso de erro na alocação de memória.
 */

complexo* rx_layer_demapper(complexo** mtx_stream, int Nstream, long int numBytes) {
    // Aloca memória para o vetor de complexos
    complexo* v = (complexo*) malloc(numBytes * sizeof(complexo));
    if (v == NULL) {
        printf("Erro na alocação de memória.\n");
        return NULL;
    }

    // Desmapeia os dados da matriz para o vetor
    for (int i = 0; i < numBytes; i++) {
        v[i] = mtx_stream[i % Nstream][i / Nstream];
    }

    return v;
}

/**
 * @brief Desmapeia os símbolos QAM para dados binários.
 *
 * Esta função recebe um vetor de complexos representando símbolos QAM e realiza o desmapeamento
 * desses símbolos para dados binários. Cada símbolo QAM é associado a um valor binário, de acordo
 * com a seguinte tabela:
 * - (-1, 1)  -> 0
 * - (-1, -1) -> 1
 * - (1, 1)   -> 2
 * - (1, -1)  -> 3
 * - Outros   -> 4
 *
 * @param vmap Vetor de complexos representando os símbolos QAM.
 * @param numQAM O número de símbolos QAM no vetor.
 *
 * @return Um vetor de inteiros contendo os dados binários desmapeados dos símbolos QAM.
 *         O chamador é responsável por liberar a memória alocada utilizando a função free().
 */
int* rx_qam_demapper(complexo *vmap, long int numQAM) {
    // Aloca memória para o vetor de inteiros
    int *vetor = (int *)malloc(numQAM * sizeof(int));
    if (vetor == NULL) {
        printf("Erro na alocação de memória\n");
        return (int *)1;
    }

    // Desmapeia os símbolos QAM para dados binários
    for (int i = 0; i < numQAM; i++) {
        if (vmap[i].real == -1.0 && vmap[i].img == 1.0) {
            vetor[i] = 0;
        } else if (vmap[i].real == -1.0 && vmap[i].img == -1.0) {
            vetor[i] = 1;
        } else if (vmap[i].real == 1.0 && vmap[i].img == 1.0) {
            vetor[i] = 2;
        } else if (vmap[i].real == 1.0 && vmap[i].img == -1.0) {
            vetor[i] = 3;
        } else {
            vetor[i] = 4;
        }
    }

    return vetor;
}

/**
 * @brief Retira os símbolos "nulos" que foram preenchidos (padding).
 *
 * Esta função desfaz o processamento feito pela a função data_padding
 * fazendo com o que o nosso vetor volte ao tamanho original
 * @param s Ponteiro para o array de inteiros contendo os dados.
 * @param numBytes O número de bytes original antes do preenchimento.
 * @param Nstream O número de streams para o qual o tamanho do array deve ser um múltiplo.
 * @return Um ponteiro para o array de inteiros com os valores originais
 *         
 */

int *rx_data_depadding(int *s, long int numBytes, int Nstream) {
    // Verifica se o número de bytes é um múltiplo do número de streams
    if ((4*numBytes) % Nstream == 0) {
        return s;
    } 
    else {
        // Cria um novo array para armazenar os valores originais
        int *resized_s = (int *)malloc(4*numBytes*sizeof(int));

        // Copia os valores originais de volta para o array redimensionado
        for (long int i = 0; i < 4*numBytes; i++) {
            resized_s[i] = s[i];
        }
        return resized_s;
    }
}
/**
 * @brief Recupera os bytes originais.
 *
 * Esta função recupera os bytes originais a cada 4 dígitos
 * realizando o processo inverso da função data_read, pegando o vetor de inteiros s
 * e decodificando em bytes de caracteres para gerar o arquivo fileName
 * @param s Ponteiro para o array de inteiros contendo os dados.
 * @param numBytes O número de bytes original antes do preenchimento.
 * @param Nstream O número de streams para o qual o tamanho do array deve ser um múltiplo.
 * @return Um ponteiro para o array de inteiros com os valores originais
 *         
 */

void rx_data_write(int* s, long int numBytes, char* fileName) {
    FILE* out = fopen(fileName, "wb");
    if (out == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", fileName);
        return;
    } else {
        printf("Arquivo %s criado com sucesso.\n", fileName);
    }

    for (int i = 0; i < numBytes; i++) {
        unsigned char byte = 0;
        for (int j = 0; j < 4; j++) {
            unsigned int bit = s[(i * 4) + j];
            byte |= (bit << (2 * j));
        }
        fwrite(&byte, sizeof(byte), 1, out);
    }

    fclose(out);
}

/**
 * @brief Realiza a multiplicação de duas matrizes complexas.
 *
 * Esta função realiza a multiplicação de duas matrizes complexas `mtx_a` e `mtx_b`,
 * resultando em uma nova matriz `matriz`. A validação da operação de multiplicação
 * é feita verificando se o número de colunas da matriz `mtx_a` é igual ao número
 * de linhas da matriz `mtx_b`. Caso não seja compatível, a função exibe uma mensagem
 * de erro e encerra o programa.
 *
 * @param mtx_a A primeira matriz complexa a ser multiplicada.
 * @param mtx_b A segunda matriz complexa a ser multiplicada.
 * @param linhas_a O número de linhas da matriz `mtx_a`.
 * @param colunas_a O número de colunas da matriz `mtx_a`.
 * @param linhas_b O número de linhas da matriz `mtx_b`.
 * @param colunas_b O número de colunas da matriz `mtx_b`.
 *
 * @return Uma nova matriz complexa resultante da multiplicação de `mtx_a` e `mtx_b`.
 *         O chamador é responsável por liberar a memória alocada utilizando a função free().
 *
 * @note Esta função assume que as matrizes `mtx_a` e `mtx_b` foram alocadas corretamente
 *       e têm dimensões compatíveis para a multiplicação.
 */

complexo** produto_matricial_geral(complexo** mtx_a, complexo** mtx_b, int linhas_a, int colunas_a, int linhas_b, int colunas_b)
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
}

/**
 * @brief Gera uma matriz complexa representando um canal de transferência.
 *
 * Esta função gera uma matriz complexa que representa um canal de transferência entre antenas
 * transmissoras e antenas receptoras. Os elementos da matriz são números complexos aleatórios
 * dentro do intervalo [minValue, maxValue], com a parte imaginária definida como zero.
 *
 * @param Nr O número de antenas receptoras.
 * @param Nt O número de antenas transmissoras.
 * @param minValue O valor mínimo para os elementos da matriz.
 * @param maxValue O valor máximo para os elementos da matriz.
 * @param sigma O valor do desvio padrão da criação do canal
 *
 * @return Uma matriz complexa representando o canal de transferência gerado.
 *         O chamador é responsável por liberar a memória alocada utilizando a função free().
 */
complexo ** channel_gen(int Nr, int Nt, double sigma){
    complexo** H = (complexo **) malloc(Nr * sizeof(complexo*));
    if (H == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < Nr; i++) {
        H[i] = (complexo *) malloc(Nt * sizeof(complexo));
        if (H[i] == NULL) {
            printf("Memory allocation failed\n");
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(H[j]);
            }
            free(H);
            return NULL;
        }
    }
    gsl_rng * r = gsl_rng_alloc (gsl_rng_default);
    sigma = 1.0;

    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            gsl_rng_set(r, rand()%10000);
            H[i][j].real = gsl_ran_gaussian(r, sigma);
            H[i][j].img = 0;
        }
    }

    return H;
}
/**
 * @brief Gera uma matriz de complexos representando o ruído do canal de comunicação.
 *
 * Esta função gera uma matriz de complexos representando o ruído do canal de comunicação.
 * A matriz resultante possui dimensões Nr x Nt, onde Nr é o número de antenas receptoras
 * e Nt é o número de antenas transmissoras. Os valores dos elementos da matriz são gerados
 * aleatoriamente dentro do intervalo definido por minValue e maxValue.
 *
 * @param Nr Número de antenas receptoras.
 * @param Nt Número de antenas transmissoras.
 * @param sigma Valor do desvio padrão do ruído.
 *
 * @return A matriz de complexos representando o ruído do canal de comunicação.
 */

complexo ** channel_rd_gen(int Nr, int Nt, double sigma){
    complexo** H = (complexo **) malloc(Nr * sizeof(complexo*));
    if (H == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < Nr; i++) {
        H[i] = (complexo *) malloc(Nt * sizeof(complexo));
        if (H[i] == NULL) {
            printf("Memory allocation failed\n");
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(H[j]);
            }
            free(H);
            return NULL;
        }
    }
    gsl_rng * r = gsl_rng_alloc (gsl_rng_default);
    
    sigma = 1.0;

    // TO DO: noise~N(0,N0)
    for (int i = 0; i < Nr; i++) {
        for (int j = 0; j < Nt; j++) {
            gsl_rng_set(r, rand()%10000);
            H[i][j].real = gsl_ran_gaussian(r, sigma);
            H[i][j].img = gsl_ran_gaussian(r, sigma);
        }
    }

    return H;
}

/**
 * @brief Realiza a decomposição em valores singulares (SVD) de uma matriz transposta.
 *
 * Esta função realiza a decomposição em valores singulares (SVD) de uma matriz transposta,
 * representada por uma matriz de complexos. A função utiliza apenas a parte real dos
 * elementos da matriz para realizar o cálculo do SVD. A função aloca memória dinamicamente
 * para as matrizes U, V e o vetor S, e armazena os resultados da decomposição nas matrizes
 * Uh, Sh e Vh.
 *
 * @param H Matriz transposta de complexos a ser decomposta.
 * @param Uh Matriz U resultante da decomposição, contendo os autovetores à esquerda.
 * @param Sh Matriz S resultante da decomposição, contendo os valores singulares na diagonal.
 * @param Vh Matriz V resultante da decomposição, contendo os autovetores à direita.
 * @param Tlinhas O número de linhas da matriz transposta H.
 * @param Tcolunas O número de colunas da matriz transposta H.
 *
 * @note Esta função considera apenas a parte real dos elementos da matriz H para o cálculo do SVD.
 *       A função imprime um aviso se elementos complexos forem detectados na matriz H, mas ignora
 *       a parte imaginária para o cálculo.
 *
 * @remark A função `transposed_channel_svd` é similar a função `square_channel_svd`, porém há uma diferença crucial
 *         entre elas. A função `square_channel_svd` recebe uma matriz quadrada como parâmetro, enquanto a função
 *         `transposed_channel_svd` recebe a matriz transposta como parâmetro. A matriz transposta é obtida trocando
 *         as linhas pelas colunas da matriz original. Portanto, enquanto a função `square_channel_svd` realiza a
 *         decomposição em valores singulares (SVD) de uma matriz quadrada, a função `transposed_channel_svd` realiza
 *         a decomposição SVD da matriz transposta.
 */

void transposed_channel_svd(complexo **H, complexo **Uh, complexo **Sh, complexo **Vh, int Tlinhas, int Tcolunas){
    for (int l = 0; l < Tlinhas; l++){
		for (int c = 0; c < Tcolunas; c++){
			if (H[l][c].img != 0){
				printf("Warning: complex matrix injected as parameter, fuction will use only real part from matrix\n");
				break;
			}
		}
	}
    gsl_matrix * U = gsl_matrix_alloc(Tlinhas, Tcolunas); // Matriz U lxc
    gsl_matrix * V = gsl_matrix_alloc(Tcolunas, Tcolunas); // Matriz V cxc
    gsl_vector * S = gsl_vector_alloc(Tcolunas); // Vetor S cx1
    gsl_vector * work = gsl_vector_alloc(Tcolunas);
    
    for(int l=0; l<Tlinhas; l++){
        for(int c=0; c<Tcolunas; c++){
           // printf("%+.1f ", H[l][c].real);
            gsl_matrix_set(U, l, c, H[l][c].real);
        }
        //printf("\n");
    }

    gsl_linalg_SV_decomp(U, V, S, work);
    for(int l=0; l<Tlinhas; l++){
        for(int c=0; c<Tcolunas; c++){
            Uh[l][c].real = gsl_matrix_get(U, l, c);
            Uh[l][c].img = 0;
            //printf("%f ", gsl_matrix_get(U, l, c));
        }
       // printf("\n");
    }
    for(int l=0; l<Tcolunas; l++){
        for(int c=0; c<Tcolunas; c++){
            Vh[l][c].real = gsl_matrix_get(V, l, c);
            Vh[l][c].img = 0;
           // printf("%f ", gsl_matrix_get(V, l, c));
        }
        //printf("\n");
    }
    for (int l = 0; l < Tcolunas; l++){
        for (int c = 0; c < Tcolunas; c++){
            if (l == c){
                Sh[l][c].real = gsl_vector_get(S,c);
                Sh[l][c].img = 0;
            }else{
                Sh[l][c].real = 0;
                Sh[l][c].img = 0;
            }
        }
    }
}

/**
 * @brief Realiza a decomposição em valores singulares (SVD) de uma matriz quadrada.
 *
 * Esta função realiza a decomposição em valores singulares (SVD) de uma matriz quadrada,
 * representada por uma matriz de complexos. A função utiliza apenas a parte real dos
 * elementos da matriz para realizar o cálculo do SVD. A função aloca memória dinamicamente
 * para as matrizes U, V e o vetor S, e armazena os resultados da decomposição nas matrizes
 * Uh, Sh e Vh.
 *
 * @param H Matriz quadrada de complexos a ser decomposta.
 * @param Uh Matriz U resultante da decomposição, contendo os autovetores à esquerda.
 * @param Sh Matriz S resultante da decomposição, contendo os valores singulares na diagonal.
 * @param Vh Matriz V resultante da decomposição, contendo os autovetores à direita.
 * @param linhas O número de linhas da matriz H.
 * @param colunas O número de colunas da matriz H.
 *
 * @note Esta função considera apenas a parte real dos elementos da matriz H para o cálculo do SVD.
 *       A função imprime um aviso se elementos complexos forem detectados na matriz H, mas ignora
 *       a parte imaginária para o cálculo.
 */
void square_channel_svd(complexo **H,  complexo **Uh, complexo **Sh, complexo **Vh, int linhas, int colunas) {
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            if (H[l][c].img != 0) {
                printf("Warning: complex matrix injected as parameter, function will use only real part from matrix\n");
                break;
            }
        }
    }
    
    gsl_matrix *U = gsl_matrix_alloc(linhas, colunas); // Matriz U lxc
    gsl_matrix *V = gsl_matrix_alloc(colunas, colunas); // Matriz V cxc
    gsl_vector *S = gsl_vector_alloc(colunas); // Vetor S cx1
    gsl_vector *work = gsl_vector_alloc(colunas);
    
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            gsl_matrix_set(U, l, c, H[l][c].real);
        }
    }

    gsl_linalg_SV_decomp(U, V, S, work);
    
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            Uh[l][c].real = gsl_matrix_get(U, l, c);
            Uh[l][c].img = 0;
        }
    }
    
    for (int l = 0; l < colunas; l++) {
        for (int c = 0; c < colunas; c++) {
            Vh[l][c].real = gsl_matrix_get(V, l, c);
            Vh[l][c].img = 0;
        }
    }
    
    for (int l = 0; l < colunas; l++) {
        for (int c = 0; c < colunas; c++) {
            if (l == c) {
                Sh[l][c].real = gsl_vector_get(S, c);
                Sh[l][c].img = 0;
            } else {
                Sh[l][c].real = 0;
                Sh[l][c].img = 0;
            }
        }
    }
}
/**
 * @brief Realiza a multiplicação dos símbolos das streams pela matriz V resultante da decomposição SVD
 *
 * Esta função utiliza a matriz V e multiplica pelo o vetor x que estamos transmitindo
 * gerando o vetor precodificado xp .
 *
 * @param V Matriz V que foi alocada na main.
 * @param x Vetor x que está sendo transmitido
 * @param Vlinhas O número de linhas da matriz V.
 * @param Vcolunas O número de colunas da matriz V.
 * @param xlinhas O número de linhas do vetor x.
 * @param xcolunas O número de colunas do vetor x.
 *
 * @return Retorna o vetor xp
 */

complexo ** tx_precoder(complexo ** V, complexo **x, int Vlinhas, int Vcolunas, int xlinhas, int xcolunas){
    complexo **xp = produto_matricial_geral(V, x, Vlinhas, Vcolunas, xlinhas, xcolunas);
    return xp;
}

/**
 * @brief Realiza a transmissão do sinal através do canal de comunicação.
 *
 * Esta função realiza a transmissão do sinal de entrada xp através do canal de comunicação
 * representado pela matriz H. O resultado da transmissão é calculado multiplicando a matriz H
 * pelo vetor xp. Além disso, um ruído Rd é adicionado ao sinal transmitido para simular as
 * características do canal de comunicação.
 *
 * @param H Matriz representando o canal de comunicação.
 * @param xp Vetor de entrada a ser transmitido pelo canal.
 * @param Hlinhas O número de linhas da matriz H.
 * @param Hcolunas O número de colunas da matriz H.
 * @param xpLinhas O número de linhas do vetor xp.
 * @param xpColunas O número de colunas do vetor xp.
 * @param r O valor que define o intervalo do ruído a ser adicionado: 0 para [-0.001, 0.001],
 *          1 para [-0.01, 0.01], 2 para [-0.5, 0.5], 3 para [-1, 1].
 *
 * @return A matriz resultante da transmissão do sinal pelo canal, acrescido do ruído.
 */

complexo ** channel_transmission(complexo ** H, complexo ** xp, int Hlinhas, int Hcolunas, int xpLinhas, int xpColunas, int r){
    complexo **xh = produto_matricial_geral(H, xp, Hlinhas, Hcolunas, xpLinhas, xpColunas);
    if (xh == NULL) {
        printf("Erro na multiplicação de matrizes\n");
        return NULL;
    }

    complexo ** Rd;
    switch(r){
        case 0:
            Rd = channel_rd_gen(Hlinhas, xpColunas, 0.001);
            break;
        case 1:
            Rd = channel_rd_gen(Hlinhas, xpColunas, 0.01);
            break;
        case 2:
            Rd = channel_rd_gen(Hlinhas, xpColunas, 0.5);
            break;
        case 3:
            Rd = channel_rd_gen(Hlinhas, xpColunas, 1);
            break;
    }
    if (Rd == NULL) {
        printf("Erro na geração do ruído do canal\n");
        free(xh);
        return NULL;
    }

    complexo ** xt = soma(xh, Rd, Hlinhas, xpColunas);
    if (xt == NULL) {
        printf("Erro na soma de matrizes\n");
        free(xh);
        free(Rd);
        return NULL;
    }

    free(xh);
    free(Rd);

    return xt;
}
/**
 * @brief Realiza a multiplicação dos sinais recebidos pelas Nr antenas pela matriz U 
 *
 * Esta função utiliza a matriz transposta U e multiplica pelo o vetor xt que estamos transmitindo
 * gerando o vetor combinado xc.
 *
 * @param U Matriz U alocada.
 * @param xt Vetor xt transmitido.
 * @param Ulinhas O número de linhas da matriz U.
 * @param Ucolunas O número de colunas da matriz U.
 * @param xtlinhas O número de linhas do vetor xt.
 * @param xtcolunas O número de colunas do vetor xt.
 *
 * @return Retorna o vetor xc
 */

complexo ** rx_combiner(complexo ** U, complexo ** xt, int Ulinhas, int Ucolunas, int xtLinhas, int xtColunas){
    complexo ** xc = produto_matricial_geral(transposta(U, Ulinhas, Ucolunas), xt, Ucolunas, Ulinhas, xtLinhas, xtColunas);
    return xc;
}
/**
 * @brief Retira a interferência do canal H (matriz S da decomposição SVD) 
 *
 * Esta função utiliza cada elemento não nulo da matriz S e
 * e divide por cada elemento do vetor xc de mesma linha.
 *
 * @param S Matriz S alocada.
 * @param xc Vetor xt transmitido.
 * @param Slinhas O número de linhas da matriz U.
 * @param Scolunas O número de colunas da matriz U.
 * @param xctlinhas O número de linhas do vetor xt.
 * @param xcColunas O número de colunas do vetor xt.
 *
 * @return Retorna o vetor xf
 */
complexo ** rx_feq(complexo ** S, complexo ** xc, int Slinhas, int Scolunas, int xcLinhas, int xcColunas){
    complexo ** xf = allocateComplexMatrix(xcLinhas, xcColunas);
    for (int l = 0; l < Slinhas; l++){
        for (int c = 0; c < Scolunas; c++){
            if (l == c){
                xf[l][0].real = xc[l][0].real/S[l][c].real;
                xf[l][0].img = xc[l][0].img/S[l][c].real;
            }
        }
    }
    return xf;
}

/**
 * @brief Gera estatísticas sobre os símbolos QAM transmitidos e recebidos.
 *
 * Esta função calcula estatísticas sobre os símbolos QAM transmitidos e recebidos,
 * comparando o vetor de símbolos transmitidos `s` com o vetor de símbolos recebidos
 * `finals`. A função conta o número de acertos e erros de transmissão e calcula a
 * porcentagem de símbolos recebidos com erro em relação ao total de símbolos.
 *
 * @param s O vetor de símbolos QAM transmitidos.
 * @param finals O vetor de símbolos QAM recebidos.
 * @param numBytes O número de bytes transmitidos (considerando 4 símbolos QAM por byte).
 *
 * @note Esta função exibe as estatísticas na saída padrão.
 */
void gera_estatistica(int *s, int *finals, long int numBytes, int teste, int Nr, int Nt, double r, complexo **original_signal, complexo **received_signal, int Nstream, long int Nsymbol){
    int cont_acertos=0;
    int cont_erros=0;
    printf("\nNúmeros de simbolos QAM Transmitidos: %ld\n",numBytes*4);
    printf("numBytes=%ld\n",numBytes);
    for(int i =0; i<numBytes*4; i++){
        if(s[i]==finals[i]){
            cont_acertos = cont_acertos + 1;
        }
        else{
            cont_erros = cont_erros + 1;
        }
    }
    double porcentagem_erro = (cont_erros*100)/(4*numBytes);
    printf("Número de bits recebidos com erro: %d\n",cont_erros);
    printf("Porcentagem de bits recebidos com erro: %0.4f%%\n\n",porcentagem_erro);

    // Cada símbolo QAM errado representa 2 bits errados
    long int total_bits = 2 * numBytes * 4;
    int erro_bits = 2 * cont_erros;

    double ber = (double)erro_bits / total_bits;
    printf("BER: %f\n", ber);

    // Calculate SNR
    double snr_dB = calculate_SNR(original_signal, received_signal, Nstream, Nsymbol);
    printf("SNR: %f dB\n", snr_dB);

    // Calculate EVM
    double evm_dB = calculate_EVM(original_signal, received_signal, Nstream, Nsymbol);
    printf("EVM: %f dB\n", evm_dB);

    double cap = calculate_capacity(snr_dB);
    printf("Capacity: %f bit/symbol\n", cap);

    // FILE *file;

    // // Open the file in append mode, so as not to overwrite existing data
    // file = fopen("output.csv", "a");

    // if (file == NULL) {
    //     printf("Não foi possível abrir o arquivo\n");
    //     return;
    // }

    // // Write the data to the file, including the SNR and EVM
    // fprintf(file, "%d,%d,%d,%f,%f,%f,%f,%f,%f\n", teste, Nr, Nt, r, porcentagem_erro, ber, snr_dB, evm_dB, cap);
    // fclose(file);
}

complexo** expandMatrix(complexo** matriz, int linhas, int colunas, int linhasExtras, int padding){
    // Calcula o novo número total de linhas
    int Nlinhas = linhas + linhasExtras;
    // Aloca uma nova matriz com as dimensões atualizadas
    complexo** novaMatriz = (complexo**) malloc(Nlinhas * sizeof(complexo*));
    if (novaMatriz == NULL){
        printf("Erro de alocacao\n");
    }
    for (int i = 0; i < Nlinhas; i++) {
        novaMatriz[i] = (complexo*) malloc(colunas * sizeof(complexo));
    }
    // Copia os elementos da matriz original para a nova matriz
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            novaMatriz[i][j] = matriz[i][j];
        }
    }
    // Preenchimento da matriz aumentada com nulos.
    if (padding == 0){
        return novaMatriz;
    }else if (padding == 1){
       for (int l = linhas; l < Nlinhas; l++){
        for (int c = 0; c < colunas; c++){
            novaMatriz[l][c].real = 0;
            novaMatriz[l][c].img = 0;
            }
        }
    }
    return novaMatriz;
}

bool is_running_in_wsl(){
    const char* wsl_interop = getenv("WSL_INTEROP");
    return wsl_interop != NULL;
}

bool is_wsl_there(){
    const char* wsl_distro = getenv("WSL_DISTRO_NAME");
    return wsl_distro != NULL;
}

int main() {
    srand(time(NULL));
    system("clear");
    char exec_path[1024];
    #ifdef __unix__
    // Código específico para sistemas Unix
        
        printf("Executando dentro de Unix\n");
        ssize_t countt = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
        if (countt != -1) {
            exec_path[countt] = '\0';
            printf("Localização do executável: %s\n", exec_path);
        }else{
            printf("Erro ao obter a localização do executável.\n");
        }
        char *exec_absolute_path = realpath(exec_path, NULL);
        char *exec_absolute_dirname_path = dirname(exec_absolute_path);
        char destino[PATH_MAX];
        snprintf(destino, sizeof(destino), "%s/testes", exec_absolute_dirname_path);
        char filename[PATH_MAX];
        snprintf(filename, sizeof(filename), "%s/Tx_msg", destino);
        char fileName[PATH_MAX];
        if (access(destino, F_OK) == 0) {
            printf("A pasta testes existe! Pronto para iniciar!\n");
        }else{
            // Cria a pasta testes
            char comando[PATH_MAX];
            sprintf(comando, "mkdir %s", destino);
            system(comando);
            printf("Legal! Pasta testes criada! Pronto para inciar!\n");
        }
    #elif defined(_WIN32) || defined(_WIN64)
        #include <windows.h>
        #include <unistd.h>
        if (is_wsl_there()){
            if (is_running_in_wsl()){
                printf("Executando dentro do WSL\n");
                ssize_t countt = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
                if (countt != -1) {
                    exec_path[countt] = '\0';
                    printf("Localização do executável: %s\n", exec_path);
                }else{
                    printf("Erro ao obter a localização do executável.\n");
                }
                char *exec_absolute_path = realpath(exec_path, NULL);
                char *exec_absolute_dirname_path = dirname(exec_absolute_path);
                destino[PATH_MAX];
                snprintf(destino, sizeof(destino), "%s/testes", exec_absolute_dirname_path);
                char filename[PATH_MAX];
                snprintf(filename, sizeof(filename), "%s/Tx_msg", destino);
                char fileName[PATH_MAX];
                if (access(destino, F_OK) == 0) {
                    printf("A pasta testes existe! Pronto para iniciar!\n");
                }else{
                    // Cria a pasta testes
                    char comando[MAX_PATH];
                    sprintf(comando, "mkdir %s", destino);
                    system(comando);
                    printf("Pasta testes criada! Pronto para inciar!\n");
                }
            }
        }else{
            printf("Executando dentro de Windows\n");
            char exec_absolute_path[MAX_PATH];
            DOWRD countt = GetModuleFileName(NULL, exec_absolute_path, MAX_PATH);
            if (countt != 0){
                printf("Localização do executável: %s\n", exec_absolute_path);
            }else{
                printf("Erro ao obter a localização do executável.\n");
            }
            char *exec_absolute_dirname_path = dirname(exec_absolute_path);
            char filename[MAX_PATH];
            snprintf(filename, sizeof(filename), "%s/Tx_msg", destino);
            char fileName[MAX_PATH];
            if (access(destino, F_OK) == 0) {
                printf("Legal! A pasta testes existe! Pronto para iniciar!\n");
            }else{
                // Cria a pasta testes
                char comando[MAX_PATH];
                sprintf(comando, "mkdir %s", destino);
                system(comando);
                printf("Pasta testes criada! Pronto para inciar!\n");
            }
        }
    #else 
        #error Plataforma de sistema operacional não suportada
    #endif
    FILE *fp;
    fp = fopen(filename, "w+");
    // Solicitar ao usuário que escreva a mensagem
    printf("Digite a mensagem que quer enviar:\n");
    char mensagem[50000];
    fgets(mensagem, sizeof(mensagem), stdin);
    // Escrever a mensagem no arquivo
    fprintf(fp, "%s", mensagem);
    // Fechar o arquivo
    fclose(fp);

    int Nr, Nt, r;
    int mode;
    int num_teste = 7; // número de testes predefinidos

    printf("Digite 1 para o modo predefinido ou 2 para o modo personalizado: ");
    scanf("%d", &mode);
  

    if (mode == 2) {
        getUserInput(&Nr, &Nt, &r);
        num_teste = 25; // apenas um teste será executado no modo personalizado
    }

    if(num_teste > 61){
        printf("\nNumero de testes inviavel. saindo...");
        system("pause");
        exit(1);
    }
    // printf("Quantos testes deseja realizar? (1-61): ");
    // scanf("%d", &num_teste);
    for(int teste = 1; teste <= num_teste; teste++){
            
        printf("\n===================== Teste %d ===================\n\n", teste);
        fp = fopen(filename, "rb");

        if (fp == NULL) {
            printf("Impossivel abrir o arquivo\n");
            return 1; // Encerra o programa se a abertura do arquivo falhar
        }
        // Calculando o número de bytes do arquivo.
        printf("Arquivo criado com sucesso!\n");
        fseek(fp, 0, SEEK_END);
        long int numBytes = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // Número de antenas recpetoras
        // Número de antenas transmissoras
        if(mode == 1) {     
            r = 3;
            if(teste == 1){
                Nr = 2;
                Nt = 4;
            }else if (teste == 2){
                Nr = 4;
                Nt = 8;
            }else if (teste == 3){
                Nr = 8;
                Nt = 16;
            }else if (teste == 4){
                Nr = 16;
                Nt = 32;
            }else if (teste == 5){
                Nr = 32;
                Nt = 64;
            }else if (teste == 6){
                Nr = 64;
                Nt = 128;
            }else if (teste == 7){
                Nr = 128;
                Nt = 256;
            }
                            
            // if(teste <= 4){
            //     Nr = 2;
            //     Nt = 4;
            // }else if (teste > 4 && teste <= 8 ){
            //     Nr = 4;
            //     Nt = 8;
            // }else if (teste > 8 && teste <= 12){
            //     Nr = 8;
            //     Nt = 16;
            // }else if (teste > 12 && teste <= 16){
            //     Nr = 32;
            //     Nt = 16;
            // }else if (teste > 16 && teste <= 20){
            //     Nr = 32;
            //     Nt = 64;
            // }else if (teste > 20 && teste <= 24){
            //     Nr = 64;
            //     Nt = 128;
            // }else if (teste > 24 && teste <= 28){
            //     Nr = 128;
            //     Nt = 256;
            // }else if (teste > 28 && teste <= 32){
            //     Nr = 256;
            //     Nt = 512;
            // }else if (teste > 32 && teste <= 36){
            //     Nr = 512;
            //     Nt = 1024;
            // }

            // Choosing noise interval: 0 for [-0.01,0.01], 1 for [-0.1,0.1], 2 for [-0.5,0.5], 3 for [-1,1]
            // r = (teste - 1) % 4;
            // r = 3;

        }
        //Declarando o número de fluxos
        int Nstream;
        if (Nr <= Nt){
            Nstream = Nr;
        }else{
            Nstream = Nt;
        }
        printf("\nNumero de antenas recpetoras Nr: %d\nNumero de antenas transmissoras Nt: %d\nNumero de fluxos Nstream: %d", Nr, Nt, Nstream);
        // Leitura do arquivo
        printf("\nRealizando leitura do Arquivo...");
        int * s= tx_data_read(fp, numBytes);
        // Calculando número de símbolos necessário para (numBytes*4 + Npadding) % Nstream == 0.
        int Npadding;
        if ((numBytes*4) % Nstream == 0){
            Npadding = 0;
        }else{ 
            Npadding = (Nstream - (numBytes*4)%Nstream);
        }
        printf("\nQuantidade de simbolos de preenchimento: %d", Npadding);
        //Preenchimento por meio do data_padding
        int *pad = tx_data_padding(s, numBytes, Npadding);
        // Calculando número de símbolos
        long int Nsymbol = (numBytes*4 + Npadding);
        // Mapeamento dos bits do arquivo
        complexo *map = tx_qam_mapper(pad, Nsymbol);
        //Transformando o vetor complexo do mapaeamento para uma matriz complexa Nstream linhas
        printf("\nMapeando a matriz stream Nstream x (Nsymbols/Nstream)...");
        complexo **mtx= tx_layer_mapper(map, Nstream, Nsymbol);
        complexo **rx_mtx= allocateComplexMatrix(Nstream, Nsymbol/Nstream); // matriz receptora
        // Criação do Canal H com range entre -1 e 1
        printf("\nCriando canal de transferencia de dados...");
        complexo ** H = channel_gen(Nr, Nt, 1);
        //Inciando transmissão pelo canal de Nsymbol/Nstream tempos de transmissão
        printf("\nIniciando segmentação de transmissão...");
        for (int Nx = 0; Nx < Nsymbol/Nstream; Nx++){
            complexo ** x = allocateComplexMatrix(Nstream, 1);
            for(int l = 0; l < Nstream; l++){
                x[l][0].real = mtx[l][Nx].real;
                x[l][0].img = mtx[l][Nx].img;
            }
            if (Nr < Nt){
                printf("\nTransmissão do vetor v%d da matriz de dados em stream...", Nx);
                complexo ** T = transposta(H, Nr, Nt);
                complexo ** U = allocateComplexMatrix(Nr, Nr);
                complexo ** S = allocateComplexMatrix(Nr, Nr);
                complexo ** V = allocateComplexMatrix(Nt, Nr);
                transposed_channel_svd(T, V, S, U, Nt, Nr);
                complexo ** xp = tx_precoder (V, x, Nt, Nr, Nstream, 1);
                complexo ** xt = channel_transmission(H, xp, Nr, Nt, Nt, 1, r);
                complexo ** xc = rx_combiner(U, xt, Nr, Nr, Nstream, 1);
                complexo ** xf = rx_feq(S, xc, Nr, Nr, Nstream, 1);
                for(int l = 0; l < Nstream; l++){
                    rx_mtx[l][Nx].real = xf[l][0].real;
                    rx_mtx[l][Nx].img = xf[l][0].img;
                }
                if (Nx+1 == Nsymbol/Nstream){
                    FILE *fp;
                    fp = fopen("lambsa.csv", "a+");
                    if (fp == NULL){
                        printf("Erro ao abrir o arquivo\n");
                        return 1;
                    }
                    fprintf(fp, "%d, %d", Nr,Nt);
                    for(int i=0; i<Nstream; i++){
                        for(int j=0; j<Nstream; j++){
                            if(i==j)
                                fprintf(fp, ", %f",r, S[i][j].real);
                        }
                    }
                    fprintf(fp, "\n");
                }

            }else if (Nr >= Nt){
                complexo ** x = allocateComplexMatrix(Nstream, 1);
                for(int l = 0; l < Nstream; l++){
                    x[l][0].real = mtx[l][Nx].real;
                    x[l][0].img = mtx[l][Nx].img;
                }
                printf("\nTransmissão do vetor %d da matriz de dados em stream...", Nx);
                complexo ** U = allocateComplexMatrix(Nr, Nt);
                complexo ** S = allocateComplexMatrix(Nt, Nt);
                complexo ** V = allocateComplexMatrix(Nt, Nt);
                square_channel_svd(H, U, S, V, Nr, Nt);
                complexo ** xp = tx_precoder (V, x, Nt, Nt, Nstream, 1);
                complexo ** xt = channel_transmission(H, xp, Nr, Nt, Nt, 1, r);
                complexo ** xc = rx_combiner(U, xt, Nr, Nt, Nr, 1);
                complexo ** xf = rx_feq(S, xc, Nt, Nt, Nstream, 1);
                for(int l = 0; l < Nstream; l++){
                    rx_mtx[l][Nx].real = xf[l][0].real;
                    rx_mtx[l][Nx].img = xf[l][0].img;
                }
            }
        }




        printf("\nCompondo o vetor de complexos rx_map..");
        complexo *rx_map = rx_layer_demapper(rx_mtx, Nstream, Nsymbol);
        for(int i = 0; i < Nsymbol; i++){
            rx_map[i].real = round(rx_map[i].real);
            rx_map[i].img = round(rx_map[i].img);
        }
        // Desmapeamento dos bits do arquivo
        printf("\nRealizando desmapeamento dos bits do arquivo em rx_qam_mapper...");
        int *a = rx_qam_demapper(rx_map, Nsymbol);
        printf("\nRemovendo simbolos nulos em rx_depadding...");
        int *s_rest = rx_data_depadding(a, numBytes, Nstream);
        // Leitura Final dos Dados
        printf("\nSalvando arquivo com a mensagem enviada no arquivo Teste_%d_Nr%d_Nt%d_Rd%d\n", teste, Nr, Nt, r);
        
        sprintf(fileName, "%s/Teste_%d_Nr%d_Nt%d_Rd%d", destino, teste, Nr, Nt, r); // Formata o nome do arquivo com base no valor de i
        rx_data_write(s_rest, numBytes, fileName);
        gera_estatistica(s, s_rest, numBytes, teste, Nr, Nt, r, mtx, rx_mtx, Nstream, Nsymbol);        
        printf("================== Fim do teste %d================\n", teste);
    }
    fclose(fp);
    return 0;
}