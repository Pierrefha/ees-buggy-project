#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <motor_engine/constants.h>
#include <motor_engine/motor_engine.h>
#include <curses.h>

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
    wiringPiSetup () ;
    pinMode (0, OUTPUT) ;
    do
    {
        digitalWrite (0, HIGH) ; delay (500) ;
        digitalWrite (0,  LOW) ; delay (500) ;
    }while(false);


    engine = new motor_engine{make_motor_engine()};
    engine->set_frequency(1600.);

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
                default: break;
            }

        }

        engine->smooth_stop();
        engine->release_engine();
        endwin();
    }
}
