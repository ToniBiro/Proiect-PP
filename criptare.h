#ifndef CRIPTARE_H_INCLUDED
#define CRIPTARE_H_INCLUDED

typedef struct
{
    unsigned char r, g, b;
}pixel;

typedef struct
{
    pixel *p;
    int latime, inaltime;
}imagine;

pixel XOR_doi_pixeli(pixel a, pixel b);
pixel XOR_pixel_si_intreg(pixel a, int b);
unsigned int XORSHIFT32(unsigned int seed);
unsigned int* generare_sir_nr_pseudo_aleatoare(unsigned int seed, int n);
void permutare(int n, int *v, unsigned int *r);
void permutare_imagine(imagine *a, unsigned int * r);
void criptare_cu_XOR(imagine *a, unsigned int *r, unsigned int sv);
imagine* liniarizare(char* nume_fisier);
void rescrie(char* fisier_init, char* fisier_fin, imagine *a);
void criptare(char *imagine_init, char *imagine_fin, char *cheie);


#endif // CRIPTARE_H_INCLUDED
