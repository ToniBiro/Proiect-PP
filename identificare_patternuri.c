#include "identificare_patternuri.h"

//functia calculeaza detectiile si coloreaza pentru fiecare sablon cu culoarea corespunzatoare
void identificare_patternuri(FILE *f_date)
{
    char img_init[1024];
    fscanf(f_date, "%1024s", img_init);

     //grayscale
    char img_gray[] = "img_gray.bmp";
    grayscale_image(img_init, img_gray);

    //liniarizez imaginea
    FILE *f_gray = fopen(img_gray, "rb");

    //aflare latime si inaltime imagine gray
    int inaltime_img, latime_img;
    fseek(f_gray, 18, SEEK_SET);
    fread(&latime_img, sizeof(int), 1, f_gray);
    fseek(f_gray, 22, SEEK_SET);
    fread(&inaltime_img, sizeof(int), 1, f_gray);

    fseek(f_gray, 0L, SEEK_SET);

    //liniarizare imaginea initiala
    imagine *icolor = liniarizare(img_init);

    //liniarizare imaginea gray
    imagine *gray = liniarizare(img_gray);

    //aloc memorie pentru vectorul ce va contine toate detectiile
    vect_detectii *fin = (vect_detectii*) malloc(sizeof(vect_detectii));
    fin->d = (detectie*) malloc(0);
    fin->lng = 0;
    vect_detectii *f;

    int n;
    fscanf(f_date, "%d", &n);
    for(int u = 0; u < n; ++u)
    {
        char sablon[1024];
        fscanf(f_date, "%1024s", sablon);

        //aleg culoarea
        pixel *cul = aleg_culoare(sablon);

        FILE *f_sab = fopen(sablon, "rb");
        if(f_sab == NULL)
        {
            printf("eroare deschidere fisier sablon");
            exit(7);
        }
        char buf[1024];
        //grayscale sablon
        sprintf(buf, "gray_%s", sablon);
        grayscale_image(sablon, buf);

        //aflare lungime si latime sablon
        int inaltime_sab, latime_sab;
        fseek(f_sab, 18, SEEK_SET);
        fread(&latime_sab, sizeof(int), 1, f_sab);
        fseek(f_sab, 22, SEEK_SET);
        fread(&inaltime_sab, sizeof(int), 1, f_sab);

        fseek(f_sab, 0L, SEEK_SET);

        //liniarizare sabloane
        imagine *sab = liniarizare(buf);

        //detectie sablonul sab
        f = template_matching(*gray, *sab, 0.5, *cul);

        fin->d = (detectie*) realloc(fin->d, sizeof(detectie)*(fin->lng + f->lng));

        for(int i = fin->lng; i < fin->lng + f->lng; ++i)
            fin->d[i] = f->d[i-fin->lng];
        fin->lng += f->lng;

        fclose(f_sab);

        //eliberez memoria alocata dinamic pentru imaginea sab si vectorul intermediar de detectii
        free(sab->p);
        free(sab);
        free(f->d);
        free(f);
    }

    //sortarea descrescatoare dupa scor a detectiilor din vectorul final fin
    qsort(fin->d, fin->lng, sizeof(detectie), cmp);

    //eliminarea non maximelor din vectorul fin
    elimin_non_maxime(fin);

    //colorez
    colorez(icolor, *fin);

    char imagine_colorata[] = "img_colorata.bmp";

    rescrie(img_init, imagine_colorata, icolor);

    //eliberez memoria alocata dinamic
    free(icolor->p);
    free(icolor);
    free(gray->p);
    free(gray);
    free(fin->d);
    free(fin);
}
