//
// Created by Puca Penna on 13/09/2018.
//

#ifndef PCV_SA_H
#define PCV_SA_H
float SA(int n, int *s, float **d, float alpha, float SAmax, double temp_inicial, double temp_final);

float temperaturaInicial(int n, int *s, float **d, float beta, float gama, float SAmax, double temp_inicial);

#endif //PCV_SA_H
