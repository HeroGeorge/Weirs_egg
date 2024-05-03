#include "weirEngine.h"

std::map<std::string, const int(*)[12][11]> egg_map = {
    {"egg_1", &egg_1},
    {"egg_2", &egg_2},
    {"egg_3", &egg_3},
    {"egg_4", &egg_4},
    {"egg_5", &egg_5}
};

weirEngine::weirEngine(){ _hunger = 10; }    

void weirEngine::init(int weir_position, int speed){
    //printf("Pong Engine: Init\n");
    _sleep = 0;
    _feed = 0;
    _speed = speed;
    _weir.init(weir_position);
}

void weirEngine::feed_weir(bool feed){
    _feed = feed;
    int speed = _speed;

    if(_feed == 1){
    _food.init(speed);
    } else {
        _food.init(speed);
    }
}

void weirEngine::sleep_weir(bool sleep){
    _sleep = sleep;
}

int weirEngine::update(UserInput input, int& hunger) {   
    //printf("Pong Engine: Update\n");
    //check_goal(); // checking for a goal is a priority 
    _food.update();
    _weir.update(input);
    // important to update paddles and food before checking collisions so can
    // correct for it before updating the display

    _hunger = hunger;
    check_weir_collision(hunger);
    
    return _hunger;
}

void weirEngine::draw(N5110 &lcd, const std::string &selected_egg) { //pass by the value of selected_egg string
    // draw the elements in the LCD buffer
    lcd.drawLine(0, 10, 84, 10, 1); //sky
    lcd.drawRect(69, 1, 13, 15, FILL_TRANSPARENT);
    lcd.drawLine(0, 30, 84, 30, 1); //ground

    const int (*egg_sprite)[12][11] = egg_map[selected_egg];
    if(egg_sprite){
        lcd.drawSprite(70, 2, 12, 11, (int *)*egg_sprite); //
    }

    _food.draw(lcd);

    if (_sleep) {
        _weir.draw(lcd, 1);
    } else {
        _weir.draw(lcd, 0);

    }
}


/*void weirEngine::check_wall_collision() {
    //printf("Pong Engine: Check Wall Collision\n");
    // read current food attributes
    Position2D food_pos = _food.get_pos();
    Position2D food_velocity = _food.get_velocity();

   
     if (food_pos.y + 12 >= (HEIGHT-1) ) {
        // hit bottom
        food_pos.y = (HEIGHT-1) - size;  // stops food going off screen
        food_velocity.y = 0;    // stops food 
    } 

    // update food parameters
    _food.set_velocity(food_velocity);
    _food.set_pos(food_pos);
}
*/


void weirEngine::check_weir_collision(int hunger_val) {
    //printf("Pong Engine: Check Paddle Collision\n");
    // read current food and paddle attributes
    Position2D food_pos = _food.get_pos();
    Position2D food_velocity = _food.get_velocity();
    Position2D weir_pos = _weir.get_pos();  // paddle
    Position2D reset;
    reset.x = 0;
    reset.y = 0;

    // see if food has hit the weir by checking for overlaps
    if (
        (food_pos.y + 10 >= weir_pos.y - 1 ) && //top
        (food_pos.y + 1 <= weir_pos.y - 1) //bottom
       /(food_pos.x + 1>= weir_pos.x) && //left
        (food_pos.x + 1 <= weir_pos.x + _weir.get_width() )  //right
    ) {
            food_pos.x = 100;  
            _hunger = hunger_val + 1;
    }
    // write new attributes
    _food.set_velocity(food_velocity);
    _food.set_pos(food_pos);
}

/*void weirEngine::check_goal() {
    //printf("Pong Engine: Check Goal\n");
    Position2D food_pos = _food.get_pos();
    int speed = abs(_food.get_velocity().x);  // speed is magnitude of velocity
    // check if food position has gone off the left
    if (food_pos.x + 12 < 0) {
        // reset the food
        _food.init(speed);
        _hunger--;  // lose a life
    }   
}*/