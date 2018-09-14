#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include "Descida.h"
#include <time.h>
#include "Arquivos.h"
#include "Construcao.h"

float calcula_delta(int n, int *s, float **d, int i, int j)
{
  int i_antes, i_depois, j_antes, j_depois;

  i_antes = i - 1;
  i_depois = i + 1;
  j_antes = j - 1;
  j_depois = j + 1;
  if (i == 0) i_antes = n-1;
  if (i == n-1) i_depois = 0;
  if (j == 0) j_antes = n-1;
  if (j == n-1) j_depois = 0;

  return  d[s[i_antes]][s[i]] + d[s[i]][s[i_depois]] + d[s[j_antes]][s[j]] + d[s[j]][s[j_depois]];
//  delta = d[s[i-1]][s[i]] + d[s[i]][s[i+1]] + d[s[j-1]][s[j]] + d[s[j]][s[j+1]]
}

float melhor_vizinho(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
  int aux;
  float fo_melhor_viz = fo;

  float fo_viz;
  for(int i=0 ; i < n - 1 ; i++){
    for(int j=i+1 ; j < n ; j++) {
      // Calcula a variacao de custo com a realizacao do movimento
      float delta1 = calcula_delta(n,s,d,i,j);

      // Faz o movimento
      aux = s[j];
      s[j] = s[i];
      s[i] = aux;

      float delta2 = calcula_delta(n,s,d,i,j);

      // Calcular a nova distancia
      fo_viz = fo - delta1 + delta2;

      // Armazenar o melhor movimento (melhor troca)
      if(fo_viz < fo_melhor_viz){
        *melhor_i = i;
        *melhor_j = j;
        fo_melhor_viz = fo_viz;
      }

      // Desfaz o movimento
      aux = s[j];
      s[j] = s[i];
      s[i] = aux;
    }
  }
  // retornar a distancia do melhor vizinho
  return fo_melhor_viz;
}//melhor_vizinho

float melhor_vizinho_adjacente(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j){
  int aux;
  float fo_melhor_viz = fo;

  float fo_viz;
  for(int i=0; i < n; i++){
    int j = i+1;
    if(i == n-1){
      j = 0;
    } 

    // Calcula a variacao de custo com a realizacao do movimento
    float delta1 = calcula_delta(n, s, d, i, j);

    // Faz o movimento
    aux = s[j];
    s[j] = s[i];
    s[i] = aux; 

    float delta2 = calcula_delta(n, s, d, i, j);

    // calcula a nova distancia
    fo_viz = fo - delta1 + delta2;

   // Armazenar o melhor movimento (melhor troca)
    if(fo_viz < fo_melhor_viz){
      *melhor_i = i;
      *melhor_j = j;
      fo_melhor_viz = fo_viz;
    }

    // Desfaz o movimento
    aux = s[j];
    s[j] = s[i];
    s[i] = aux;
  }

  // retornar a distancia do melhor vizinho
  return fo_melhor_viz;
}

float descida(int n, int *s, float **d)
{
  int aux, melhor_i, melhor_j, iter;
  float fo_viz, fo;
  bool melhorou;
  clock_t inicio_CPU, fim_CPU;

  fo = fo_viz = calcula_fo(n, s, d);
  limpa_arquivo("Descida.txt");
  inicio_CPU = fim_CPU = clock();
  iter = 0;
  imprime_fo("Descida.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  do{
     melhorou = false;
     fo_viz = melhor_vizinho(n, s, d, fo, &melhor_i, &melhor_j);
     if (fo_viz < fo){
          iter = iter + 1;
          //printf("Rota antes:  Fo antes = %f \n", fo);
          //imprime_rota(s,n);

          aux = s[melhor_j];
          s[melhor_j] = s[melhor_i];
          s[melhor_i] = aux;

          fo = fo_viz;
          melhorou = true;
          fim_CPU = clock();
          imprime_fo("Descida.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

          //printf("Vou trocar %d com %d \n",melhor_i,melhor_j);
          //printf("Rota depois do movimento: Fo melhor vizinho = %f \n", fo_viz);
          //imprime_rota(s,n);
          //getchar();
     }
  } while (melhorou == true);
  fim_CPU = clock();
  imprime_fo("Descida.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  return fo;
}

float descida_adjacente(int n, int *s, float **d){
  int aux, melhor_i, melhor_j, iter;
  float fo_viz, fo;
  bool melhorou;
  clock_t inicio_CPU, fim_CPU;

  fo = fo_viz = calcula_fo(n, s, d);
  // limpa_arquivo("Descida.txt");
  inicio_CPU = fim_CPU = clock();
  iter = 0;
  // imprime_fo("Descida.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  do{
     melhorou = false;
     fo_viz = melhor_vizinho_adjacente(n, s, d, fo, &melhor_i, &melhor_j);
     if (fo_viz < fo){
          iter = iter + 1;
          //printf("Rota antes:  Fo antes = %f \n", fo);
          //imprime_rota(s,n);

          aux = s[melhor_j];
          s[melhor_j] = s[melhor_i];
          s[melhor_i] = aux;

          fo = fo_viz;
          melhorou = true;
          fim_CPU = clock();
          // imprime_fo("Descida.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

          //printf("Vou trocar %d com %d \n",melhor_i,melhor_j);
          //printf("Rota depois do movimento: Fo melhor vizinho = %f \n", fo_viz);
          //imprime_rota(s,n);
          //getchar();
     }
  } while (melhorou == true);
  fim_CPU = clock();
  // imprime_fo("Descida.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  
  return fo;
}

float vizinho_aleatorio(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j){
  int i, j, iter, aux;
  float delta1, delta2, fo_viz;
  j = rand()%n;
  do{
    i = rand()%n;
  }while (i == j);

  delta1 = calcula_delta(n,s,d,i,j);

  aux = s[i];
  s[i] = s[j];
  s[j] = aux;

  delta2 = calcula_delta(n,s,d,i,j);

  aux = s[j];
  s[j] = s[i];
  s[i] = aux;
  
  *melhor_i = i;
  *melhor_j = j;

  return fo - delta1 + delta2;
}

float descida_randomica(int n, int *s, float **d, int IterMax)
{
  int i, j, iter, aux;
  float delta1, delta2, fo, fo_viz;

  fo = calcula_fo(n,s,d);
  iter = 0;
  while (iter < IterMax){
    iter ++;
    j = rand()%n;
    do{
      i = rand()%n;
    }while (i == j);
    delta1 = calcula_delta(n,s,d,i,j);

    aux = s[i];
    s[i] = s[j];
    s[j] = aux;

    delta2 = calcula_delta(n,s,d,i,j);
    fo_viz = fo - delta1 + delta2;

    if (fo_viz < fo){
      iter = 0;
      fo = fo_viz;
    }
    else{
      aux = s[i];
      s[i] = s[j];
      s[j] = aux;
    }
  }
  return fo;
}

float descida_randomica_adjacente(int n, int *s, float **d, int IterMax)
{
  int i, j, iter, aux;
  float delta1, delta2, fo, fo_viz;

  fo = calcula_fo(n,s,d);
  iter = 0;
  while (iter < IterMax){
    iter ++;
    i = rand()%n;
    j = i+1;
    if(i == n-1)
      j = 0;

    delta1 = calcula_delta(n,s,d,i,j);

    aux = s[i];
    s[i] = s[j];
    s[j] = aux;

    delta2 = calcula_delta(n,s,d,i,j);
    fo_viz = fo - delta1 + delta2;

    if (fo_viz < fo){
      iter = 0;
      fo = fo_viz;
    }
    else{
      aux = s[i];
      s[i] = s[j];
      s[j] = aux;
    }
  }
  return fo;
}

float vizinho_primeiro_melhora(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
  int aux;
  float fo_melhor_viz = fo;
  bool melhorou = false;
  int *vet;

  vet = cria_vetor(n);
  for (int i=0; i < n; i++) vet[i] = i;
  embaralha_vetor(vet,n);

  float fo_viz;
    for(int i=0 ; i < n - 1 ; i++){
      for(int j=i+1 ; j < n ; j++) {
        // Calcula a variacao de custo com a realizacao do movimento
        float delta1 = calcula_delta(n,s,d,vet[i],vet[j]);

        // Faz o movimento
        aux = s[vet[j]];
        s[vet[j]] = s[vet[i]];
        s[vet[i]] = aux;

        float delta2 = calcula_delta(n,s,d,vet[i],vet[j]);

        // Calcular a nova distancia
        fo_viz = fo - delta1 + delta2;

        // Armazenar o melhor movimento (melhor troca)
        if(fo_viz < fo_melhor_viz){
          *melhor_i = vet[i];
          *melhor_j = vet[j];
          fo_melhor_viz = fo_viz;
          melhorou = true;
        }


        // Desfaz o movimento
        aux = s[vet[j]];
        s[vet[j]] = s[vet[i]];
        s[vet[i]] = aux;

        if (melhorou) break;
      }
      if (melhorou) break;
  }
  libera_vetor(vet);
  // retornar a distancia do melhor vizinho
  return fo_melhor_viz;
}//first inprovment

float vizinho_primeiro_melhora_adjacente(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
  int aux;
  float fo_melhor_viz = fo;
  bool melhorou = false;
  int *vet;

  vet = cria_vetor(n);
  for (int i=0; i < n; i++) vet[i] = i;
  embaralha_vetor(vet,n);

  float fo_viz;
    for(int i=0 ; i < n ; i++){
      int j = i+1;
      if(i == n-1){
        j = 0;
      }

      // Calcula a variacao de custo com a realizacao do movimento
      float delta1 = calcula_delta(n,s,d,vet[i],vet[j]);

      // Faz o movimento
      aux = s[vet[j]];
      s[vet[j]] = s[vet[i]];
      s[vet[i]] = aux;

      float delta2 = calcula_delta(n,s,d,vet[i],vet[j]);

      // Calcular a nova distancia
      fo_viz = fo - delta1 + delta2;

      // Armazenar o melhor movimento (melhor troca)
      if(fo_viz < fo_melhor_viz){
        *melhor_i = vet[i];
        *melhor_j = vet[j];
        fo_melhor_viz = fo_viz;
        melhorou = true;
      }

      // Desfaz o movimento
      aux = s[vet[j]];
      s[vet[j]] = s[vet[i]];
      s[vet[i]] = aux;

      if (melhorou) break;
  }
  libera_vetor(vet);
  // retornar a distancia do melhor vizinho
  return fo_melhor_viz;
}//first inprovment (adjacente)

float descida_primeiro_melhora(int n, int *s, float **d)
{
  int aux, melhor_i, melhor_j, iter;
  float fo_viz, fo;
  bool melhorou;
  clock_t inicio_CPU, fim_CPU;

  fo = fo_viz = calcula_fo(n, s, d);
  limpa_arquivo("DescidaPrimeiroMelhora.txt");
  inicio_CPU = fim_CPU = clock();
  iter = 0;
  imprime_fo("DescidaPrimeiroMelhora.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

  /* Implementar a descida First Improvement armazenando, a cada iteração:
      o tempo de execução
      o valor da função de avaliação da solução corrente
      a iteração na qual a solução vizinha foi aceita
      Para fazer essa impressão, use o procedimento imprime_fo, disponível em Arquivos.cpp */
   do{
     melhorou = false;
     fo_viz = vizinho_primeiro_melhora(n, s, d, fo, &melhor_i, &melhor_j);
     if (fo_viz < fo){
          iter = iter + 1;
          //printf("Rota antes:  Fo antes = %f \n", fo);
          //imprime_rota(s,n);

          aux = s[melhor_j];
          s[melhor_j] = s[melhor_i];
          s[melhor_i] = aux;

          fo = fo_viz;
          melhorou = true;
          fim_CPU = clock();
          imprime_fo("DescidaPrimeiroMelhora.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

          //printf("Vou trocar %d com %d \n",melhor_i,melhor_j);
          //printf("Rota depois do movimento: Fo melhor vizinho = %f \n", fo_viz);
          //imprime_rota(s,n);
          //getchar();
     }
  } while (melhorou == true);

  fim_CPU = clock();
  imprime_fo("DescidaPrimeiroMelhora.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  return fo;
}

float descida_primeiro_melhora_adjacente(int n, int *s, float **d)
{
  int aux, melhor_i, melhor_j, iter;
  float fo_viz, fo;
  bool melhorou;
  clock_t inicio_CPU, fim_CPU;

  fo = fo_viz = calcula_fo(n, s, d);
  limpa_arquivo("DescidaPrimeiroMelhora(adjacente).txt");
  inicio_CPU = fim_CPU = clock();
  iter = 0;
  imprime_fo("DescidaPrimeiroMelhora(adjacente).txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

  /* Implementar a descida First Improvement armazenando, a cada iteração:
      o tempo de execução
      o valor da função de avaliação da solução corrente
      a iteração na qual a solução vizinha foi aceita
      Para fazer essa impressão, use o procedimento imprime_fo, disponível em Arquivos.cpp */
   do{
     melhorou = false;
     fo_viz = vizinho_primeiro_melhora_adjacente(n, s, d, fo, &melhor_i, &melhor_j);
     if (fo_viz < fo){
          iter = iter + 1;
          //printf("Rota antes:  Fo antes = %f \n", fo);
          //imprime_rota(s,n);

          aux = s[melhor_j];
          s[melhor_j] = s[melhor_i];
          s[melhor_i] = aux;

          fo = fo_viz;
          melhorou = true;
          fim_CPU = clock();
          imprime_fo("DescidaPrimeiroMelhora(adjacente).txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

          //printf("Vou trocar %d com %d \n",melhor_i,melhor_j);
          //printf("Rota depois do movimento: Fo melhor vizinho = %f \n", fo_viz);
          //imprime_rota(s,n);
          //getchar();
     }
  } while (melhorou == true);

  fim_CPU = clock();
  imprime_fo("DescidaPrimeiroMelhora(adjacente).txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  return fo;
}