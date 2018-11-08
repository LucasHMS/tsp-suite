#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
// #include "listas.h"
// #include "Estruturas.h"
#include "Util.h"
#include "AG.h"
#include "Construcao.h"
// #include "GRASP.h"
#include "Descida.h"

float AG(int n,
        int *s,
        float **d,
        int nind,
        float max_desvio,
        double prob_crossover,
        double prob_mutacao,
        int tipo_operador)
{
    int **pop; // populacao
    float *fo_pop; // funcao objetivo corrente de um dado individuo da populacao
    int **pop_sobrev; // populacao sobrevivente
    float *fo_pop_sobrev; // funcao objetivo corrente de um dado individuo da populacao sobrevivente
    int *s_star;   // vetor que contem a melhor solucao gerada
    float fo, fo_star;   // valor da melhor solucao gerada

    s_star = cria_vetor(n);
    fo_star = INT_MAX;
    if (nind % 2 != 0){
      printf("Numero de individuos deve ser par!\n");
      printf("Vou aumentar o numero de individuos ...\n");
      getchar();
      nind++;
    }
    nind = nind * 2;
    pop = cria_matriz(nind, n);
    fo_pop = cria_vetor_float(nind);
    inicializa_vetor_float(fo_pop,nind);
    for (int j = 0; j < nind; j++) inicializa_vetor(pop[j],n);
    inicializa_vetor(s_star,n);

    pop_sobrev = cria_matriz(nind/2, n);
    fo_pop_sobrev = cria_vetor_float(nind/2);
    inicializa_vetor_float(fo_pop_sobrev,nind/2);
    for (int j = 0; j < nind/2; j++) inicializa_vetor(pop_sobrev[j],n);

    /* Geracao da populacao inicial */
    for (int j = 0; j < nind/2; j++){
        //GRASP(n, pop[j], d, 0.10, 10, 1);
        // constroi_solucao_aleatoria(n, pop[j], d);
        constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n, pop[j], d, 0.5);
        //constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n, pop[j], d, 0.05);
        //constroi_solucao_parcialmente_gulosa_insercao_mais_barata(n, pop[j], d, 0.05);
        fo_pop[j] = calcula_fo(n, pop[j], d);
        if (fo_pop[j] < fo_star){
           fo_star = fo_pop[j];
           atualiza_vetor(s_star, pop[j], n);
        }
    }
    printf("Melhor solucao encontrada na construcao: %f\n", fo_star);

    int ngeracoes = 0;
    float desvio = INT_MAX;
    while (desvio > max_desvio){
      ngeracoes++;
      /* Aplicar crossover */
      int quant_filhos = 0;
      while (quant_filhos < nind/2){
          /* Torneio binario para seleção dos pais */
          int jpai1, jpai2, jpai1_star, jpai2_star;
          float fo_pai1, fo_pai2;
           
          // seleciona o priemeiro pai
          do{
            jpai1 = rand()%(nind/2);
            jpai2 = rand()%(nind/2);
          }while (jpai1 == jpai2);

          fo_pai1 = calcula_fo(n, pop[jpai1], d);
          fo_pai2 = calcula_fo(n, pop[jpai2], d);
          jpai1_star = (fo_pai1 > fo_pai2) ? jpai1 : jpai2;

          // seleciona o segundo pai
          do{
            jpai1 = rand()%(nind/2);
            jpai2 = rand()%(nind/2);
          }while (jpai1 == jpai2 || jpai1 == jpai1_star || jpai2 == jpai1_star);

          fo_pai1 = calcula_fo(n, pop[jpai1], d);
          fo_pai2 = calcula_fo(n, pop[jpai2], d);
          jpai2_star = (fo_pai1 > fo_pai2) ? jpai1 : jpai2;

          jpai1 = jpai1_star;
          jpai2 = jpai2_star;

          if (randomico(0,1) < prob_crossover){
                /* Refinando os pais antes de eles procriarem:
                descida_primeiro_melhora(n,pop[jpai1],d); */
              if (randomico(0,1) < 0.1)
                descida_randomica(n,pop[jpai2],d,n*10);

             if (tipo_operador == 1)
               crossover_OX(pop[jpai1], pop[jpai2], pop[quant_filhos+nind/2], pop[quant_filhos+nind/2+1], n);
             if (tipo_operador == 2)
               crossover_ERX(pop[jpai1], pop[jpai2], pop[quant_filhos+nind/2], pop[quant_filhos+nind/2+1], n);
             quant_filhos+=2;
          }
      }
      /* Aplicar mutacao */
      for (int j = 0; j < nind; j++)
        if (randomico(0,1) < prob_mutacao){
          mutacao(pop[j], n);
        } 
          

      /* Avaliar a populacao inteira (pais e filhos) */
      for (int j = 0; j < nind; j++){
        fo_pop[j] = calcula_fo(n, pop[j], d);
        if (fo_pop[j] < fo_star){
           fo_star = fo_pop[j];
           atualiza_vetor(s_star, pop[j], n);
           printf("Geracao = %4d   fo star = %10.3f\n", ngeracoes, fo_star);
        }
      }

      /* Definir a populacao sobrevivente */
      /* fo de todos os individuos da populacao, isto é, pais e filhos */
      for (int j = 0; j < nind/2; j++){
        /* escolha dos individuos sobreviventes pelo mecanismo da roleta */
        int individuo_escolhido = roleta(nind, fo_pop);
        /* escolha dos individuos sobreviventes pelo mecanismo da roleta
           colocando aptidao nula para aqueles que estiverem acima de uma dada
           faixa, no caso, duas vezes de desvio padrao acima da media */
        //int individuo_escolhido = roleta_scaling(nind, fo_pop);
        for (int i = 0; i < n; i++) pop_sobrev[j][i] = pop[individuo_escolhido][i];
        fo_pop_sobrev[j] = fo_pop[individuo_escolhido];

      }
      /* Zerar a populacao e seus dados */
      inicializa_vetor_float(fo_pop,nind);
      for (int j = 0; j < nind; j++) inicializa_vetor(pop[j],n);
      /* Primeira metade da populacao <-- populacao sobrevivente */
      for (int j = 0; j < nind/2; j++){
        for (int i = 0; i < n; i++) pop[j][i] = pop_sobrev[j][i];
        fo_pop[j] = fo_pop_sobrev[j];
      }


      /* Zerar a matriz e os vetores que armazenam os dados da populacao sobrevivente */
      inicializa_vetor_float(fo_pop_sobrev,nind/2);
      for (int j = 0; j < nind/2; j++) inicializa_vetor(pop_sobrev[j],n);

      /* Calcular o desvio padrão das fos da população */
      desvio = calcula_desvio_padrao(fo_pop,nind/2);
    }

    atualiza_vetor(s, s_star, n);
    fo = fo_star;
    printf("\nNumero de geracoes avaliadas: %d\n", ngeracoes);

    libera_matriz(pop, nind);
    free(fo_pop);
    libera_matriz(pop_sobrev, nind/2);
    free(fo_pop_sobrev);
    libera_vetor(s_star);

    return fo;
}

/* Esta rotina devolve o individuo escolhido pelo mecanismo da roleta */
int roleta(int nind, float *fo_pop)
{
  register int j;
  float aux;
  float *escala, *fracao, *faptidao, tg_alfa, soma, fo_max, fo_min;
  int escolhido;

  fracao = cria_vetor_float(nind);
  escala = cria_vetor_float(nind);
  faptidao = cria_vetor_float(nind);
  soma = 0;
  fo_min = INT_MAX;
  fo_max = -INT_MAX;

  for (int j = 0; j < nind; j++){
    if (fo_pop[j] < fo_min) fo_min = fo_pop[j];
    if (fo_pop[j] > fo_max) fo_max = fo_pop[j];
  }

  tg_alfa = 100 / (fo_max - fo_min);

  for (int j = 0; j < nind; j++){
    faptidao[j] = tg_alfa * (fo_max - fo_pop[j]);
    soma += faptidao[j];
  }

  for (int j = 0; j < nind; j++)
    fracao[j] = faptidao[j] / soma;

  escala[0] = fracao[0];

  for (int j = 1; j < nind; j++)
    escala[j] = escala[j-1] + fracao[j];

  aux = randomico(0,1);
  j = 0;
  while (escala[j] < aux) j++;
  escolhido = j;
  free(fracao);
  free(escala);
  free(faptidao);
  return escolhido;
}

/* Esta rotina devolve o individuo escolhido pelo mecanismo da roleta */
int roleta_scaling(int nind, float *fo_pop)
{
  register int j;
  float aux, alfa, soma, media, somatorio, desvio, desvio_aux;
  float *escala, *fracao, *faptidao;
  int escolhido;

  fracao = cria_vetor_float(nind);
  escala = cria_vetor_float(nind);
  faptidao = cria_vetor_float(nind);
  soma = 0;
  somatorio = 0;
  alfa = 2;
  desvio = 0;

  for (int j = 0; j < nind; j++)
   somatorio += fo_pop[j];

  media = somatorio / nind;

  desvio = calcula_desvio_padrao(fo_pop, nind) * media;

  for (int j = 0; j < nind; j++){
    faptidao[j] = media + alfa * desvio - fo_pop[j];
  }

  for (int j = 0; j < nind; j++) {
    if (faptidao[j] < 0) faptidao[j] = 0;
    soma += faptidao[j];
  }

  for (int j = 0; j < nind; j++)
    fracao[j] = faptidao[j] / soma;

  escala[0] = fracao[0];

  for (int j = 1; j < nind; j++)
    escala[j] = escala[j-1] + fracao[j];

  aux = randomico(0,1);
  j = 0;
  while (escala[j] < aux) j++;
  escolhido = j;
  free(fracao);
  free(escala);
  free(faptidao);
  return escolhido;
}


void mutacao(int *s, int n)
{
   int i, j, cid;

   i = rand()%n;
   do {
     j = rand()%n;
   } while (i == j);

   cid = s[i];
   s[i] = s[j];
   s[j] = cid;
}

/* Operador Crossover OX */
void crossover_OX(int *pai1,
                  int *pai2,
                  int *filho1,
                  int *filho2,
                  int n)
{

  int ponto_de_corte_1, ponto_de_corte_2, i, j;
  bool existe;
  int *lista_pai1, *lista_pai2;

  ponto_de_corte_1 = randomico(2,(n-1)/2);
  ponto_de_corte_2 = randomico((n+1)/2,n-3);

  /* Copia os genes entre os 2 pontos de corte em cada filho */
  for (int i = ponto_de_corte_1; i <= ponto_de_corte_2; i++) {
    filho1[i] = pai1[i];
    filho2[i] = pai2[i];
  }

  int tam_lista = n - (ponto_de_corte_2 - ponto_de_corte_1 + 1);

  /* Cria uma lista com os genes do outro pai a serem inseridos */
  lista_pai1 = cria_vetor(tam_lista);
  lista_pai2 = cria_vetor(tam_lista);

  i = ponto_de_corte_2 + 1;
  j = 0;
  do {
    /* procura a cidade pai1[i] no filho2*/
    existe = false;
    for(int k = ponto_de_corte_1; k <= ponto_de_corte_2; k++) {
      if (filho2[k] == pai1[i]) existe = true;
    }
    if (! existe) {
      lista_pai1[j] = pai1[i];
      j++;
    }
    if (i == (n-1)) i = 0;
    else i++;

  } while(j < tam_lista);

  i = ponto_de_corte_2 + 1;
  j = 0;
  do {
    /* procura a cidade pai2[i] no filho1*/
    existe = false;
    for(int k = ponto_de_corte_1; k <= ponto_de_corte_2; k++) {
      if (filho1[k] == pai2[i]) existe = true;
    }
    if (! existe) {
      lista_pai2[j] = pai2[i];
      j++;
    }

    if (i == (n-1)) i = 0;
    else i++;

  } while(j < tam_lista);

  /* Completa os genes que faltam */
  i = 0;
  j = ponto_de_corte_2 + 1;
  do {
    filho1[j] = lista_pai2[i];
    filho2[j] = lista_pai1[i];
    i++;
    if (j == (n-1)) j = 0;
    else j++;
  } while (i < tam_lista);

  libera_vetor(lista_pai1);
  libera_vetor(lista_pai2);
}

/* Operador Crossover ERX */
void crossover_ERX(int *pai1,
                   int *pai2,
                   int *filho1,
                   int *filho2,
                   int n)
{
  int **arestas1, **arestas2, pos_cid_p1, pos_cid_p2, j, m;
  m = 5;
  arestas1 = cria_matriz(n,m);
  for(int i = 0; i < n; i++) inicializa_vetor(arestas1[i], m);
  arestas2 = cria_matriz(n,m);
  for(int i = 0; i < n; i++) inicializa_vetor(arestas2[i], m);

  /* Preenche a matriz de arestas */
  for(int i = 0; i < n; i++) {
    pos_cid_p1 = busca_pos_valor(pai1, i, n);
    pos_cid_p2 = busca_pos_valor(pai2, i, n);
    j = 1;
    if (pos_cid_p1 == 0) {
      arestas1[i][j] = pai1[pos_cid_p1+1];
      arestas2[i][j] = pai1[pos_cid_p1+1];
      j++;
      arestas1[i][j] = pai1[n-1];
      arestas2[i][j] = pai1[n-1];
      j++;
    }
    else if (pos_cid_p1 == (n-1)) {
      arestas1[i][j] = pai1[pos_cid_p1-1];
      arestas2[i][j] = pai1[pos_cid_p1-1];
      j++;
      arestas1[i][j] = pai1[0];
      arestas2[i][j] = pai1[0];
      j++;
    }
    else {
      arestas1[i][j] = pai1[pos_cid_p1-1];
      arestas2[i][j] = pai1[pos_cid_p1-1];
      j++;
      arestas1[i][j] = pai1[pos_cid_p1+1];
      arestas2[i][j] = pai1[pos_cid_p1+1];
      j++;
    }
    if (pos_cid_p2 == 0 ) {
      arestas1[i][j] = pai2[pos_cid_p2+1];
      arestas2[i][j] = pai2[pos_cid_p2+1];
      j++;
      arestas1[i][j] = pai2[n-1];
      arestas2[i][j] = pai2[n-1];
      j++;
    }
    else if (pos_cid_p2 == (n-1)) {
      arestas1[i][j] = pai2[pos_cid_p2-1];
      arestas2[i][j] = pai2[pos_cid_p2-1];
      j++;
      arestas1[i][j] = pai2[0];
      arestas2[i][j] = pai2[0];
      j++;
    }
    else {
      arestas1[i][j] = pai2[pos_cid_p2-1];
      arestas2[i][j] = pai2[pos_cid_p2-1];
      j++;
      arestas1[i][j] = pai2[pos_cid_p2+1];
      arestas2[i][j] = pai2[pos_cid_p2+1];
      j++;
    }

    /* Retira cidades repetidas e armazena o nº de arestas */
    if (arestas1[i][3] == arestas1[i][1] || arestas1[i][3] == arestas1[i][2]) {
      arestas1[i][3] = -1;
      arestas2[i][3] = -1;
      j--;
    }

    if (arestas1[i][4] == arestas1[i][1] || arestas1[i][4] == arestas1[i][2]) {
      arestas1[i][4] = -1;
      arestas2[i][4] = -1;
      j--;
    }
    arestas1[i][0] = j - 1;
    arestas2[i][0] = j - 1;
  }
  int prox_cid, cid_corrente, cid_rota, cid_aleatoria, num_arestas;

  /* Construção do 1º filho */
  filho1[0] = pai1[0];
  atualiza_arestas(arestas1,n,m,0);
  for (int i = 1; i < n; i++) {
    num_arestas = INT_MAX;
    cid_rota = filho1[i-1];
    prox_cid = -1;
    for (int j = 1; j < m; j++) {
      cid_corrente = arestas1[cid_rota][j];
      if (cid_corrente != -1) {
        if (arestas1[cid_corrente][0] < num_arestas) {
          num_arestas = arestas1[cid_corrente][0];
          prox_cid = cid_corrente;
        }
        else if (arestas1[cid_corrente][0] == num_arestas)
          if (cid_corrente < prox_cid)
            prox_cid = cid_corrente;
      }
    }
    if (prox_cid == -1) {
      do {
        cid_aleatoria = (int) rand()%n;
      } while (foi_inserida(filho1, cid_aleatoria, i));
      prox_cid = cid_aleatoria;
    }
    filho1[i] = prox_cid;
    atualiza_arestas(arestas1,n,m,prox_cid);
  }

  /* Construção do 2º filho */
  filho2[0] = pai2[0];
  atualiza_arestas(arestas2,n,m,0);
  for (int i = 1; i < n; i++) {
    num_arestas = INT_MAX;
    cid_rota = filho2[i-1];
    prox_cid = -1;
    for (int j = 1; j < m; j++) {
      cid_corrente = arestas2[cid_rota][j];
      if (cid_corrente != -1) {
        if (arestas2[cid_corrente][0] < num_arestas) {
          num_arestas = arestas2[cid_corrente][0];
          prox_cid = cid_corrente;
        }
        else if (arestas2[cid_corrente][0] == num_arestas)
          if (cid_corrente < prox_cid)
            prox_cid = cid_corrente;
      }
    }
    if (prox_cid == -1) {
      do {
        cid_aleatoria = (int) rand()%n;
      } while (foi_inserida(filho2, cid_aleatoria, i));
      prox_cid = cid_aleatoria;
    }
    filho2[i] = prox_cid;
    atualiza_arestas(arestas2,n,m,prox_cid);
  }
  libera_matriz(arestas1,n);
  libera_matriz(arestas2,n);
}
