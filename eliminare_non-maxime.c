#include "eliminare_non-maxime.h"

//functia de comparare pentru qsort intre doua detectii
int cmp(const void *a, const void* b)
{
    detectie x = *(detectie*)a;
    detectie y = *(detectie*)b;

    float rez = y.scor - x.scor;
    if(rez < 0)
        return -1;
    if(rez > 0)
        return 1;
    return 0;
}

int arie(detectie a, detectie b)
{
    int latime = a.w - (a.j > b.j ? a.j - b.j : b.j - a.j);
    int inaltime = a.h - (a.i > b.i ? a.i - b.i : b.i - a.i);
    if(latime < 0 || inaltime < 0)
        return -1;

    return latime * inaltime;
}

void elimin_non_maxime(vect_detectii *f)
{
    int i, j;
    int arie_d = f->d[0].w * f->d[0].h;

    for(i = 0; i < f->lng-1; ++i)
    {
        for(j = i+1; j < f->lng; ++j)
        {
            int arie_intersectie = arie(f->d[i], f->d[j]);
            if(arie_intersectie < 0)
                continue;

            float suprap = (float)arie_intersectie / (arie_d + arie_d - arie_intersectie);

            if(suprap > 0.2)
                f->d[j].scor = -2;
        }
    }
}
