#ifndef weir_H
#define weir_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"

class weir
{
public:

    weir();
    void init(int x);
    void draw(N5110 &lcd, bool sleeping);
    void update(UserInput input);
    void add_score();
    int get_score();
    Position2D get_pos();
    int get_height();
    int get_width();

private:

    int _height;
    int _width;
    int _x;
    int _y;
    int _speed;
    int _score;


};


extern const int weir_idle_1 [12][12];
extern const int weir_idle_2 [12][12];


#endif