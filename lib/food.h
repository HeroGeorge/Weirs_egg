#ifndef FOOD_H
#define FOOD_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"  // for Position


/** food Class
@author Dr Craig A. Evans, University of Leeds
@brief Controls the food in the Pong game 
@date Febraury 2017
*/ 

class food
{

public:
    food();
    void init(int speed);
    void draw(N5110 &lcd);
    void update();
    /// accessors and mutators
    void set_velocity(Position2D v);
    Position2D get_velocity();
    Position2D get_pos();
    int get_size();
    void set_pos(Position2D p);
    
private:

    Position2D _velocity;
    int _size;
    int _x;
    int _y;
    
};

extern const int food_sprite [12] [12];
extern const int obs_sprite [12] [12];
extern const int sleep_sprite [12] [12];
extern const int grave [20] [28];
extern const int controls[15][27];


#endif