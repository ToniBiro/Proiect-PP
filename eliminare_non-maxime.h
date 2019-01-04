#ifndef ELIMINARE_NON_MAXIME_H_INCLUDED
#define ELIMINARE_NON_MAXIME_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "cross_correlation.h"
int cmp(const void *a, const void* b);
int arie(detectie a, detectie b);
void elimin_non_maxime(vect_detectii *f);

#endif // ELIMINARE_NON_MAXIME_H_INCLUDED
