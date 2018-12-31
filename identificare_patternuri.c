#include "identificare_patternuri.h"

//functia calculeaza detectiile si coloreaza pentru fiecare sablon cu culoarea corespunzatoare
int identificare_patternuri(char *fisier_cu_date)
{
    FILE *f_date = fopen(fisier_cu_date, "r");

    if(f_date == NULL)
    {
        printf("eroare deschidere fisier cu numele imaginilor!");
        return 0;
    }

    char img_init[30];
    fscanf(f_date, "%s", img_init);

    char img_gray[] = "img_gray.bmp";

    //grayscale
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
    while(1)
    {
        char sablon[30];
        fscanf(f_date, "%s", sablon);
        printf("sablon nume:%s\n", sablon);

        //aleg culoarea
        pixel *cul = aleg_culoare(sablon);
        printf("r:%d g:%d b:%d\n", cul->r, cul->g, cul->b);

        FILE *f_sab = fopen(sablon, "rb");
        if(f_sab == NULL)
        {
            printf("eroare deschidere fisier sablon");
            return 0;
        }

        //aflare lungime si latime sablon
        int inaltime_sab, latime_sab;
        fseek(f_sab, 18, SEEK_SET);
        fread(&latime_sab, sizeof(int), 1, f_sab);
        fseek(f_sab, 22, SEEK_SET);
        fread(&inaltime_sab, sizeof(int), 1, f_sab);

        fseek(f_sab, 0L, SEEK_SET);

        //liniarizare sabloane
        imagine *sab = liniarizare(sablon);
        printf("sablon dupa liniarizare");

        //detectie sablonul sab
        f = template_matching(*gray, *sab, 0.5, *cul);

        fin->d = (detectie*) realloc(fin->d, sizeof(detectie)*(fin->lng + f->lng));

        for(int i = fin->lng; i < fin->lng + f->lng; ++i)
            fin->d[i] = f->d[i-fin->lng];
        fin->lng += f->lng;

        fclose(f_sab);

        if(feof(f_date))
            break;

    }

    //sortarea descrescatoare dupa scor a detectiilor din vectorul final fin
    qsort(fin->d, fin->lng, sizeof(detectie), cmp);

    //eliminarea non maximelor din vectorul fin
    elimin_non_maxime(fin);

    //colorez
    colorez(icolor, *fin);

    char imagine_colorata[] = "img_colorata.bmp";

    rescrie(img_init, imagine_colorata, icolor);

    return 1;
}
