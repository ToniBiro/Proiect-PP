#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "criptare.h"
#include "decriptare.h"
#include "test_chi_patrat.h"
#include "identificare_patternuri.h"

int main()
{

    FILE *fisier_cript_date = fopen("criptare_date.txt", "r");

    if(fisier_cript_date == NULL)
    {
        printf("Eroare deschiderre fisier criptare date!\n");
        return -1;
    }

    char imagine_init[30];
    char imagine_fin[30];
    char cheie[30];

    fscanf(fisier_cript_date, "%s", imagine_init);
    fscanf(fisier_cript_date, "%s", imagine_fin);
    fscanf(fisier_cript_date, "%s", cheie);

    fclose(fisier_cript_date);

    //criptare
    criptare(imagine_init, imagine_fin, cheie);

    FILE *fisier_decript_date = fopen("decriptare_date.txt", "r");

    if(fisier_decript_date == NULL)
    {
        printf("Eroare deschiderrre fisier decriptare date!\n");
        return -1;
    }

    char img_de_decriptat[30];
    char cheie_decript[30];

    fscanf(fisier_decript_date, "%s", img_de_decriptat);
    fscanf(fisier_decript_date, "%s", cheie_decript);

    fclose(fisier_decript_date);

    //decriptare
    decriptare(img_de_decriptat, cheie_decript);


    //template matching
    char fisier_cu_date[] = "fisier_cu_date2.txt";

    if(identificare_patternuri(fisier_cu_date) == 0)
    {
        printf("eroare deschidere fisier!");
        return -1;
    }

    //bordar e(img_init, img_bord, 15/2, 11/2);
}
