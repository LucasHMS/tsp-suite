float calcula_delta(int n, int *s, float **d, int i, int j);
float descida(int n, int *s, float **d);
float melhor_vizinho(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j);
float descida_randomica(int n, int *s, float **d, int IterMax);
float descida_primeiro_melhora(int n, int *s, float **d);
float vizinho_primeiro_melhora(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j);
float vizinho_aleatorio(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j);

float descida_adjacente(int n, int *s, float **d);
float descida_randomica_adjacente(int n, int *s, float **d, int IterMax);
float descida_primeiro_melhora_adjacente(int n, int *s, float **d);