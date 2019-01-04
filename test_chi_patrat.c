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

float test_chi_patrat_pe_un_canal(FILE *f, int *canal, int latime, int inaltime)
{
    int i;

    float rez = 0 , f_barat = (latime * inaltime) / 256.0;

    for(i = 0; i < 256; ++i)
        rez += (float)( (canal[i] - f_barat)*(canal[i] - f_barat) ) / (float)f_barat;

    return rez;
}

void test_chi_patrat(char *fisier)
{
    FILE * f = fopen(fisier, "rb");
    if(f == NULL)
    {
        printf("Eroare deschidere fisier %s!", fisier);
        exit(7);
    }

    int inaltime, latime;
    fseek(f, 18, SEEK_SET);
    fread(&latime, sizeof(int), 1, f);
    fread(&inaltime, sizeof(int), 1, f);

    int *r, *g, *b;
    frecvente_pe_canale(&r, &g, &b, f, latime, inaltime);
    printf("testul pe canalul r: %.2f\n", test_chi_patrat_pe_un_canal(f, r, latime, inaltime));
    printf("testul pe canalul g: %.2f\n", test_chi_patrat_pe_un_canal(f, g, latime, inaltime));
    printf("testul pe canalul b: %.2f\n", test_chi_patrat_pe_un_canal(f, b, latime, inaltime));
    free(r);
    free(g);
    free(b);

    fclose(f);
}


