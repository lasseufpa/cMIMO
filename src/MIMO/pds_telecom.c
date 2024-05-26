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

/**
 * @brief Calculate the capacity of a channel given the SNR in dB.
 * 
 * @param snr_dB Signal-to-noise ratio in decibels.
 * @return double Capacity in bits per symbol.
 */
double calculate_capacity(double snr_dB) {
    double snr = pow(10,snr_dB/10);
    return log2(1 + snr);  // Capacity in bits per symbol
}

/**
 * @brief Calculate the Error Vector Magnitude (EVM) of a signal.
 * 
 * @param original_signal The original transmitted signal.
 * @param received_signal The signal received after transmission.
 * @param Nstream The number of streams in the signal.
 * @param Nsymbol The total number of symbols in the signal.
 * @return double The calculated EVM of the signal in dB.
 */
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

/**
 * @brief Calculate the Signal-to-Noise Ratio (SNR) of a signal.
 * 
 * @param original_signal The original transmitted signal.
 * @param received_signal The signal received after transmission.
 * @param Nstream The number of streams in the signal.
 * @param Nsymbol The total number of symbols in the signal.
 * @return double The calculated SNR of the signal in dB.
 */
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

/**
 * @brief Get user input for the values of Nr, Nt, and r.
 * 
 * @param Nr Pointer to an integer where the value for Nr will be stored.
 * @param Nt Pointer to an integer where the value for Nt will be stored.
 * @param r Pointer to an integer where the value for r will be stored.
 */
void getUserInput(int* Nr, int* Nt, int* r) {
    printf("Enter the value for Nr: ");
    scanf("%d", Nr);

    printf("Enter the value for Nt: ");
    scanf("%d", Nt);

    printf("Enter the value for r: ");
    scanf("%d", r);
}
/**
 * @brief Reads data from a file and converts it into an array of integers.
 *
 * This function reads binary data from a file and converts it into an array of integers.
 * Each integer in the array represents 2 bits of the original data. The conversion is done by reading
 * two bits at a time from the binary data and storing their combined value as an integer in the array.
 *
 * @param fp Pointer to the file to be read. The file should be opened in binary read mode before calling this function.
 * @param numBytes The number of bytes to be read from the file. This should be the size of the data that you want to convert.
 * @return A pointer to the integer array that contains the converted data, or NULL
 *         in case of memory allocation error or if the file cannot be read.
 *
 * @note The caller is responsible for freeing the memory allocated for the integer array
 *       when it is no longer needed, using the free() function. The caller is also responsible for closing the file when it's no longer needed.
 */
int * tx_data_read(FILE *fp, long int numBytes){
    // Allocates memory for the integer array
    int *s = (int *)malloc(numBytes * 4 * sizeof(int));
    if (s == NULL) {
        printf("Error in memory allocation\n");
        fclose(fp);
        return (int *)1;
    }
    // Reads the bytes from the file and converts them into 2-bit integers
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
 * @brief Performs padding of the data with zeros.
 *
 * This function performs padding of the data with zeros to ensure
 * that the size of the integer array is an integer multiple of the number of streams (Nstream).
 * Padding is necessary when the number of bytes in the data is not a multiple of the number of streams.
 * In such cases, padding with zeros ensures that the data can be evenly divided among the streams.
 *
 * @param s Pointer to the integer array containing the data. This array should contain the binary representation of the data to be transmitted.
 * @param numBytes The original number of bytes in the data before padding. This is the size of the array pointed to by `s`.
 * @param Nstream The number of streams for which the array size should be a multiple. This represents the number of separate data streams that the data will be divided among.
 * @return A pointer to the integer array with the padding performed, or NULL
 *         in case of memory allocation error. The caller is responsible for freeing this memory when it is no longer needed.
 */
int * tx_data_padding(int* s, long int numBytes, int Npadding){
    // Checks if the number of bytes is a multiple of the number of streams.
    if(Npadding == 0){
        return s;
    }
    else{
        // Memory allocation for the integer array with the necessary additional space.
        int *resized_s = (int*) malloc((numBytes*4 + Npadding)*sizeof(int));
        // Fills the additional space with nulls - represented by the integer 4 -.
        for (long int i = 0; i < numBytes*4; i++)
            resized_s[i] = s[i];
        for(long int i = numBytes*4; i < ((numBytes*4) + Npadding); i++){
            resized_s[i] = 4;
        }
        return resized_s;
    }     
}

/**
 * @brief Maps binary data into a sequence of QAM symbols.
 *
 * This function maps binary data into a sequence of QAM (Quadrature
 * Amplitude Modulation) symbols represented by complex numbers. The function dynamically allocates
 * memory for the complex vector and returns a pointer to this vector.
 *
 * The mapping is as follows:
 * 0 -> (-1, 1)
 * 1 -> (-1, -1)
 * 2 -> (1, 1)
 * 3 -> (1, -1)
 * Any other value -> (0, 0)
 *
 * The `complexo` type is a struct with two members: `real` and `img`, representing the real and imaginary parts of a complex number.
 *
 * If memory allocation fails, the function returns NULL and prints an error message to stdout. It's the caller's responsibility to check the return value and handle the error appropriately.
 *
 * The caller is also responsible for freeing the memory allocated by this function when it's no longer needed.
 *
 * @param s Pointer to the integer array containing the binary data.
 * @param numBytes The number of bytes contained in the integer array.
 * @return A pointer to the complex vector that contains the mapped QAM symbols, or NULL
 *         in case of memory allocation error.
 */
complexo* tx_qam_mapper(int *s, long int numQAM){
    // Allocates memory for the complex vector
    complexo *c1 = (complexo *)malloc(numQAM * sizeof(complexo));   
    if (c1 == NULL) {
        printf("Error in memory allocation\n");
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
 * @brief Maps data from a complex vector to a complex matrix.
 *
 * This function maps data from a complex vector to a complex matrix,
 * where each position in the matrix represents a transmission stream. The function dynamically allocates
 * memory for the complex matrix and returns a pointer to the matrix.
 *
 * @param v Pointer to the complex vector containing the data to be mapped.
 * @param Nstream The number of transmission streams.
 * @param Nsymbol The number of symbols contained in the complex vector.
 * @return A pointer to the complex matrix that contains the mapped data, or NULL
 *         in case of memory allocation error.
 */

complexo ** tx_layer_mapper(complexo *v, int Nstream, long int Nsymbol){
    // Allocates memory for the complex matrix
    complexo **mtx_stream;
    mtx_stream = (complexo**) malloc(Nstream*sizeof(complexo*));

    for(int i = 0; i < Nstream; i++){
        mtx_stream[i] = (complexo *) malloc((Nsymbol/Nstream)*sizeof(complexo));
    }
    // Maps the data from the vector to the complex matrix
    for (int i = 0; i < Nsymbol; i++){
        mtx_stream[i%Nstream][i/Nstream] = v[i];
    }
    return mtx_stream;
}
/**
 * @brief Maps data from a complex matrix to a complex vector.
 *
 * This function maps data from a complex matrix to a complex vector,
 * where each position in the matrix represents a transmission stream. The function dynamically allocates
 * memory for the complex vector and returns a pointer to the vector.
 *
 * @param mtx_stream Pointer to the complex matrix containing the data to be mapped.
 * @param Nstream The number of transmission streams.
 * @param numBytes The number of bytes contained in the complex matrix.
 * @return A pointer to the complex vector that contains the mapped data, or NULL
 *         in case of memory allocation error.
 *
 * @note The caller is responsible for freeing the memory allocated for the complex vector
 *       when it is no longer needed, using the free() function.
 */

complexo* rx_layer_demapper(complexo** mtx_stream, int Nstream, long int numBytes) {
    // Allocates memory for the complex vector
    complexo* v = (complexo*) malloc(numBytes * sizeof(complexo));
    if (v == NULL) {
        printf("Error in memory allocation\n");
        return NULL;
    }

    // Maps the data from the matrix to the complex vector
    for (int i = 0; i < numBytes; i++) {
        v[i] = mtx_stream[i % Nstream][i / Nstream];
    }

    return v;
}
/**
 * @brief Demaps QAM symbols to binary data.
 *
 * This function takes a vector of complex numbers representing QAM symbols and performs the demapping
 * of these symbols to binary data. Each QAM symbol is associated with a binary value, according to
 * the following table:
 * - (-1, 1)  -> 0
 * - (-1, -1) -> 1
 * - (1, 1)   -> 2
 * - (1, -1)  -> 3
 * - Others   -> 4
 *
 * @param vmap Vector of complex numbers representing the QAM symbols.
 * @param numQAM The number of QAM symbols in the vector.
 *
 * @return A vector of integers containing the binary data demapped from the QAM symbols.
 *         The caller is responsible for freeing the allocated memory using the free() function.
 *
 * @note The function returns a pointer to integer 1 if memory allocation fails. This is a non-standard
 *       practice and it's recommended to return NULL in such cases. The caller should check for NULL
 *       to ensure that memory allocation was successful.
 */
int* rx_qam_demapper(complexo *vmap, long int numQAM) {
    // Allocates memory for the integer vector
    int *vetor = (int *)malloc(numQAM * sizeof(int));
    if (vetor == NULL) {
        printf("Error in memory allocation\n");
        return (int *)1;
    }

    // Demaps the QAM symbols to binary data
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
 * @brief Removes the "null" symbols that were filled (padding).
 *
 * This function undoes the processing done by the data_padding function,
 * returning our vector to its original size.
 * @param s Pointer to the integer array containing the data.
 * @param numBytes The original number of bytes before padding.
 * @param Nstream The number of streams for which the array size should be a multiple.
 * @return A pointer to the integer array with the original values.
 *         
 * @note The function returns a new array if the original number of bytes is not a multiple of the number of streams.
 *       The caller is responsible for freeing the allocated memory using the free() function.
 */

int *rx_data_depadding(int *s, long int numBytes, int Nstream) {
    // Checks if the number of bytes is a multiple of the number of streams
    if ((4*numBytes) % Nstream == 0) {
        return s;
    } 
    else {
        // Creates a new array to store the original values
        int *resized_s = (int *)malloc(4*numBytes*sizeof(int));

        // Copies the original values back to the resized array
        for (long int i = 0; i < 4*numBytes; i++) {
            resized_s[i] = s[i];
        }
        return resized_s;
    }
}
/**
 * @brief Recovers the original bytes.
 *
 * This function recovers the original bytes every 4 digits by performing the inverse process of the data_read function.
 * It takes the integer array 's' and decodes it into character bytes to generate the file named 'fileName'.
 *
 * @param s Pointer to the integer array containing the data.
 * @param numBytes The original number of bytes before padding.
 * @param fileName The name of the file to be written.
 *
 * @note The function does not return a value. It writes the decoded bytes directly to the file named 'fileName'.
 *       If the file cannot be opened for writing, an error message is printed to the console.
 */

void rx_data_write(int* s, long int numBytes, char* fileName) {
    FILE* out = fopen(fileName, "wb");
    if (out == NULL) {
        printf("Error opening file %s for writing.\n", fileName);
        return;
    } else {
        printf("File %s successfully created.\n", fileName);
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
 * @brief Performs the multiplication of two complex matrices.
 *
 * This function performs the multiplication of two complex matrices `mtx_a` and `mtx_b`,
 * resulting in a new matrix `matriz`. The validation of the multiplication operation
 * is done by checking if the number of columns of the matrix `mtx_a` is equal to the number
 * of rows of the matrix `mtx_b`. If they are not compatible, the function displays an error message
 * and terminates the program.
 *
 * @param mtx_a The first complex matrix to be multiplied.
 * @param mtx_b The second complex matrix to be multiplied.
 * @param linhas_a The number of rows of the matrix `mtx_a`.
 * @param colunas_a The number of columns of the matrix `mtx_a`.
 * @param linhas_b The number of rows of the matrix `mtx_b`.
 * @param colunas_b The number of columns of the matrix `mtx_b`.
 *
 * @return A new complex matrix resulting from the multiplication of `mtx_a` and `mtx_b`.
 *         The caller is responsible for freeing the allocated memory using the free() function.
 *
 * @note This function assumes that the matrices `mtx_a` and `mtx_b` have been correctly allocated
 *       and have compatible dimensions for multiplication.
 */

complexo** general_matrix_product(complexo** mtx_a, complexo** mtx_b, int linhas_a, int colunas_a, int linhas_b, int colunas_b)
{
    // Validation of the multiplication operation (colunas_a == linhas_b).
    if (colunas_a != linhas_b)
    {
        printf("\nError: The product cannot be performed (incompatibility between matrices)\n");
        exit(1);
    }
    
    complexo** matriz;
    matriz = allocateComplexMatrix(linhas_a, colunas_b);
    
    for (int l = 0; l < linhas_a; l++)
    {
        for (int c = 0; c < colunas_b; c++)
        {
            complexo accumulator;
            accumulator.real = 0;
            accumulator.img = 0;
            
            for (int i = 0; i < colunas_a; i++)
            {
                accumulator = soma_complexo(accumulator, multcomp(mtx_a[l][i], mtx_b[i][c]));
            }
            
            matriz[l][c].real = accumulator.real;
            matriz[l][c].img = accumulator.img;
        }
    }
    
    return matriz;
}

/**
 * @brief Generates a complex matrix representing a transfer channel.
 *
 * This function generates a complex matrix that represents a transfer channel between
 * transmitting antennas and receiving antennas. The elements of the matrix are random complex numbers
 * with the imaginary part set to zero.
 *
 * @param Nr The number of receiving antennas.
 * @param Nt The number of transmitting antennas.
 * @param sigma The standard deviation value for the channel creation.
 *
 * @return A complex matrix representing the generated transfer channel.
 *         The caller is responsible for freeing the allocated memory using the free() function.
 *
 * @note This function assumes that the GSL library is correctly installed and linked to the project.
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
 * @brief Generates a complex matrix representing the noise in the communication channel.
 *
 * This function generates a complex matrix representing the noise in the communication channel.
 * The resulting matrix has dimensions Nr x Nt, where Nr is the number of receiving antennas
 * and Nt is the number of transmitting antennas. The values of the matrix elements are generated
 * randomly with a Gaussian distribution with mean 0 and standard deviation sigma.
 *
 * @param Nr The number of receiving antennas.
 * @param Nt The number of transmitting antennas.
 * @param sigma The standard deviation of the noise.
 *
 * @return A complex matrix representing the noise in the communication channel.
 *         The caller is responsible for freeing the allocated memory using the free() function.
 *
 * @note This function assumes that the GSL library is correctly installed and linked to the project.
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
 * @brief Performs Singular Value Decomposition (SVD) on a transposed matrix.
 *
 * This function performs SVD on a transposed matrix, represented by a matrix of complex numbers.
 * The function only uses the real part of the matrix elements for the SVD calculation. It dynamically
 * allocates memory for the matrices U, V, and the vector S, and stores the decomposition results in
 * the matrices Uh, Sh, and Vh.
 *
 * @param H Transposed complex matrix to be decomposed.
 * @param Uh Resulting U matrix from the decomposition, containing the left eigenvectors.
 * @param Sh Resulting S matrix from the decomposition, containing the singular values on the diagonal.
 * @param Vh Resulting V matrix from the decomposition, containing the right eigenvectors.
 * @param Tlinhas The number of rows of the transposed matrix H.
 * @param Tcolunas The number of columns of the transposed matrix H.
 *
 * @note This function only considers the real part of the matrix H elements for the SVD calculation.
 *       The function prints a warning if complex elements are detected in the matrix H, but ignores
 *       the imaginary part for the calculation.
 *
 * @remark The `transposed_channel_svd` function is similar to the `square_channel_svd` function, but there's a crucial
 *         difference between them. The `square_channel_svd` function takes a square matrix as a parameter, while the
 *         `transposed_channel_svd` function takes the transposed matrix as a parameter. The transposed matrix is obtained
 *         by swapping the rows and columns of the original matrix. Therefore, while the `square_channel_svd` function performs
 *         the SVD of a square matrix, the `transposed_channel_svd` function performs the SVD of the transposed matrix.
 */

void transposed_channel_svd(complexo **H, complexo **Uh, complexo **Sh, complexo **Vh, int Tlinhas, int Tcolunas){
    for (int l = 0; l < Tlinhas; l++){
        for (int c = 0; c < Tcolunas; c++){
            if (H[l][c].img != 0){
                printf("Warning: complex matrix injected as parameter, function will use only real part from matrix\n");
                break;
            }
        }
    }
    gsl_matrix * U = gsl_matrix_alloc(Tlinhas, Tcolunas); // U matrix lxc
    gsl_matrix * V = gsl_matrix_alloc(Tcolunas, Tcolunas); // V matrix cxc
    gsl_vector * S = gsl_vector_alloc(Tcolunas); // S vector cx1
    gsl_vector * work = gsl_vector_alloc(Tcolunas);
    
    for(int l=0; l<Tlinhas; l++){
        for(int c=0; c<Tcolunas; c++){
            gsl_matrix_set(U, l, c, H[l][c].real);
        }
    }

    gsl_linalg_SV_decomp(U, V, S, work);
    for(int l=0; l<Tlinhas; l++){
        for(int c=0; c<Tcolunas; c++){
            Uh[l][c].real = gsl_matrix_get(U, l, c);
            Uh[l][c].img = 0;
        }
    }
    for(int l=0; l<Tcolunas; l++){
        for(int c=0; c<Tcolunas; c++){
            Vh[l][c].real = gsl_matrix_get(V, l, c);
            Vh[l][c].img = 0;
        }
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
 * @brief Performs Singular Value Decomposition (SVD) on a square matrix.
 *
 * This function performs SVD on a square matrix, represented by a matrix of complex numbers.
 * The function only uses the real part of the matrix elements for the SVD calculation.
 * It dynamically allocates memory for the U, V matrices and the S vector, and stores the 
 * decomposition results in the Uh, Sh, and Vh matrices.
 *
 * @param H The square complex matrix to be decomposed.
 * @param Uh The resulting U matrix from the decomposition, containing the left eigenvectors.
 * @param Sh The resulting S matrix from the decomposition, containing the singular values on the diagonal.
 * @param Vh The resulting V matrix from the decomposition, containing the right eigenvectors.
 * @param linhas The number of rows in the H matrix.
 * @param colunas The number of columns in the H matrix.
 *
 * @note This function only considers the real part of the H matrix elements for the SVD calculation.
 *       The function returns immediately if complex elements are detected in the H matrix.
 */
void square_channel_svd(complexo **H,  complexo **Uh, complexo **Sh, complexo **Vh, int linhas, int colunas) {
    // Check for complex elements in the matrix
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            if (H[l][c].img != 0) {
                return;
            }
        }
    }
    
    // Allocate memory for the matrices and vectors
    gsl_matrix *U = gsl_matrix_alloc(linhas, colunas);
    gsl_matrix *V = gsl_matrix_alloc(colunas, colunas);
    gsl_vector *S = gsl_vector_alloc(colunas);
    gsl_vector *work = gsl_vector_alloc(colunas);
    
    // Copy the real part of the elements from H to U
    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            gsl_matrix_set(U, l, c, H[l][c].real);
        }
    }

    // Perform the SVD
    gsl_linalg_SV_decomp(U, V, S, work);
    
    // Copy the results to the output matrices and set the imaginary parts to 0
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
 * @brief Performs the multiplication of the stream symbols by the V matrix resulting from the SVD decomposition
 *
 * This function uses the V matrix and multiplies it by the x vector we are transmitting,
 * generating the precoded vector xp.
 *
 * @param V The V matrix that was allocated in main.
 * @param x The x vector that is being transmitted
 * @param Vlinhas The number of rows in the V matrix.
 * @param Vcolunas The number of columns in the V matrix.
 * @param xlinhas The number of rows in the x vector.
 * @param xcolunas The number of columns in the x vector.
 *
 * @return Returns the xp vector
 */

complexo ** tx_precoder(complexo ** V, complexo **x, int Vlinhas, int Vcolunas, int xlinhas, int xcolunas){
    complexo **xp = general_matrix_product(V, x, Vlinhas, Vcolunas, xlinhas, xcolunas);
    return xp;
}
/**
 * @brief Performs the transmission of the signal through the communication channel.
 *
 * This function performs the transmission of the input signal xp through the communication channel
 * represented by the H matrix. The result of the transmission is calculated by multiplying the H matrix
 * by the xp vector. In addition, a Rd noise is added to the transmitted signal to simulate the
 * characteristics of the communication channel.
 *
 * @param H Matrix representing the communication channel.
 * @param xp Input vector to be transmitted through the channel.
 * @param Hlinhas The number of rows in the H matrix.
 * @param Hcolunas The number of columns in the H matrix.
 * @param xpLinhas The number of rows in the xp vector.
 * @param xpColunas The number of columns in the xp vector.
 * @param r The value that defines the range of the noise to be added: 0 for [-0.001, 0.001],
 *          1 for [-0.01, 0.01], 2 for [-0.5, 0.5], 3 for [-1, 1].
 *
 * @return The resulting matrix of the signal transmission through the channel, plus the noise.
 */

complexo ** channel_transmission(complexo ** H, complexo ** xp, int Hlinhas, int Hcolunas, int xpLinhas, int xpColunas, int r){
    complexo **xh = general_matrix_product(H, xp, Hlinhas, Hcolunas, xpLinhas, xpColunas);
    if (xh == NULL) {
        printf("Error in matrix multiplication\n");
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
        printf("Error in channel noise generation\n");
        free(xh);
        return NULL;
    }

    complexo ** xt = soma(xh, Rd, Hlinhas, xpColunas);
    if (xt == NULL) {
        printf("Error in matrix addition\n");
        free(xh);
        free(Rd);
        return NULL;
    }

    free(xh);
    free(Rd);

    return xt;
}
/**
 * @brief Performs the multiplication of signals received by Nr antennas by the U matrix.
 *
 * This function uses the transposed U matrix and multiplies it by the xt vector that we are transmitting,
 * generating the combined vector xc.
 *
 * @param U Allocated U matrix.
 * @param xt Transmitted xt vector.
 * @param Ulinhas The number of rows in the U matrix.
 * @param Ucolunas The number of columns in the U matrix.
 * @param xtlinhas The number of rows in the xt vector.
 * @param xtcolunas The number of columns in the xt vector.
 *
 * @return Returns the xc vector.
 */

complexo ** rx_combiner(complexo ** U, complexo ** xt, int Ulinhas, int Ucolunas, int xtLinhas, int xtColunas){
    // Transpose the U matrix and multiply it by the xt vector to generate the combined vector xc
    complexo ** xc = general_matrix_product(transposta(U, Ulinhas, Ucolunas), xt, Ucolunas, Ulinhas, xtLinhas, xtColunas);
    return xc;
}
/**
 * @brief Removes the interference from the H channel (S matrix from the SVD decomposition)
 *
 * This function uses each non-zero element of the S matrix and
 * divides it by each element of the xc vector in the same row.
 *
 * @param S Allocated S matrix.
 * @param xc Transmitted xc vector.
 * @param Slinhas The number of rows in the S matrix.
 * @param Scolunas The number of columns in the S matrix.
 * @param xcLinhas The number of rows in the xc vector.
 * @param xcColunas The number of columns in the xc vector.
 *
 * @return Returns the xf vector.
 */
complexo ** rx_feq(complexo ** S, complexo ** xc, int Slinhas, int Scolunas, int xcLinhas, int xcColunas){
    // Allocate memory for the xf vector
    complexo ** xf = allocateComplexMatrix(xcLinhas, xcColunas);

    // For each row in the S matrix
    for (int l = 0; l < Slinhas; l++){
        // For each column in the S matrix
        for (int c = 0; c < Scolunas; c++){
            // If the row index is equal to the column index
            if (l == c){
                // Divide the real part of the xc vector by the real part of the S matrix
                xf[l][0].real = xc[l][0].real/S[l][c].real;
                // Divide the imaginary part of the xc vector by the real part of the S matrix
                xf[l][0].img = xc[l][0].img/S[l][c].real;
            }
        }
    }

    // Return the xf vector
    return xf;
}

/**
 * @brief Generates statistics about the transmitted and received QAM symbols.
 *
 * This function calculates statistics about the transmitted and received QAM symbols,
 * comparing the transmitted symbols vector `s` with the received symbols vector
 * `finals`. The function counts the number of correct and incorrect transmissions and calculates the
 * percentage of symbols received with errors in relation to the total symbols.
 *
 * @param s The vector of transmitted QAM symbols.
 * @param finals The vector of received QAM symbols.
 * @param numBytes The number of transmitted bytes (considering 4 QAM symbols per byte).
 *
 * @note This function displays the statistics on the standard output.
 */
void generate_statistics(int *s, int *finals, long int numBytes, int teste, int Nr, int Nt, double r, complexo **original_signal, complexo **received_signal, int Nstream, long int Nsymbol){
    int correct_count=0;
    int error_count=0;
    printf("\nNumber of QAM symbols transmitted: %ld\n",numBytes*4);
    printf("numBytes=%ld\n",numBytes);
    for(int i =0; i<numBytes*4; i++){
        if(s[i]==finals[i]){
            correct_count = correct_count + 1;
        }
        else{
            error_count = error_count + 1;
        }
    }
    double error_percentage = (error_count*100)/(4*numBytes);
    printf("Number of bits received with error: %d\n",error_count);
    printf("Percentage of bits received with error: %0.4f%%\n\n",error_percentage);

    // Each incorrect QAM symbol represents 2 incorrect bits
    long int total_bits = 2 * numBytes * 4;
    int error_bits = 2 * error_count;

    double ber = (double)error_bits / total_bits;
    printf("BER: %f\n", ber);

    // Calculate SNR
    double snr_dB = calculate_SNR(original_signal, received_signal, Nstream, Nsymbol);
    printf("SNR: %f dB\n", snr_dB);

    // Calculate EVM
    double evm_dB = calculate_EVM(original_signal, received_signal, Nstream, Nsymbol);
    printf("EVM: %f dB\n", evm_dB);

    double cap = calculate_capacity(snr_dB);
    printf("Capacity: %f bit/symbol\n", cap);

    FILE *file;

    // Open the file in append mode, so as not to overwrite existing data
    file = fopen("output.csv", "a");

    if (file == NULL) {
        printf("Could not open the file\n");
        return;
    }

    // Write the data to the file, including the SNR and EVM
    fprintf(file, "%d,%d,%d,%f,%f,%f,%f,%f,%f\n", teste, Nr, Nt, r, error_percentage, ber, snr_dB, evm_dB, cap);
    fclose(file);
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
        
        printf("Running within Unix\n");
        ssize_t countt = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
        if (countt != -1) {
            exec_path[countt] = '\0';
            printf("Executable location: %s\n", exec_path);
        }else{
            printf("Error obtaining the executable location.\n");
        }
        char *exec_absolute_path = realpath(exec_path, NULL);
        char *exec_absolute_dirname_path = dirname(exec_absolute_path);
        char destino[PATH_MAX];
        snprintf(destino, sizeof(destino), "%s/testes", exec_absolute_dirname_path);
        char filename[PATH_MAX];
        snprintf(filename, sizeof(filename), "%s/Tx_msg", destino);
        char fileName[PATH_MAX];
        if (access(destino, F_OK) == 0) {
            printf("The test folder exists! Ready to start!\n");
        }else{
            // Cria a pasta testes
            char comando[PATH_MAX];
            sprintf(comando, "mkdir %s", destino);
            system(comando);
            printf("Cool! Test folder created! Ready to start!\n");
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
    // Ask the user to write the message
    printf("Enter the message you want to send:\n");
    char mensagem[50000];
    fgets(mensagem, sizeof(mensagem), stdin);
    // Write the message to the file
    fprintf(fp, "%s", mensagem);
    // Close the file
    fclose(fp);
    int Nr, Nt, r;
    int mode;
    int num_teste = 30; // number of predefined tests
    printf("Enter 1 for default mode or 2 for custom mode: ");
    scanf("%d", &mode);
    
    if (mode == 2) {
        getUserInput(&Nr, &Nt, &r);
        num_teste = 25; // only one test will be run in custom mode
    }
    if(num_teste > 61){
        printf("\nNumber of tests not feasible. exiting...");
        system("pause");
        exit(1);
    }
    printf("How many tests do you want to perform? (1-61): ");
    scanf("%d", &num_teste);
    for(int teste = 1; teste <= num_teste; teste++){
            
        printf("\n===================== Test %d ===================\n\n", teste);
        fp = fopen(filename, "rb");
        if (fp == NULL) {
            printf("Unable to open the file\n");
            return 1; // Ends the program if the file opening fails
        }
        // Calculating the number of bytes in the file.
        printf("File created successfully!\n");
        fseek(fp, 0, SEEK_END);
        long int numBytes = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // Número de antenas recpetoras
        // Número de antenas transmissoras
        if(mode == 1) {
            //if(teste % 2 == 0){
            //    Nr = teste;
            //    Nt = teste;
            //}else{
            //    Nr = teste * 2;
            //    Nt = teste * 2;
            //}          

            if(teste <= 4){
                Nr = 2;
                Nt = 4;
            }else if (teste > 4 && teste <= 8 ){
                Nr = 4;
                Nt = 8;
            }else if (teste > 8 && teste <= 12){
                Nr = 8;
                Nt = 16;
            }else if (teste > 12 && teste <= 16){
                Nr = 32;
                Nt = 16;
            }else if (teste > 16 && teste <= 20){
                Nr = 32;
                Nt = 64;
            }else if (teste > 20 && teste <= 24){
                Nr = 64;
                Nt = 128;
            }else if (teste > 24 && teste <= 28){
                Nr = 128;
                Nt = 256;
            }else if (teste > 28 && teste <= 32){
                Nr = 256;
                Nt = 512;
            }else if (teste > 32 && teste <= 36){
                Nr = 512;
                Nt = 1024;
            }

            // Choosing noise interval: 0 for [-0.01,0.01], 1 for [-0.1,0.1], 2 for [-0.5,0.5], 3 for [-1,1]
            r = (teste - 1) % 4;
            // r = 3;
        }
        //Declarando o número de fluxos
        int Nstream;
        if (Nr <= Nt){
            Nstream = Nr;
        }else{
            Nstream = Nt;
        }
        printf("\nNumber of receiving antennas Nr: %d\nNumber of transmitting antennas Nt: %d\nNumber of streams Nstream: %d", Nr, Nt, Nstream);
        // Reading the file
        printf("\nReading the file...");
        int * s= tx_data_read(fp, numBytes);
        // Calculating number of symbols necessary for (numBytes*4 + Npadding) % Nstream == 0.
        int Npadding;
        if ((numBytes*4) % Nstream == 0){
            Npadding = 0;
        }else{ 
            Npadding = (Nstream - (numBytes*4)%Nstream);
        }
        printf("\nAmount of padding symbols: %d", Npadding);
        // Padding through data_padding
        int *pad = tx_data_padding(s, numBytes, Npadding);
        // Calculating number of symbols
        long int Nsymbol = (numBytes*4 + Npadding);
        // Mapping the file bits
        complexo *map = tx_qam_mapper(pad, Nsymbol);
        // Transforming the complex vector from the mapping to a complex matrix with Nstream rows
        printf("\nMapping the stream matrix Nstream x (Nsymbols/Nstream)...");
        complexo **mtx= tx_layer_mapper(map, Nstream, Nsymbol);
        complexo **rx_mtx= allocateComplexMatrix(Nstream, Nsymbol/Nstream); // receiving matrix
        // Creating the H Channel with range between -1 and 1
        printf("\nCreating data transfer channel...");
        complexo ** H = channel_gen(Nr, Nt, 1);
        // Starting transmission through the channel of Nsymbol/Nstream transmission times
        printf("\nStarting transmission segmentation...");
        for (int Nx = 0; Nx < Nsymbol/Nstream; Nx++){
            complexo ** x = allocateComplexMatrix(Nstream, 1);
            for(int l = 0; l < Nstream; l++){
                x[l][0].real = mtx[l][Nx].real;
                x[l][0].img = mtx[l][Nx].img;
            }
            if (Nr < Nt){
                printf("\nTransmission of vector v%d from the data matrix in stream...", Nx);
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
            }else if (Nr >= Nt){
                complexo ** x = allocateComplexMatrix(Nstream, 1);
                for(int l = 0; l < Nstream; l++){
                    x[l][0].real = mtx[l][Nx].real;
                    x[l][0].img = mtx[l][Nx].img;
                }
                printf("\nTransmission of vector v%d from the data matrix in stream...", Nx);
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
        printf("\nComposing the complex vector rx_map..");
        complexo *rx_map = rx_layer_demapper(rx_mtx, Nstream, Nsymbol);
        for(int i = 0; i < Nsymbol; i++){
            rx_map[i].real = round(rx_map[i].real);
            rx_map[i].img = round(rx_map[i].img);
        }
        // Desmapeamento dos bits do arquivo
        printf("\nPerforming file bit demapping in rx_qam_mapper...");
        int *a = rx_qam_demapper(rx_map, Nsymbol);
        printf("\nRemoving null symbols in rx_depadding...");
        int *s_rest = rx_data_depadding(a, numBytes, Nstream);
        // Final Data Reading
        printf("\nSaving file with the sent message in the file Test_%d_Nr%d_Nt%d_Rd%d\n", teste, Nr, Nt, r);

        sprintf(fileName, "%s/Test_%d_Nr%d_Nt%d_Rd%d", destino, teste, Nr, Nt, r); // Formats the file name based on the value of i
        rx_data_write(s_rest, numBytes, fileName);
        generate_statistics(s, s_rest, numBytes, teste, Nr, Nt, r, mtx, rx_mtx, Nstream, Nsymbol);        
        printf("================== End of test %d================\n", teste);
        }
    fclose(fp);
    return 0;
    }