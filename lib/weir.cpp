#include "weir.h"

// nothing doing in the constructor and destructor
weir::weir() { }

void weir::init(int y) {
    //printf("weir: Init\n");
    _y = y;  // x value on screen is fixed
    _x = HEIGHT/2 - 12/2;  // y depends on height of screen and height of weir
    _height = 12;
    _width = 12;
    _speed = 1;  // default speed
    _score = 0;  // start score from zero
}

void weir::draw(N5110 &lcd, bool sleeping) { 
    //printf("weir: Draw\n");
    if(sleeping){
        lcd.printString("Zzz", _x, 2);
        lcd.drawSprite(_x,_y,_width,_height,(int *)weir_idle_2);
    } else {
        lcd.drawSprite(_x,_y,_width,_height,(int *)weir_idle_1);
    }
}

void weir::update(UserInput input) {
    //printf("weir: Update\n");
    _speed = 4;
    // update y value depending on direction of movement
    // North is decrement as origin is at the top-left so decreasing moves up
    if (input.d == W) { 
        _x -= _speed;
        }
    else if (input.d == E) { 
        _x += _speed;
         }

    // check the y origin to ensure that the weir doesn't go off screen
    if (_x < 1) { _x = -1; }
    if (_x > WIDTH - (_width-2)) { _x = (WIDTH - _width- 2); }
}

void weir::add_score() { _score++; }

int weir::get_score() { return _score; }

Position2D weir::get_pos() { return {_x,_y}; }

int weir::get_height() { return _height; }

int weir::get_width() { return _width; }

//idle weir
const int weir_idle_1 [12] [12] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,1,0,0,1,0,1,0,0},
    {0,0,1,0,1,0,0,1,0,1,0,0},
    {0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,1,1,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
}; 

const int weir_idle_2 [12] [12] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,1,0,1,0,0,1,0,1,0,0},
    {0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,1,1,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
}; 

//walk animations
const int weir_right_1 [12][12] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,0,1,0,1,1,0,0},
    {0,0,1,0,0,1,0,1,1,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,1,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0},
};

const int weir_right_2 [12][12] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,0,1,0,1,1,0,0},
    {0,0,1,0,0,1,0,1,1,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,1,0,0,0},
    {0,1,1,0,0,0,0,1,0,0,0},
    {0,1,0,0,0,0,0,1,1,0,0},
};

const int weir_right_3 [12][12] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,1,0,1,1,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,1,1,1,0,0,0,0,0},
};

const int weir_right_4 [12][12] = {
    {0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,1,0,1,1,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,1,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,1,0,0,1,0,0,0},
    {0,0,1,1,0,0,0,0,1,0,0},
    {0,0,1,0,0,0,0,0,1,1,0},
};

//walk order 1,2,3,4,3,2,1