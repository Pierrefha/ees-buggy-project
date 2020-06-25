//
// Created by leonhard on 25.06.20.
//

#include "../test_inc/test.h"
#include "../test_inc/driving/test_automatic_movement.h"
#include "../test_inc/calibration.h"
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
void run_tests(motor_engine *engine, ultrasonic_sensor *ultrasonic, compass *_compass) {
    bool ultrasonic_test = false;
    if(ultrasonic_test){
        //debug time diff
        for(int i=0 ;i<100;i++){
            ultrasonic->calc_distance() ;
        }
        for(int i=0 ;i<100;i++){
            auto dist = ultrasonic->calc_distance();
            if(!dist){
                std::cout << "No obstacle in front" << std::endl;
            }else{
                std::cout << "Obstacle in: " << dist->get() << " cm" << std::endl;
            }
        }
    };

    /*
     * Test code for the motor engine.
     */
    bool auto_movement = false;
    if (auto_movement) {
        std::cout << "driving forward" << std::endl;
        engine->set_speed(850);
        engine->forward();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        engine->smooth_stop();

        std::cout << "driving backwards" << std::endl;
        engine->set_speed(850);
        engine->backwards();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        engine->smooth_stop();

        engine->release_engine();
    }

    //Test Case for compass: für das drehen des bugg
    bool test_magnetic_sensor = false;
    if (test_magnetic_sensor) {
        for (int i = 0; i < 10000; i++) {
//            _compass->check();
//            std::cout << "x,y regs: " << _compass->x << " " << _compass->y << std::endl;
            std::cout << "rot: " << _compass->get_rotation().value << std::endl;
            auto dir = _compass->get_direction();
            std::cout << "dir : " << dir.x << " " << dir.y << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    //test blink led
    /*
       pinMode (0, OUTPUT) ;
       do
       {
       digitalWrite (0, HIGH) ; delay (1000) ;
       digitalWrite (0,  LOW) ; delay (1000) ;
       }while(true);
       */



    //test_rotation_over_time(engine, _compass);
    //test_rectangle(engine, ultrasonic, _compass);
    //test_move_to_point(engine, ultrasonic, _compass);
    //test_turns(engine, ultrasonic, magnet_sensor);
}
