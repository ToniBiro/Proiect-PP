#include <stdio.h>
#include <stdlib.h>
#include "criptare.h"
#include "test_chi_patrat.h"

void frecvente_pe_canale(int **r, int **g, int **b, FILE *f, int latime, int inaltime)
{
    *r = (int*)calloc(256, sizeof(int));
    *g = (int*)calloc(256, sizeof(int));
    *b = (int*)calloc(256, sizeof(int));

    int *rr = *r, *gg = *g, *bb = *b;

    fseek(f, 54, SEEK_SET);
    int i;
    for(i = 0; i < latime*inaltime; ++i)
    {
        unsigned char x;
        fread(&x, 1, 1, f);
        //printf("x:%d\n", x);
        bb[x] ++;
        fread(&x, 1, 1, f);
        gg[x] ++;
        fread(&x, 1, 1, f);
        rr[x] ++;
    }
}

float test_chi_patrat_pe_un_canal(FILE *f, int *canal, imagine *a)
{

    int i;
    float rez = 0 , f_barat = (a->latime * a->inaltime) / 256;

    for(i = 0; i < 256; ++i)
        rez += (float)( (canal[i] - f_barat)*(canal[i] - f_barat) ) / (float)f_barat;

    return rez;
}

void test_chi_patrat(char *fisier, imagine *a)
{
    FILE * f = fopen(fisier, "rb");

    int *r, *g, *b;
    frecvente_pe_canale(&r, &g, &b, f, a->latime, a->inaltime);
    printf("testul pe canalul r: %.2f\n", test_chi_patrat_pe_un_canal(f, r, a));
    printf("testul pe canalul g: %.2f\n", test_chi_patrat_pe_un_canal(f, g, a));
    printf("testul pe canalul b: %.2f\n", test_chi_patrat_pe_un_canal(f, b, a));

    fclose(f);
}


