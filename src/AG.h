float AG(int n,
        int *s,
        float **d,
        int nind,
        float max_desvio,
        double prob_crossover,
        double prob_mutacao,
        int tipo_operador);

/* Esta rotina devolve o individuo escolhido pelo mecanismo da roleta russa */
int roleta(int nind, float *fo_pop);


/* Esta rotina devolve o individuo escolhido pelo mecanismo da roleta russa */
int roleta_scaling(int nind, float *fo_pop);

/* Faz a mutacao no individuo, no caso, troca duas cidades de posicao */
void mutacao(int *s, int n);


/* Operador Crossover OX */
void crossover_OX(int *pai1,
                  int *pai2,
                  int *filho1,
                  int *filho2,
                  int n);

/* Operador Crossover ERX */
void crossover_ERX(int *pai1,
                   int *pai2,
                   int *filho1,
                   int *filho2,
                   int n);





