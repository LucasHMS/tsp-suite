#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Descida.h"
#include "Util.h"
#include "SA.h"
#include "Arquivos.h"
#include "Estruturas.h"
#include "Listas.h"
#include "BT.h"


float BT(int n, int *s, float **d, int tamanho_maximo_lista, int BTmax) {
  int melhor_i, melhor_j;
  struct lista *inicio_tabu, *final_tabu, *tabu;
  int iterBT = 0;  // numero corrente de iteracoes da Busca Tabu
  int MelhorIter = 0; // Iteracao em que ocorreu a melhor solucao
  int *s_star;
  float fo, fo_star;
  clock_t inicio_CPU, fim_CPU;

  s_star = cria_vetor(n);
  inicializa_vetor(s_star,n);
  atualiza_vetor(s_star, s, n);
  fo_star = fo = calcula_fo(n,s,d);

  printf("\nIniciando a Busca Tabu com fo = %8.2f\n", fo);
  inicio_tabu = final_tabu = NULL;
  inicio_CPU = fim_CPU = clock();
  limpa_arquivo("BTsaida.txt");
  limpa_arquivo("BTsaidaMelhorfo.txt");
  imprime_fo("BTsaida.txt", (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC, fo, iterBT);
  imprime_fo("BTsaidaMelhorfo.txt", (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC, fo_star, iterBT);
  while (iterBT - MelhorIter < BTmax) {
    iterBT++;
    float fo_viz = melhor_vizinho_BT(n,s,d,fo,&melhor_i,&melhor_j,fo_star,&inicio_tabu,&final_tabu);
    tabu = (struct lista *)malloc(sizeof(struct lista));
    if (!tabu) {
      printf("Faltando memoria ...\n"); getchar();
      exit(1);
    }


    /* vou inserir um registro na lista tabu */
    tabu->campo1  = melhor_i;
    tabu->campo2  = melhor_j;
    insere_final_lista(tabu, &inicio_tabu, &final_tabu);
    /* vou calcular o tamanho da lista tabu e imprimi-la */
    //       tamanho_lista(&inicio_tabu, &final_tabu));
    //mostra_lista(&inicio_tabu, &final_tabu);

    /* vou manter uma lista tabu com tamanho_maximo_lista_tabu elementos,
            isto e', se a lista tiver mais que essa quantidade de elementos,
            vou apagar o primeiro registro da lista */
    if (tamanho_lista(&inicio_tabu, &final_tabu) > tamanho_maximo_lista){
      /* vou apagar o primeiro registro da lista tabu */
      apaga_registro(inicio_tabu, &inicio_tabu, &final_tabu);
      //       tamanho_lista(&inicio_tabu, &final_tabu));
      //mostra_lista(&inicio_tabu, &final_tabu);
    }

      /* Vou fazer o movimento */
    int aux = s[melhor_j];
    s[melhor_j] = s[melhor_i];
    s[melhor_i] = aux;
    fo = fo_viz;
      //getchar();

    fim_CPU = clock();
    imprime_fo("BTsaida.txt", (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC, fo, iterBT);
    
    if (fo < fo_star){
      MelhorIter = iterBT;
      atualiza_vetor(s_star, s, n);
      fo_star = fo;
      printf("fo_star = %8.2f \n",fo_star);
    }
    imprime_fo("BTsaidaMelhorfo.txt", (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC, fo_star, iterBT);
  }
  
  fim_CPU = clock();
  imprime_fo("BTsaida.txt", (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC, fo, iterBT);
  imprime_fo("BTsaidaMelhorfo.txt", (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC, fo_star, iterBT);
  printf("********** Melhor Solucao Obtida *************** \n");
  printf("Saindo da Busca Tabu com fo_star = %8.2f\n",fo_star);
  printf("Numero de iteracoes realizadas = %d \n",iterBT);
  printf("******************************************** \n");
  getchar();
  
  atualiza_vetor(s, s_star, n);
  libera_vetor(s_star);
  fo = fo_star;
  apaga_lista(&inicio_tabu, &final_tabu);
  return fo;
}

float melhor_vizinho_BT(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j,
                        float fo_star, struct lista **inicio, struct lista **final) {
  int aux;
  float fo_melhor_viz = INT_MAX;

  float fo_viz;
  for(int i=0 ; i < n - 1 ; i++){
    for(int j=i+1 ; j < n ; j++) {
      // Calcula a variacao de energia com a realizacao do movimento
      float delta1 = calcula_delta(n,s,d,i,j);

      // Faz o movimento
      aux = s[j];
      s[j] = s[i];
      s[i] = aux;

      float delta2 = calcula_delta(n,s,d,i,j);

      // Calcular a nova distancia
      fo_viz = fo - delta1 + delta2;

      bool estanalista = esta_lista_tabu(i,j,inicio);

      /* Armazenar o melhor movimento (melhor troca) que nao seja tabu
         ou, se tabu, que melhore a função objetivo estrela */
      if(!estanalista || (estanalista && fo_viz < fo_star)){
        if (estanalista && fo_viz < fo_star){
          printf("\nAspirando, isto e, realizando o movimento tabu que troca a cidade %2d que esta na posicao %2d com a cidade %2d que esta na posicao %2d\n",s[i],i,s[j],j);
          printf("fo_viz = %10.4f fo_star = %10.4f \n\n",fo_viz,fo_star);
          //getchar();
        }
        if (fo_viz < fo_melhor_viz){
          *melhor_i = i;
          *melhor_j = j;
          fo_melhor_viz = fo_viz;
        }
      }

      // Desfaz o movimento
      aux = s[j];
      s[j] = s[i];
      s[i] = aux;
    }
  }

  // retornar a distancia do melhor vizinho
 return fo_melhor_viz;

}//melhor_vizinho_BT


/* Verifica se posicao esta na lista tabu */
bool esta_lista_tabu(int posicao1, int posicao2, struct lista **inicio) {
  struct lista *registro = *inicio;

  while(registro) {
    if (registro->campo1 == posicao1 && registro->campo2 == posicao2)
      return true;
    registro = registro->proximo;  /* obtem novo registro */
  }
  return false; /* nao encontrou */
}
