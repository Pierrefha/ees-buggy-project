/*
 * Pierre Dahmani pd1528s 3215892
 * Jens Peter Dennigmann jd8389s 3190025
 * Leonhard Kipp lk2149s 3188047 
 */

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <motor_engine/constants.h>
#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <curses.h>

#define ULTRASONIC_BRAKE_LIGHT_PIN_WPI 0
#define ULTRASONIC_ECHO_PIN_WPI 1
#define ULTRASONIC_TRIGGER_PIN_WPI 2 

motor_engine* engine = nullptr;
/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    endwin();
    std::cout << "Strg-C Programmende" << std::endl;
    // Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
    if(engine){
        engine->emergency_stop();
        engine->release_engine();
    }
    exit(signum);
}

int main ()
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);
    //init wiringPi
    wiringPiSetup () ;

    //test blink led
    /*
    pinMode (0, OUTPUT) ;
    do
    {
        digitalWrite (0, HIGH) ; delay (1000) ;
        digitalWrite (0,  LOW) ; delay (1000) ;
    }while(true);
    */

    engine = new motor_engine{make_motor_engine()};
    engine->set_frequency(1600.);

    //create sensor
    ultrasonic_sensor ultrasonic(ULTRASONIC_TRIGGER_PIN_WPI, ULTRASONIC_ECHO_PIN_WPI,
            ULTRASONIC_BRAKE_LIGHT_PIN_WPI);
    //init sensor
    ultrasonic.init();

    bool auto_movement = false;
    if(auto_movement){
        std::cout << "driving forward" << std::endl;
        engine->set_speed(850);
        engine->forward();
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
        engine->smooth_stop();

        std::cout << "driving backwards" << std::endl;
        engine->set_speed(850);
        engine->backwards();
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
        engine->smooth_stop();

        engine->release_engine();
        delete engine;
    }else{
        char user_cmd = 0;
        initscr();
        cbreak();
        noecho();
        while((user_cmd = getch()) != 'x'){
            const int DIST_THRESHOLD = 10;
            /* TODO uncomment if(ultrasonic.calc_distance() < DIST_THRESHOLD && engine->get_direction() == direction::FORWARD){
                engine->emergency_stop();
            }*/
            switch (user_cmd){
                case 'e':{
                    engine->emergency_stop();
                    break;
                }
                case 'q':{
                    engine->smooth_stop();
                    break;
                }
                case 'w':{
                    switch(engine->get_direction()){
                        case direction::IN_PLACE_TURN_RIGHT:
                        case direction::IN_PLACE_TURN_LEFT:
                            engine->smooth_stop();
                            engine->forward();
                            engine->increase_speed();
                            break;
                        case direction::BACKWARDS:
                            engine->decrease_speed();
                            break;
                        case direction::STOP:
                            engine->forward();
                            engine->increase_speed();
                            break;
                        case direction::FORWARD:
                            engine->increase_speed();
                            break;
                    }
                    break;
                }
                case 's':{
                    switch(engine->get_direction()){
                        case direction::IN_PLACE_TURN_RIGHT:
                        case direction::IN_PLACE_TURN_LEFT:
                            engine->smooth_stop();
                            engine->backwards();
                            engine->increase_speed();
                            break;
                        case direction::FORWARD:
                            engine->decrease_speed();
                            break;
                        case direction::STOP:
                            engine->backwards();
                            engine->increase_speed();
                            break;
                        case direction::BACKWARDS:
                            engine->increase_speed();
                            break;
                    }
                    break;
                }
                case 'd':{
                    switch(engine->get_direction()){
                        case direction::IN_PLACE_TURN_RIGHT:
                            engine->increase_speed();
                            break;
                        case direction::IN_PLACE_TURN_LEFT:
                            engine->decrease_speed();
                            break;
                        case direction::FORWARD:
                        case direction::BACKWARDS:
                            engine->turn_right();
                            break;
                        case direction::STOP:
                            engine->turn_in_place_right();
                            engine->increase_speed();
                            break;
                    }
                    break;
                }
                case 'a':{
                    switch(engine->get_direction()){
                        case direction::IN_PLACE_TURN_RIGHT:
                            engine->decrease_speed();
                            break;
                        case direction::IN_PLACE_TURN_LEFT:
                            engine->increase_speed();
                            break;
                        case direction::FORWARD:
                        case direction::BACKWARDS:
                            engine->turn_left();
                            break;
                        case direction::STOP:
                            engine->turn_in_place_left();
                            engine->increase_speed();
                            break;
                    }
                    break;
                }
                //check distance
                case 'c':{
				 //debug time diff
				 ultrasonic.calc_distance();
                         }

                default: break;
            }

        }

        engine->smooth_stop();
        engine->release_engine();
        endwin();
    }
}
