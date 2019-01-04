#include <stdio.h>
#include <stdlib.h>
#include "criptare.h"
#include "test_chi_patrat.h"


pixel XOR_doi_pixeli(pixel a, pixel b)
{
    pixel rez;
    rez.b = a.b ^ b.b;
    rez.g = a.g ^ b.g;
    rez.r = a.r ^ b.r;

    return rez;
}

pixel XOR_pixel_si_intreg(pixel a, int b)
{
    pixel rez;
    char *p = (char*)&b;
    rez.b = a.b ^ *p;
    p++;
    rez.g = a.g ^ *p;
    p++;
    rez.r = a.r ^ *p;

    return rez;
}

//functie genereaza si returneaza un numar aleator in functie de seed-ul transmis ca parametru
unsigned int XORSHIFT32(unsigned int seed)
{
    seed = seed ^ seed << 13;
    seed = seed ^ seed >> 17;
    seed = seed ^ seed << 5;

    return seed;
}

//functia genereaza si returneaza un sir pseudo-aleator de lungime 2*latime*inaltime imagine, sir utilizat in criptarea imaginii
unsigned int* generare_sir_nr_pseudo_aleatoare(unsigned int seed, int n)
{
    unsigned int *r = (unsigned int*)malloc(sizeof(unsigned int) * 2 * n);
    r[0] = seed;

    int i;
    for(i = 1; i < 2 * n; ++i)
    {
        r[i] = XORSHIFT32(seed);
        seed = r[i];
    }

    return r;
}

//functia creaza o permutare de n elemente cu ajutorul sirului pseudo-aleator r
void permutare(int n, int *v, unsigned int *r)
{
    unsigned int i, j;
    for(i = 0; i < n; ++i)
        v[i] = i;

    for(i = n-1; i >= 1; --i)
    {
        j = r[n-i+1] % (i+1);
        int aux = v[i];
        v[i] = v[j];
        v[j] = aux;
    }
}

//functia primeste ca parametru o imagine si ii permuta pixelii
void permutare_imagine(imagine *a, unsigned int * r)
{
    int n = a->latime * a->inaltime;
    int i;
    int *v = (int*)malloc(sizeof(int)*n);

    //permutarea cu ajutorul sirului pseudo-aleator
    permutare(n, v, r);

    //aloc dinamic spatiu pt vectorul cu pixeli permutati
    pixel *rez = (pixel*)malloc(sizeof(pixel)*n);

    //permutarea imaginii
    for(i = 0; i < n; ++i)
        rez[v[i]] = a->p[i];

    free(a->p);
    a->p = rez;
    free(v);
}

//functia primeste o imagine cu pixelii permutati si aplica asupra fiecarui pixel criptarea cu xor
//este folosita a doua jumatate a sirului pseudo-aleator r
void criptare_cu_XOR(imagine *a, unsigned int *r, unsigned int sv)
{
    int n = a->latime * a->inaltime;

    pixel *c = (pixel*)malloc(sizeof(pixel) * n);

    c[0] = XOR_pixel_si_intreg(XOR_pixel_si_intreg(a->p[0], sv), r[n]);

    int i;
    for(i = 1; i < n; ++i)
        c[i] = XOR_pixel_si_intreg(XOR_doi_pixeli(c[i-1], a->p[i]), r[n+i]);

    free(a->p);
    a->p = c;
}

//functia primeste un nume de fisier cu o imagine bmp, aloca dinamic spatiu pentru o imagine
//o liniarizeaza si returneaza adresa la care a fost memorata
imagine* liniarizare(char* nume_fisier)
{
    FILE *f = fopen(nume_fisier, "rb");
    if(f == NULL)
    {
        printf("Eroare deschidere");
        exit(7);
    }

    imagine *a = (imagine*) malloc(sizeof(imagine));

    //obtin latimea si inaltimea imaginii in pixeli

    fseek(f, 18, SEEK_SET);
    fread(&a->latime, sizeof(int), 1, f);
    fseek(f, 22, SEEK_SET);
    fread(&a->inaltime, sizeof(int), 1, f);
    //printf("--latime img de liniarizat: %d\n--inaltime img de liniarizat: %d\n", a->latime, a->inaltime);

    //se calculeaza padding-ul
    int padding;
    if(a->latime%4  != 0)
        padding = 4 - (3 * a->latime)%4;
    else
        padding = 0;

    //liniarizarea imaginii
    fseek(f, 54, SEEK_SET);
    int i, j, k;
    char t;
    a->p = (pixel*)malloc(a->latime*a->inaltime*sizeof(pixel));
    int h = 0;
    for( i = 0; i < a->inaltime; ++i)
    {
        for(j = 0; j < a->latime; ++j)
        {
            fread(&a->p[h].b, 1, 1, f);
            fread(&a->p[h].g, 1, 1, f);
            fread(&a->p[h++].r, 1, 1, f);
        }
        for(k = 0; k < padding; ++k)
            fread(&t, 1, 1, f);
    }
    int lungime = ftell(f);
    //printf("\nlungime img de liniarizat: %d\n", lungime);

    fclose(f);
    return a;
}

//functia rescrie imaginea criptata liniarizata la o forma neliniarizata
//este nevoie de fisierul initial pentru a copia header-ul
void rescrie(char* fisier_init, char* fisier_fin, imagine *a)
{
    //deschidere fisiere
    FILE *fin = fopen(fisier_init, "rb");
    FILE *fout = fopen(fisier_fin, "wb");

    if(fin == NULL)
    {
        printf("Eroare deschidere");
        return;
    }

    //se calculeaza padding-ul
    int padding;
    if(a->latime%4  != 0)
        padding = 4 - (3 * a->latime)%4;
    else
        padding = 0;

    fseek(fin, 0L, SEEK_SET);
    //copiere header
    int i;
    for(i = 0; i < 54; ++i)
    {
        char x;
        fread(&x, 1, 1, fin);
        fwrite(&x, 1, 1, fout);
    }

    //refacere imagine
    int j, k, t = 0, h = 0;
    for(i = 0; i < a->inaltime; ++i)
    {
        for(j = 0; j < a->latime; ++j)
        {
            fwrite(&a->p[h].b, 1, 1, fout);
            fwrite(&a->p[h].g, 1, 1, fout);
            fwrite(&a->p[h++].r, 1, 1, fout);
        }
        for(k = 0; k < padding; ++k)
            fwrite(&t, 1, 1, fout);
    }

    int lungime = ftell(fout);
    //printf("\n lungime:%d\n", lungime);

    fclose(fin);
    fclose(fout);
}

//functia liniarizeaza imaginea primita in fisierul imagine_init
//modifica vectorul de pixeli obtinut si rescrie imaginea criptata in fisierul imagine_fin
//in fisierul cheie returneaza seed-ul si sv-ul
void criptare(char *imagine_init, char *imagine_fin, char *cheie)
{
    FILE *f = fopen(cheie, "r");

    unsigned int seed, sv;
    fscanf(f, "%u %u", &seed, &sv);

    fclose(f);

    //liniarizare imagine
    if(strstr(imagine_init, ".bmp") == NULL)
    {
        printf("Nu este un fisier de tip bmp.");
        return NULL;
    }

    imagine *a = liniarizare(imagine_init);

    //modificare vector de pixeli
    //se genereaza sir de nr psedo-aleatoare

    unsigned int * r;

    r = generare_sir_nr_pseudo_aleatoare(seed, a->latime * a->inaltime);

    //permut imaginea cu ajutorul primei jumatati a sirului pseudo-aleator

    permutare_imagine(a, r);

    //criptez cu xor folosind a doua jumatate a sirului pseudo-aleator


    criptare_cu_XOR(a, r, sv);

    //rescriere a imaginii
    rescrie(imagine_init, imagine_fin, a);

    free(a->p);
    free(a);


}
