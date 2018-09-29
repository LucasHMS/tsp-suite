
/*
Tecnicas Heuristicas para resolucao do Problema do Caixeiro Viajante
Autor: Marcone Jamilson Freitas Souza
Local: Departamento de Computacao - Universidade Federal de Ouro Preto
Homepage: www.decom.ufop.br/prof/marcone
Data: 21-05-2007
*/

//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Util.h"
#include "Arquivos.h"
#include "Estruturas.h"
#include "Construcao.h"
#include "Listas.h"
#include "Menus.h"
#include "Descida.h"
#include "SA.h"
#include "MultiStart.h"
#include "ILS.h"
#include "BT.h"
//---------------------------------------------------------------------------
using namespace std;


int main(int argc, char* argv[])
{
  int n,                    // numero de cidades
      *s;                   // vetor solucao corrente
  float **d,                // matriz de distancias entre as cidades
        fo,                 // funcao objetivo corrente
        melhor_fo_lit;      // melhor fo da literatura
  clock_t inicio_CPU,       // clock no inicio da aplicacao do metodo
          fim_CPU;          // clock no final da aplicacao do metodo
  float temp_inicial;



  obter_parametros_pcv("input/c50info.txt", &n, &melhor_fo_lit);
  s = cria_vetor(n); // vetor solucao
  d = cria_matriz_float(n, n); // matriz de distancias
  le_arq_matriz("input/c50.txt", n, d);

  int escolha = 0;
  do {
    escolha = menu_principal();
    switch (escolha) {
    case 1: /* Geracao de uma solucao inicial */
           switch(menu_solucao_inicial()) {
           case 1: /* Geracao gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 constroi_solucao_gulosa_vizinho_mais_proximo(n,s,d);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma gulosa (Vizinho Mais Proximo):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 2: /* Geracao parcialmente gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n,s,d,0.05);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma gulosa (Vizinho Mais Proximo):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 3: /* Geracao gulosa de uma solucao inicial via metodo da insercao mais barata */
                 constroi_solucao_gulosa_insercao_mais_barata(n,s,d);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma gulosa (Insercao Mais Barata):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 4: /* Geracao parcialmente gulosa de uma solucao inicial via insercao mais barata */
                 printf("Ainda nao implementado...\n",fo);
	         break;
           case 5: /* Geracao aleatória de uma solucao inicial */
                 //srand(1000); // pega o numero 1000 como semente de numeros aleatorios
                 srand((unsigned) time(NULL)); // pega a hora do relogio como semente
                 constroi_solucao(n,s,d);
                 embaralha_vetor(s,n);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma aleatoria:\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           }
           break;
    case 2: /* Descida */
           inicio_CPU = clock();
           fo = descida(n,s,d);
           fim_CPU = clock();
           printf("Solucao obtida usando a estrategia Best Improvement do Metodo da Descida:\n");
           imprime_rota(s,n);
           printf("Funcao objetivo = %f\n",fo);
           printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 3: /* Descida Randômica */
          inicio_CPU = clock();
          fo = descida_randomica(n, s, d, 10*n);
          fim_CPU = clock();
          printf("Solucao obtida usando a estrategia Randomica do Metodo da Descida:\n");
          imprime_rota(s,n);
          printf("Funcao objetivo = %f\n",fo);
          printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
          break;

    case 4: /* Descida com primeiro de melhora */
          inicio_CPU = clock();
          fo = descida_primeiro_melhora(n, s, d);
          fim_CPU = clock();
          printf("Solucao obtida usando a estrategia primeiro de melhora do Metodo da Descida:\n");
          imprime_rota(s,n);
          printf("Funcao objetivo = %f\n",fo);
          printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
          break;

    case 5: /* Multi-Start */
            inicio_CPU = clock();
            fo = multstart(n, s, d, 1000);
            fim_CPU = clock();
            printf("Solucao obtida usando Multi-Start:\n");
            imprime_rota(s,n);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 6: /* Simulated Annealing */
            inicio_CPU = clock();
            temp_inicial = temperaturaInicial(n, s, d, 2, 0.95, 50*n, 100);
            fo = SA(n, s, d, 0.998, 50*n, temp_inicial, 0.01);
            fim_CPU = clock();
            printf("Solucao obtida usando Smulated Anealling:\n");
            imprime_rota(s,n);
            printf("tempretura inicial = %f\n", temp_inicial);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;           

    case 7: /* Busca Tabu */
            inicio_CPU = clock();
            fo = BT(n, s, d, 7, 30);
            fim_CPU = clock();
            printf("Solucao obtida usando Busca Tabu:\n");
            imprime_rota(s,n);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 8: /* Iterated Local Search */
            inicio_CPU = clock();
            fo = SmartILS(n, s, d, fo, 1, 7, 30);
            fim_CPU = clock();
            printf("Solucao obtida usando ILS:\n");
            imprime_rota(s,n);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 9: /* GRASP */
           switch(menu_GRASP()) {
           case 1: /* Geracao Parcialmente gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 printf("Nao implementado\n");
                 break;
           case 2: /* Geracao parcialmente gulosa de uma solucao inicial via metodo da insercao mais barata */
	         break;
           }
           break;
    case 10: /* VND */
           printf("Nao implementado\n");
           break;

    case 11: /* VNS */
           printf("Nao implementado\n");
           break;

    case 12: /* Algoritmos Geneticos */
           switch(menu_AG()) {
           case 1: /* Algoritmos Geneticos usando operador OX */
                 printf("Nao implementado\n");
                 break;
           case 2: /* Algoritmos Geneticos usando operador ERX */
                 printf("Nao implementado\n");
                 break;
           }
           break;

    case 13: /* Algoritmos Memeticos */
           printf("\n Algoritmos Memeticos ainda nao implementado ... \n");
           break;

    case 14: /* Colonia de Formigas */
           printf("\n Colonia de Formigas ainda nao implementado ... \n");
           break;
     
    case 15: /* Descida completa (vizinhanca adjacente) */
        inicio_CPU = clock();
        fo = descida_adjacente(n, s, d);
        fim_CPU = clock();
        printf("Solucao obtida usando a estrategia Best Improvement do Metodo da Descida (vizinhanca adjacente):\n");
        imprime_rota(s,n);
        printf("Funcao objetivo = %f\n",fo);
        printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
        break;

    case 16: /* Descida Randômica (vizinhanca adjacente)*/
        inicio_CPU = clock();
        fo = descida_randomica_adjacente(n, s, d, 10*n);
        fim_CPU = clock();
        printf("Solucao obtida usando a estrategia Randomica do Metodo da Descida (vizinhanca adjacente):\n");
        imprime_rota(s,n);
        printf("Funcao objetivo = %f\n",fo);
        printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
        break;

    case 17: /* Descida com primeiro de melhora (vizinhanca adjacente) */
          inicio_CPU = clock();
          fo = descida_primeiro_melhora_adjacente(n, s, d);
          fim_CPU = clock();
          printf("Solucao obtida usando a estrategia primeiro de melhora do Metodo da Descida:\n");
          imprime_rota(s,n);
          printf("Funcao objetivo = %f\n",fo);
          printf("Tempo de CPU = %f segundos\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
          break;

    default:
          break;
    }
  } while (escolha != 0);


  libera_vetor(s);
  libera_matriz_float(d, n);
  return 0;
}
//---------------------------------------------------------------------------
