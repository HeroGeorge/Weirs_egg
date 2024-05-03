
///////////// includes /////////////////////
#include "mbed.h"
#include "Joystick.h"
#include "N5110.h"  
#include "weirEngine.h"
#include "Utils.h"
#include "eggs.h"
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <map>
#include <string>
#include <iostream>
#include <chrono>

//note definitions
#define NOTE_C4  262    //octave 4 - middle c
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

///////////// objects ///////////////////
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
Joystick joystick(PC_0, PC_1); //y , x
BusOut life_led(PC_9, PB_8);// red, green
DigitalIn buttonA(PC_2); //onboard user button
DigitalIn buttonB(PC_3); //to be changed
PwmOut buzzer(PA_15); //Buzzer 
weirEngine Weir;

///////////// prototypes ///////////////
void init();
void render();
void welcome();
std::string egg_select();
void game_over();
void help();
void hatch();
void music();
void feed_event();
void sleep_event();
void lose_hunger();
void hunger_led();
void sleep_sr();



///////////// defines ///////////////////// 
const int start_tone[] = {NOTE_G4, NOTE_A4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_A4, NOTE_E4, NOTE_C5}; //create array with the required notes (in order)
std::string selected_egg; //global variable to store selected egg 

Thread music_thread;

volatile bool sleep_flag = 0;

Ticker hunger_loss;

int time_score;
int itr = 0;
int hunger = 10;


int main() {
    music_thread.start(music);
    init();      // initialise devices and objects
    welcome();
    selected_egg = egg_select(); 
    hatch();

    render();    // first draw the initial frame 
    int fps = 30;
    thread_sleep_for(1000/fps);  // and wait for one frame period - millseconds
    hunger_loss.attach(&lose_hunger, 1s);

    while (hunger > 0 && hunger < 21) {  // keep looping while hunger remains 0-20
    printf("hunger = %d\n", hunger);
        sleep_event();

        if (!sleep_flag) {
            buzzer.resume();
            lcd.setBrightness(0.45);
            lcd.setContrast(0.5);
            UserInput input = {joystick.get_direction(),joystick.get_mag()};
            hunger = Weir.update(input, hunger);   // update the game engine based on input    
            feed_event();
            render();
            thread_sleep_for(1000/fps);

                if (hunger <= 3 || hunger >= 18)life_led = 0b01; //red flash taken out as it slowed down the thread significantly
                else life_led = 0b10;

        }   else {
            render();
            thread_sleep_for(1200);
            buzzer.suspend();
            lcd.setBrightness(0);
            lcd.setContrast(0);
            life_led = 0;
        }
    }
    game_over();
}

void play_note(int frequency){
    buzzer.period_us((float) 1000000.0f/ (float) frequency);    //set the period of the pwm signal (in us)
    buzzer.pulsewidth_us(buzzer.read_period_us()/4);            //set pulse width of the pwm to 1/2 the period
    ThisThread::sleep_for(700ms);                              
}

void music(){
    while (1) {
        for (int i = 0; i < 8; i++) {
            play_note(start_tone[i]);
        }   
    }
}


void lose_hunger(){
    itr++;
    if (itr % 3 == 0 && itr!= 0) {
        hunger--;
    }
}


void init() {
    printf("Init Function Run\n");
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    lcd.setBrightness(0.45);
    joystick.init();
    life_led = 0b00;
    buttonA.mode(PullUp);
    buttonB.mode(PullUp);
    Weir.init(33, 3);

    while(1){
    lcd.clear();
    lcd.drawRect(2, 2, 81, 45, FILL_TRANSPARENT);
    lcd.drawRect(30, 19, 33, 21, FILL_TRANSPARENT);
    lcd.printString("   Controls   ", 0, 1);
    lcd.drawSprite(33, 22, 15, 27, (int *)controls);
    lcd.refresh();
    if (buttonA.read() == 1 || buttonB.read() == 1) {
        thread_sleep_for(300);
        break;
        }
    }
}

void feed_event(){
    if (buttonA.read() == 1) {
        Weir.feed_weir(1);
        thread_sleep_for(300);
    }
}

void sleep_event(){
    if (buttonB.read() == 1){
        thread_sleep_for(300);
        Weir.sleep_weir(!sleep_flag);
        sleep_flag = !sleep_flag;
    }
}
void render() {  // clear screen, re-draw and refresh
    lcd.clear();  
    Weir.draw(lcd, selected_egg);
    lcd.refresh();
}

void welcome() { 
    printf("\nWelcome Function Run\n");
    while(1){
    lcd.clear();
    lcd.printString("  WEIRS EGG   ",0,1);  
    lcd.printString("A - Hatch",0,3);
    lcd.printString("B - Help",0,4);
    lcd.refresh();

    while(1){
        if(buttonB.read() == 1){ 
            thread_sleep_for(100);
            help();
            thread_sleep_for(1500);
            break;
        }
        else if (buttonA.read() == 1) { 
            thread_sleep_for(300);
            return;
            }
        }
    }
}

std::string egg_select(){
    printf("\nEgg_select Function Run\n");
    int rect_x;
    bool selected = false;
    std::map<int, std::string> eggs{
        {2, "egg_1"},
        {20, "egg_2"},
        {38, "egg_3"},
        {56, "egg_4"},
        {71, "egg_5"}};

    rect_x = eggs.begin()->first; //initialize rect_x to the key of the first element

    // Initialize the iterator and joystick holder
    auto current_egg = eggs.begin();
    volatile bool joystick_moved = false;

    // LCD setup initially
    lcd.clear();
    draw_eggs(lcd);
    lcd.drawRect(rect_x, 20, 13, 15, FILL_TRANSPARENT);
    lcd.refresh();

    while (!selected) {
        if (!joystick_moved) {
            if (joystick.get_direction() == E) {
                printf("E\n");

                // Move the iterator to the next element
                ++current_egg;

                // Wrap around if reached the end
                if (current_egg == eggs.end())
                    current_egg = eggs.begin();

                // Update rect_x
                rect_x = current_egg->first;

                // Set joystick move flag
                joystick_moved = true;
                printf("JY E, R = %d\n", rect_x);

                // Redraw eggs and rectangle
                lcd.clear();
                draw_eggs(lcd);
                lcd.drawRect(rect_x, 20, 13, 15, FILL_TRANSPARENT);
                lcd.refresh();
            } else if (joystick.get_direction() == W) {
                                printf("W\n");
                // Move the iterator to the previous element
                if (current_egg == eggs.begin())
                    current_egg = std::prev(eggs.end()); // Move to the last element
                else
                    --current_egg;

                // Update rect_x
                rect_x = current_egg->first;

                // Set joystick move flag
                joystick_moved = true;
                printf("JY E, R = %d\n", rect_x);

                // Redraw eggs and rectangle
                lcd.clear();
                draw_eggs(lcd);
                lcd.drawRect(rect_x, 20, 13, 15, FILL_TRANSPARENT);
                lcd.refresh();
            }
        }

        // Reset joystick move flag if the joystick returns to the center position
        if (joystick.get_direction() == CENTRE){
            joystick_moved = false;
        }

        if (buttonA.read() == 1) {
            thread_sleep_for(100);
            printf("Egg selected\n");
            selected = true;
            lcd.clear();
        }
    }
    thread_sleep_for(300);
    printf("Rect X = %d\n", rect_x);
    printf("%s\n", eggs[rect_x].c_str());
    return eggs[rect_x];
}

void game_over() { // splash screen 
    printf("\nGame Over Function Run\n");
    buzzer.suspend();
    time_score = (itr);
    char buffer[14];
    sprintf(buffer, "    %d S    ", time_score);
    while (1) {
        lcd.clear();
        lcd.printString("  Game Over ", 0, 1);
        lcd.printString(buffer,0, 5);
        lcd.drawSprite(22, 17, 20, 28, (int *)grave);
        lcd.refresh();
        thread_sleep_for(300);
        lcd.clear();
        lcd.refresh();
        thread_sleep_for(300);
    }
}

void hatch(){
    printf("\nHatch Function Run\n");
    int i = 0;
    while ( i < 42) {
    lcd.randomiseBuffer();
    lcd.refresh();
    lcd.clear();
    thread_sleep_for(12);
    i++;
    }
    lcd.clear();
    lcd.printString("   Hatching   ", 0, 2);
    lcd.printString("   Complete   ", 0, 3);
    lcd.refresh();
    ThisThread::sleep_for(1500ms);
    lcd.clear();

}

void help(){

    printf("\nHelp Function Run\n");
    lcd.clear();
    lcd.printString("How To Play",0,0);
    lcd.printString("Weir is Happy",0,2);
    lcd.printString("to make a new",0,3);
    lcd.printString("friend.",0,4);
    lcd.refresh();
    thread_sleep_for(5000);
    lcd.clear();
    lcd.printString("Develope your",0,0);
    lcd.printString("egg by playing",0,1);
    lcd.printString("games to make",0,2);
    lcd.printString("weir as ",0,3);
    lcd.printString("COMPLETE",0,4);
    lcd.printString("as possible. ",0,5);
    lcd.refresh();
    thread_sleep_for(5000);
    lcd.clear();
    lcd.printString("Feed(A) & make",0,0);
    lcd.printString("Weir sleep(B)",0,1);
    lcd.printString("when neccesary.",0,2);
    lcd.printString("Have fun with ",0,3);
    lcd.printString("your friend",0,4);
    lcd.printString("and Good Luck.	",0,5);
    lcd.refresh();
    thread_sleep_for(5000);
    return;
}