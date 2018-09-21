//
// Created by Puca Penna on 21/09/2018.
//

#ifndef PCV_ILS_H
#define PCV_ILS_H

float ILS(int n, int *s, float **d, float fo, int nivelPert, int ILSMax);
float perturbacao(int n, int *s, int *s_linha, float **d, float fo, int nivelPert);

float SmartILS(int n, int *s, float **d, float fo, int nivelPert, int PertMax, int ILSMax);

#endif //PCV_ILS_H
