//11colx12rows = 12x11
#ifndef EGGS_H
#define EGGS_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"
#include <map>


extern const int egg_1 [12] [11];
extern const int egg_2 [12] [11];
extern const int egg_3 [12] [11];
extern const int egg_4 [12] [11];
extern const int egg_5 [12] [11];

void draw_eggs(N5110 &lcd);


#endif


