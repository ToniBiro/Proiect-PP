#ifndef IDENTIFICARE_PATTERNURI_H_INCLUDED
#define IDENTIFICARE_PATTERNURI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "grayscale.h"
#include "cross_correlation.h"
#include "colorare.h"
#include "eliminare_non-maxime.h"
void identificare_patternuri(FILE *f_date);

#endif // IDENTIFICARE_PATTERNURI_H_INCLUDED
