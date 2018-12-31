#include <stdio.h>
#include <stdlib.h>
#include "criptare.h"
#include "decriptare.h"

void permutare_inversa(imagine *a, unsigned int *r)
{
    int n = a->inaltime*a->latime;
    //creez permutarea in functie de sirul generat
    int *v = (int*) malloc(sizeof(int) * n);
    int i;
    for(i = 0; i < n; ++i)
        v[i] = i;

    permutare(n, v, r);

    //permut invers imaginea
    pixel *rez = (pixel*) malloc(sizeof(pixel) * a->latime * a->inaltime);
    for(i = 0; i < a->latime * a->inaltime; ++i)
        rez[i] = a->p[v[i]];

    free(a->p);
    a->p = rez;
}

void decriptare_cu_XOR(imagine *a, unsigned int *r, unsigned int sv)
{
    int n = a->inaltime*a->latime;
    pixel *c = (pixel*)malloc(sizeof(pixel) * n);

    c[0] = XOR_pixel_si_intreg(XOR_pixel_si_intreg(a->p[0], sv), r[n]);

    int i;
    for(i = 1; i < n; ++i)
        c[i] = XOR_pixel_si_intreg(XOR_doi_pixeli(a->p[i-1], a->p[i]), r[n+i]);

    free(a->p);
    a->p = c;
}

void decriptare(char *img_de_decriptat, char *cheie)
{
    //citre seed, sv
    FILE *f = fopen(cheie, "r");

    if(f == NULL)
    {
        printf("Eroare deschidere fisier cheie");
        return;
    }

    int seed, sv;
    fscanf(f, "%d", &seed);
    fscanf(f, "%d", &sv);

    fclose(f);

    //liniarizare imagine criptata
    imagine *a = liniarizare(img_de_decriptat);

    //generare sir pseudo aleator
    unsigned int *r = generare_sir_nr_pseudo_aleatoare(seed, a->latime * a->inaltime);

    //decriptarea cu xor
    decriptare_cu_XOR(a, r, sv);

    //crearea permutarii inverse
    permutare_inversa(a, r);

    //rescriere imagine
    rescrie(img_de_decriptat, "imagine_decriptata.bmp", a);
    free(a->p);
    free(a);
}
