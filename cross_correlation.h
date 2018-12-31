#ifndef CROSS_CORRELATION_H_INCLUDED
#define CROSS_CORRELATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "criptare.h"


typedef struct
{
    float scor;
    int i, j;
    int h, w;
    pixel c;
}detectie;

typedef struct
{
    detectie *d;
    int lng;
}vect_detectii;

double intensitate_medie(imagine a, int x, int y, int h, int w);
double deviatie_standard(imagine a, int x, int y, int h, int w, double intensitate_med);
double calculare_scor(imagine f, imagine sablon, int x, int y);
vect_detectii* template_matching(imagine a, imagine sab, float ps, pixel cul);


#endif // CROSS_CORRELATION_H_INCLUDED
