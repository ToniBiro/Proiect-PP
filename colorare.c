#include "colorare.h"

//colorez in imaginea a conturul ferestrei f cu culoarea data de pixelul c
void color_fereastra(imagine *a, detectie f, pixel c)
{
    int i, j;
    for(i = f.i; i < f.i + f.h; ++i)
    {
        a->p[i*a->latime + f.j] = c;
        a->p[i*a->latime + f.j + (f.w - 1)] = c;
    }

    for(j = f.j; j < f.j + f.w; ++j)
    {
        a->p[f.i * a->latime + j] = c;
        a->p[(f.i + f.h - 1) * a->latime + j] = c;
    }
}

//functia coloreaza conturul tuturor ferestrelor din vectorul de detectii cu culoarea pixelului c
void colorez(imagine *a, vect_detectii f)
{
    //dechidere fisierul in care colorez

    int i;
    for(i = 0; i < f.lng; ++i)
    {
        if(f.d[i].scor != -2)
            color_fereastra(a, f.d[i], f.d[i].c);
    }

}

//din numele sablonului ales functia decide culoarea acelei cifre
pixel* aleg_culoare(char *sablon)
{
    pixel *c = (pixel*)malloc(sizeof(pixel));
    c->r = 0; c->g = 0; c->b = 0;
    if(strchr(sablon, '0') != NULL)
    {
        c->r = 255; c->g = 0; c->b = 0;
        return c;
    }
    if(strchr(sablon, '1') != NULL)
    {
        c->r = 255; c->g = 255; c->b = 0;
        return c;
    }
    if(strchr(sablon, '2') != NULL)
    {
        c->r = 0; c->g = 255; c->b = 0;
        return c;
    }
    if(strchr(sablon, '3') != NULL)
    {
        c->r = 0; c->g = 255; c->b = 255;
        return c;
    }
    if(strchr(sablon, '4') != NULL)
    {
        c->r = 255; c->g = 0; c->b = 255;
        return c;
    }
    if(strchr(sablon, '5') != NULL)
    {
        c->r = 0; c->g = 0; c->b = 255;
        return c;
    }
    if(strchr(sablon, '6') != NULL)
    {
        c->r = 192; c->g = 192; c->b = 192;
        return c;
    }
    if(strchr(sablon, '7') != NULL)
    {
        c->r = 255; c->g = 140; c->b = 0;
        return c;
    }
    if(strchr(sablon, '8') != NULL)
    {
        c->r = 128; c->g = 0; c->b = 128;
        return c;
    }
    if(strchr(sablon, '9') != NULL)
    {
        c->r = 128; c->g = 0; c->b = 0;
        return c;
    }
    return c;
}
