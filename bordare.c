#include "bordare.h"




//functia bordare primeste o imagine si latimea + inaltimea marginii pe care trebuie sa o adauge
//creeaza o imagine noua
void bordare(char *img_init, char* img_fin, int inaltime_bord, int latime_bord)
{
    FILE *f = fopen(img_init, "rb+");
    FILE *g = fopen(img_fin, "wb+");

    if(f == NULL)
    {
        printf("EROARE DESCHIDERE FISIER LA BORDARE\n");
        return;
    }

    int inaltime_img, latime_img;

    fseek(f, 18, SEEK_SET);
    fread(&latime_img, sizeof(int), 1, f);
    printf("latime imagine de bordat:%d\n", latime_img);

    fseek(f, 22, SEEK_SET);
    fread(&inaltime_img, sizeof(int), 1, f);
    printf("inaltime imagine de bordat:%d\n", inaltime_img);

    //copiez headerul
    fseek(f, 0L, SEEK_SET);
    for(int i = 0; i < 54; ++i)
    {
        char x;
        fread(&x, 1, 1, f);
        fwrite(&x, 1, 1, g);
    }

    //modific latimea si inaltimea in header

    fseek(g, 18, SEEK_SET);
    latime_img += latime_bord*2;
    fwrite(&latime_img, sizeof(int), 1, g);

    fseek(g, 22, SEEK_SET);
    inaltime_img += inaltime_bord*2;
    fwrite(&inaltime_img, sizeof(int), 1, g);

    fseek(g, 54, SEEK_SET);
    fseek(f, 54, SEEK_SET);


    //calculare padding-ul

    int padding;
    if(latime_img%4  != 0)
        padding = 4 - (3 * latime_img)%4;
    else
        padding = 0;


    //bordez
    //partea de sus
    int y = 0;
    for(int i = 0; i < inaltime_bord; ++i)
    {
        for(int j = 0; j < latime_img; ++j)
            fwrite(&y, 1, 3, g);
        //padding
        for(int k = 0; k < padding; ++k)
            fwrite(&y, 1, 1, g);
    }

    //laterale
    for(int i = 0; i < inaltime_img - inaltime_bord*2; ++i)
    {
        for(int j = 0; j < latime_bord; ++j)
            fwrite(&y, 1, 3, g);

        for(int j = 0; j < latime_img - latime_bord*2; ++j)
        {
            char x;
            fread(&x, 1, 1, f);
            fwrite(&x, 1, 1, g);
            fread(&x, 1, 1, f);
            fwrite(&x, 1, 1, g);
            fread(&x, 1, 1, f);
            fwrite(&x, 1, 1, g);
        }

        for(int j = 0; j < latime_bord; ++j)
            fwrite(&y, 1, 3, g);
        //padding
        for(int k = 0; k < padding; ++k)
            fwrite(&y, 1, 1, g);
    }

    //partea de jos
    for(int i = 0; i < inaltime_bord; ++i)
    {
        for(int j = 0; j < latime_img; ++j)
            fwrite(&y, 1, 3, g);
        //padding
        for(int k = 0; k < padding; ++k)
            fwrite(&y, 1, 1, g);
    }


    fclose(f);
    fclose(g);
}
