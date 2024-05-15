#ifndef PDS_TELECOM
#define PDS_TELECOM

int * tx_data_read(FILE *fp, long int numBytes);
int * tx_data_padding(int* s, long int numBytes, int Npadding);
complexo* tx_qam_mapper(int *s, long int numQAM);
complexo ** tx_layer_mapper(complexo *v, int Nstream, long int Nsymbol);
complexo* rx_layer_demapper(complexo** mtx_stream, int Nstream, long int numBytes);
int* rx_qam_demapper(complexo * vmap, long int numQAM);
int *rx_data_depadding(int *s, long int numBytes, int Nstream);
void rx_data_write(int* s, long int numBytes, char* fileName);
complexo** produto_matricial_geral(complexo** mtx_a, complexo** mtx_b, int linhas_a, int colunas_a, int linhas_b, int colunas_b);
complexo ** channel_gen(int Nr, int Nt, double sigma;
complexo ** channel_rd_gen(int Nr, int Nt, double sigma);
void transposed_channel_svd(complexo **H, complexo **Uh, complexo **Sh, complexo **Vh, int Tlinhas, int Tcolunas);
void square_channel_svd(complexo **H, complexo**Uh, complexo**Sh, complexo**Vh, int linhas, int colunas);
complexo ** tx_precoder(complexo ** V, complexo **x, int Vlinhas, int Vcolunas, int xlinhas, int xcolunas);
complexo ** channel_transmission(complexo ** H, complexo ** xp, int Hlinhas, int Hcolunas, int xpLinhas, int xpColunas, int r);
complexo ** rx_combiner(complexo ** U, complexo ** xt, int Ulinhas, int Ucolunas, int xtLinhas, int xtColunas);
complexo ** rx_feq(complexo ** S, complexo ** xc, int Slinhas, int Scolunas, int xcLinhas, int xcColunas);
void gera_estatistica(int *s, int *finals, long int numBytes);
complexo** expandMatrix(complexo** matriz, int linhas, int colunas, int linhasExtras, int padding);

#endif