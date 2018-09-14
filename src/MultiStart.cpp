#include "MultiStart.h"
#include "float.h"
#include "Util.h"
#include "Descida.h"
#include "Construcao.h"

float multstart(int n, int *s, float **d, int iterMax){
    float fo_star, fo;
    int *melhor_i, *melhor_j;
    int iter = 0;
    
    int *s_star;
    s_star = cria_vetor(n);
    atualiza_vetor(s_star, s, n);

    fo_star = FLT_MAX;

    while(iter < iterMax){
        constroi_solucao_aleatoria(n, s, d);
        descida_primeiro_melhora(n, s, d);
        fo = calcula_fo(n, s, d);
        if(fo < fo_star){
            atualiza_vetor(s_star, s, n);
            fo_star = fo;
            iter = 0;
        }else{
           iter++;
        }
    }
    atualiza_vetor(s, s_star, n);
    return fo_star;
}
