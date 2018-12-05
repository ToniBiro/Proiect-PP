#include <stdio.h>
#include <stdlib.h>
#include "criptare.h"
#include "greyscale.h"
#include "test_chi_patrat.h"

int main()
{
    char imagine[] = "peppers.bmp";
    char imagine_permutata[] = "permutata.bmp";

    pixel *p = (pixel*)liniarizare(imagine);
    p = (pixel*)criptare(p, imagine);
    rescrie(imagine, imagine_permutata, p);

    //testare de chi-patrat

    FILE * f = fopen(imagine_permutata, "rb");

    int *r, *g, *b;
    frecvente_pe_canale(&r, &g, &b, f);
    printf("testul pe canalul r: %.2f\n", test_chi_patrat(f, r));
    printf("testul pe canalul g: %.2f\n", test_chi_patrat(f, g));
    printf("testul pe canalul b: %.2f\n", test_chi_patrat(f, b));

    fclose(f);

    return 0;
}
