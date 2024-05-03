#ifndef PONGENGINE_H
#define PONGENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "food.h"
#include "Utils.h"
#include "weir.h"
#include <map>
#include <string>
#include "eggs.h"

class weirEngine {
    public:
        weirEngine();  // pass in the lcd object from the main file
        void init(int weir_position, int speed);
        int update(UserInput input, int& hunger);
        void draw(N5110 &lcd, const std::string &selected_egg);
        void feed_weir(bool feed);
        void sleep_weir(bool sleep);


    private:
        void check_wall_collision();
        void check_weir_collision(int hunger_val);
        void check_goal();
        bool _feed;
        bool _sleep;
        food _food;
        int _speed;
        weir _weir;
        int _hunger;
};


#endif