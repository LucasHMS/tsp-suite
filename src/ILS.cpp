//
// Created by Puca Penna on 21/09/2018.
//

#include <iostream>
#include <time.h>
#include "Util.h"
#include "Descida.h"
#include "Arquivos.h"
#include "ILS.h"
#include "stdlib.h"

float perturbacao(int n, int *s, int *s_linha, float **d, float fo, int nivelPert){
    atualiza_vetor(s_linha, s, n);
    int i, j, aux;
    int n_mod = nivelPert + 1;
    float fo_viz;
    float delta1, delta2;
    for (int i=0; i<n_mod; i++){
        j = rand()%n;
        do{
            i = rand()%n;
        }while (i == j);

        delta1 = calcula_delta(n,s_linha,d,i,j);
        aux = s_linha[i];
        s_linha[i] = s_linha[j];
        s_linha[j] = aux;
        delta2 = calcula_delta(n,s_linha,d,i,j);
        fo_viz = fo - delta1 + delta2;;
    }

    return fo_viz;
}//fim da perturbacao

float ILS(int n, int *s, float **d, float fo, int nivelPert, int ILSMax){
    int iter = 0;
    int melhor_iter = 0;
    int n_vezes = 0;
    
    int *s_linha = cria_vetor(n);

    float fo_viz = fo;

    while(iter - melhor_iter < ILSMax) {
        iter++;
        
        perturbacao(n, s, s_linha, d, fo, nivelPert);
        
        fo_viz = descida_primeiro_melhora(n, s_linha, d);

        if (fo_viz < fo) {
            atualiza_vetor(s, s_linha, n);
            melhor_iter = iter;
            nivelPert = 1;
            fo = fo_viz;
        } else {
            nivelPert++;
        }
    }

    libera_vetor(s_linha);
    return fo;
}//fim do ILS

//Implementar o Smart ILS
float SmartILS(int n, int *s, float **d, float fo, int nivelPert, int PertMax, int ILSMax){
    int iter = 0;
    int melhor_iter = 0;
    int n_vezes = 1;
    
    int *s_linha = cria_vetor(n);

    float fo_viz = fo;

    while(iter - melhor_iter < ILSMax) {
        iter++;
        
        perturbacao(n, s, s_linha, d, fo, nivelPert);
        
        fo_viz = descida(n, s_linha, d);

        if (fo_viz < fo) {
            atualiza_vetor(s, s_linha, n);
            melhor_iter = iter;
            nivelPert = 1;
            n_vezes = 1;
            fo = fo_viz;
        } else {
            if (n_vezes >= PertMax){
                nivelPert++;
                n_vezes = 1;
            } else {
                n_vezes++;
            }
        }
    }

    return fo;
}