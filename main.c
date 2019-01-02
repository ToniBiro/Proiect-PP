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
    /// Testament  - Biro Balan Antonia - (multumiri lui Gabi si Wombat(radu))
    ///te iubesc mult mult mult si calduros dragul meu ALEXE si Ghidoveanu, mersi radu (wombat), amm..te rog da un 10 pentru Ateneu :)
    ///in cazul in care acest comentariu ramana aici, am fost beata cand l-am scris
    ///totul e !false ,mersi gabi, pentru ca cand esti beat iese la iveala adevarul
    ///in vino veritas, mersi gabi
    ///dragul meu Dragulici (Licurici) imi cer scuze pentru ca nu stiu sa inmultesc,dar stii bine
    /// ca iubirea noastra nu poate fi oprita de un mic detaliu ca acesta
    ///ar trebui sa vezi cum inmultesc aia din clasa de stiinte sociale de la Miercurea Ciuc
    FILE *fisier_cript_date = fopen("criptare_date.txt", "r");

    if(fisier_cript_date == NULL)
    {
        printf("Eroare deschiderre fisier criptare date!\n");
        return -1;
    }

    char imagine_init[1024];
    char imagine_fin[1024];
    char cheie[1024];

    fscanf(fisier_cript_date, "%s", imagine_init);
    fscanf(fisier_cript_date, "%s", imagine_fin);
    fscanf(fisier_cript_date, "%s", cheie);

    fclose(fisier_cript_date);

    printf("test chi - patrat imagine necriptata:\n");
    test_chi_patrat(imagine_init);

    //criptare
    criptare(imagine_init, imagine_fin, cheie);

    printf("test chi - patrat imagine criptata:\n");
    test_chi_patrat(imagine_fin);

    //deschidere fisier pentru decriptare
    FILE *fisier_decript_date = fopen("decriptare_date.txt", "r");

    if(fisier_decript_date == NULL)
    {
        printf("Eroare deschiderrre fisier decriptare date!\n");
        return -1;
    }

    char img_de_decriptat[1024];
    char cheie_decript[1024];

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
    return 0;

}
