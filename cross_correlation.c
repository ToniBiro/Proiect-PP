#include "cross_correlation.h"



//calculul intensitatii medii
double intensitate_medie(imagine a, int x, int y, int h, int w)
{
    int i, j;
    double s = 0;
    for(i = x; i < x + h; ++i)
    {
        for(j = y; j < y + w; ++j)
        {
            s += a.p[i*a.latime + j].r;
        }
    }

    double rez = s / (h*w);
    return rez;
}

//calcul deviatie standard
double deviatie_standard(imagine a, int x, int y, int h, int w, double intensitate_med)
{
    float raport = 1.0 / (( h*w ) - 1 );
    int i, j;
    double s = 0;
    for( i = x; i < x + h; ++i)
    {
        for(j = y; j < y + w; ++j)
        {
            s += (a.p[i*a.latime + j].r - intensitate_med) * (a.p[i*a.latime + j].r - intensitate_med);
        }
    }
    double rez = sqrt(raport * s);
    return rez;
}

//calcularea scorului de cross correlation
double calculare_scor(imagine f, imagine sablon, int x, int y)
{
    //calcul conform formulei
    int n_sab = sablon.latime * sablon.inaltime;
    double n_raport = 1.0 / n_sab;

    double fereastra_medie = intensitate_medie(f, x, y, sablon.inaltime, sablon.latime);
    double sablon_medie = intensitate_medie(sablon, 0, 0, sablon.inaltime, sablon.latime);

    double dev_fereastra = deviatie_standard(f, x, y, sablon.inaltime, sablon.latime, fereastra_medie);
    double dev_sablon = deviatie_standard(sablon, 0, 0, sablon.inaltime, sablon.latime, sablon_medie);

    if(dev_fereastra * dev_sablon == 0)
        return -1;

    double d_raport = 1.0 / (dev_sablon * dev_fereastra);

    int i, j;
    double s = 0;
    for(i = 0; i < sablon.inaltime; ++i)
    {
        for(j = 0; j < sablon.latime; ++j)
        {
            s += d_raport * ( f.p[(i+x)*f.latime + (j+y)].r - fereastra_medie) * ( sablon.p[i*sablon.latime + j].r - sablon_medie );
            //printf("f: %d %d %d   s: %d %d %d\n", f.p[(i+x)*f.latime + (j+y)].r, f.p[(i+x)*f.latime + (j+y)].g, f.p[(i+x)*f.latime + (j+y)].b, sablon.p[i*sablon.latime + j].r, sablon.p[i*sablon.latime + j].g, sablon.p[i*sablon.latime + j].b);
        }
    }
    double rez = n_raport * s;
    return rez;
}

//functia returneaza un vector de detectii pentru un sablon dat care au un scor mai mare de pragul ps
vect_detectii* template_matching(imagine a, imagine sab, float ps, pixel cul)
{
    //aloc memorie pentru matricea de ferestre detectate peste pragul ps

    vect_detectii *v = (vect_detectii*) malloc(sizeof(vect_detectii));
    v->d = (detectie*) malloc(0);
    v->lng = 0;

    //intensitati
    int i, j;
    for(i = 0; i < a.inaltime - sab.inaltime + 1; ++i)
    {
        for(j = 0; j < a.latime - sab.latime + 1; ++j)
        {
            double scor = calculare_scor(a, sab, i, j);
            if( scor >= ps)
            {
                ++v->lng;
                v->d = (detectie*) realloc(v->d, sizeof(detectie)*v->lng);
                v->d[v->lng-1].scor = scor;
                v->d[v->lng-1].i = i;
                v->d[v->lng-1].j = j;
                v->d[v->lng-1].h = sab.inaltime;
                v->d[v->lng-1].w = sab.latime;
                v->d[v->lng-1].c = cul;
            }
        }
    }

    return v;
}
