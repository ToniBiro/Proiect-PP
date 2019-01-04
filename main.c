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
    FILE *fisier_cu_cai = fopen("fisier_cu_cai.txt", "r");

    if(fisier_cu_cai == NULL)
    {
        printf("Eroare la deschiderea fisierului cu cai!\n");
        return -1;
    }

    char imagine_init[1024];
    char imagine_fin[1024];
    char cheie[1024];

    fscanf(fisier_cu_cai, "%1024s", imagine_init);
    fscanf(fisier_cu_cai, "%1024s", imagine_fin);
    fscanf(fisier_cu_cai, "%1024s", cheie);

    printf("test chi - patrat imagine necriptata:\n");
    test_chi_patrat(imagine_init);

    //criptare
    criptare(imagine_init, imagine_fin, cheie);

    printf("test chi - patrat imagine criptata:\n");
    test_chi_patrat(imagine_fin);

    char img_de_decriptat[1024];
    char cheie_decript[1024];

    fscanf(fisier_cu_cai, "%1024s", img_de_decriptat);
    fscanf(fisier_cu_cai, "%1024s", cheie_decript);

    //decriptare
    decriptare(img_de_decriptat, cheie_decript);

    //template matching
    identificare_patternuri(fisier_cu_cai);

    fclose(fisier_cu_cai);

    return 0;

}
