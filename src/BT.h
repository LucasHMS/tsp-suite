float BT(int n,
         int *s,
         float **d,
         int tamanho_maximo_lista,
         int BTmax);

float melhor_vizinho_BT(int n,
                        int *s,
                        float **d,
                        float fo,
                        int *melhor_i,
                        int *melhor_j,
                        float fo_star,
                        struct lista **inicio,
                        struct lista **final);

bool esta_lista_tabu(int posicao1, int posicao2, struct lista **inicio);



 