#include "criptare.h"

typedef struct
{
    unsigned char r, g, b;
}pixels;

void obtinere_latime_inaltime_img(FILE * f, int *latime, int *inaltime)
{
    fseek(f, 18, SEEK_SET);
    fread(latime, sizeof(int), 1, f);
    fseek(f, 22, SEEK_SET);
    fread(inaltime, sizeof(int), 1, f);
}

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

unsigned int XORSHIFT32(unsigned int seed)
{
    seed = seed ^ seed << 13;
    seed = seed ^ seed >> 17;
    seed = seed ^ seed << 5;

    return seed;
}

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

void permutare(int n, int *v, unsigned int * r)
{
    unsigned int i, j;
    for(i = 1; i < n; ++i)
    {
        j = r[i] % i;
        int aux = v[i];
        v[i] = v[j];
        v[j] = aux;
    }
}

pixel* permutare_imagine(pixel *p, int latime, int inaltime, unsigned int * r)
{
    int n = latime * inaltime;
    int i;

    int *v = (int*)malloc(sizeof(int)*n);

    for(i = 0; i < n; ++i)
        v[i] = i;

    //permutarea cu ajutorul sirului pseudo-aleator
    permutare(n, v, r);

    //aloc dinamic spatiu pt vectorul cu pizxeli permutat
    pixel *rez = (pixel*)malloc(sizeof(pixel)*n);

    //permutarea imaginii
    for(i = 0; i < n; ++i)
        rez[v[i]] = p[i];

    return rez;
}

pixel* criptare_cu_XOR(pixel *img_permutata, int n, unsigned int *sir, unsigned int sv)
{
    // n este lungimea imaginii (latimea * inaltimea)

    pixel *c = (pixel*)malloc(sizeof(pixel) * n);

    c[0] = XOR_pixel_si_intreg(XOR_pixel_si_intreg(img_permutata[0], sv), sir[n]);

    int i;
    for(i = 1; i < n; ++i)
        c[i] = XOR_pixel_si_intreg(XOR_doi_pixeli(c[i-1], img_permutata[i]), sir[n+i]);

    return c;
}

pixel* liniarizare(char* nume_fisier)
{
    FILE *f = fopen(nume_fisier, "rb");
    if(f == NULL)
    {
        printf("Eroare deschidere");
        return NULL;
    }

    //obtin latimea si inaltimea imaginii in pixeli
    int latime, inaltime;
    obtinere_latime_inaltime_img(f, &latime, &inaltime);
    printf("--latime: %d\n--inaltime: %d\n", latime, inaltime);

    //padding
    int padding;
    if(latime%4  != 0)
        padding = 4 - (3 * latime)%4;
    else
        padding = 0;

    //liniarizez imaginea

    fseek(f, 54, SEEK_SET);
    int i, j, k;
    char t;
    pixel *p = (pixel*)malloc(latime*inaltime*sizeof(pixel));
    int h = 0;
    for( i = 0; i < inaltime; ++i)
    {
        for(j = 0; j < latime; ++j)
        {
            fread(&p[h].b, 1, 1, f);
            fread(&p[h].g, 1, 1, f);
            fread(&p[h++].r, 1, 1, f);
        }
        for(k = 0; k < padding; ++k)
            fread(&t, 1, 1, f);
    }
    int lungime = ftell(f);
    printf("\nlungime: %d\n", lungime);

    fclose(f);
    return p;
}

pixel* criptare(pixel *p, unsigned char *imagine)
{
    FILE *f = fopen(imagine, "rb");

    int latime, inaltime;
    obtinere_latime_inaltime_img(f, &latime, &inaltime);

    fclose(f);

    //generez sir de nr psedo-aleatoare

    unsigned int * r;
    r = generare_sir_nr_pseudo_aleatoare(123456789, latime * inaltime);

    //permut imaginea cu ajutorul primei jumatati a sirului pseudo-aleator

    pixel *img_permutata = permutare_imagine(p, latime, inaltime, r);

    //criptez cu xor folosind a doua jumatate a sirului pseudo-aleator

    int sv = 987654321;
    pixel *c;
    c = criptare_cu_XOR(img_permutata, latime*inaltime, r, sv);

    return c;
}

void rescrie(char* fis_init, char *nume_rez, pixel *p)
{
    //deschidere fisiere
    FILE *f = fopen(fis_init, "rb");
    FILE *fout = fopen(nume_rez, "wb");

    if(f == NULL)
    {
        printf("Eroare deschidere");
        return;
    }
    fseek(f, 0L, SEEK_SET);

    //latime si inaltime
    int latime, inaltime;
    obtinere_latime_inaltime_img(f, &latime, &inaltime);
    printf("--latime: %d\n--inaltime: %d\n", latime, inaltime);

    //aflare padding
    int padding;
    if(latime%4  != 0)
        padding = 4 - (3 * latime)%4;
    else
        padding = 0;
    fseek(f, 0L, SEEK_SET);
    //copiere header
    int i;
    for(i = 0; i < 54; ++i)
    {
        char x;
        fread(&x, 1, 1, f);
        fwrite(&x, 1, 1, fout);
    }

    //refacere imagine
    int j, k, l = 0, h = 0;
    for(i = 0; i < inaltime; ++i)
    {
        for(j = 0; j < latime; ++j)
        {
            fwrite(&p[h].b, 1, 1, fout);
            fwrite(&p[h].g, 1, 1, fout);
            fwrite(&p[h++].r, 1, 1, fout);
        }
        for(k = 0; k < padding; ++k)
            fwrite(&l, 1, 1, fout);
    }

    int lungime = ftell(fout);
    printf("\n lungime:%d\n", lungime);

    fclose(f);
    fclose(fout);
}
