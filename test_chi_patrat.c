#include "test_chi_patrat.h"

void frecvente_pe_canale(int **r, int **g, int **b, FILE *f)
{
    int latime, inaltime;
    obtinere_latime_inaltime_img(f, &latime, &inaltime);

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

float test_chi_patrat(FILE *f, int *canal)
{
    int latime, inaltime;
    obtinere_latime_inaltime_img(f, &latime, &inaltime);

    int i;
    float rez = 0 , f_barat = (latime * inaltime) / 256;

    for(i = 0; i < 256; ++i)
        rez += (float)( (canal[i] - f_barat)*(canal[i] - f_barat) ) / (float)f_barat;

    return rez;
}
