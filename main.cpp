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
#include <chrono>
#include <magnetic_sensor/magnetic_sensor.h>
#include <applications/wasd_control.h>


//Test files
#include "test_inc/driving/test_automatic_movement.h"

#define ULTRASONIC_BRAKE_LIGHT_PIN_WPI 0
#define ULTRASONIC_ECHO_PIN_WPI 1
#define ULTRASONIC_TRIGGER_PIN_WPI 2 

motor_engine* engine = nullptr;
wasd_control* wasd_controller = nullptr;
magnetic_sensor* magnet_sensor = nullptr;
/// Interrupt Routine for STRG-C
void release_resources(){
    if(engine){
        engine->emergency_stop();
        engine->release_engine();
    }
    if(magnet_sensor){
        magnet_sensor->release_resources();
    }
    if(wasd_controller){
        wasd_controller->release_resources();
    }
}
void signalHandler(int signum)
{
    release_resources();
    std::cout << "Strg-C Programmende" << std::endl;
    // Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
    exit(signum);
}


/*
 * Periodically checks distance using the ultrasonic sensor.
 */
void poll_distance(bool *  stop_condition_ptr, ultrasonic_sensor * sensor,
        motor_engine * engine)
{

	double speed = 0;
	double distance = 0;
	double safety_threshold= 0;
	while(true){

		// Adapt threshold depending on current speed.
		speed = engine->get_speed_right();
		if(speed >= 2000){
			safety_threshold = 20;
		}
		else {
			safety_threshold = 10;
		}

		auto dist = sensor->calc_distance();
		if(!dist){
		    *stop_condition_ptr = false;
            continue;
		}

		distance = dist->get();
		// DEBUG CODE
        // plot current speed
		//std::cout << "current distance: " << distance << std::endl;
		//std::cout << "current speed: " << speed << std::endl;


		// Set stop condition to true if the distance is to the wall 
		// ois below our safety threshold. 
		if(distance <= safety_threshold){
			* stop_condition_ptr=true;
			sensor->set_brake_light(ON);
			engine->emergency_stop();

			/*
			 * TURN ROUTINE HERE
			 * TODO TURN 45 degrees for a maximum of 4 times (maybe some cat just ran by)
			 * If distance will be <=10 after one iteration we disable the brake light
			 * and enable remote wasd control again.
			 * otherwise we are stuck inside 4 walls.
			 * and just quit with a "you shall not pass" message? xd
			 */

			// after turn routine set *stop_condition_ptr to false again
			// mock routine by sleep timer
			// pause 2000 ms
			auto start = std::chrono::steady_clock::now();
			auto delay = std::chrono::steady_clock::now();
			while(delay - start < std::chrono::milliseconds(2000)){
				delay = std::chrono::steady_clock::now();
			}
			* stop_condition_ptr=false;

		}
		else {
			//TODO find better solution than turning on every iteration
			sensor->set_brake_light(OFF);
		}

		// pause 100 ms
		auto start = std::chrono::steady_clock::now();
		auto delay = std::chrono::steady_clock::now();
		while(delay - start < std::chrono::milliseconds(100)){
			delay = std::chrono::steady_clock::now();
		}
	}
}


int main () {
    bool stop_condition = false;
    bool *stop_condition_ptr = &stop_condition;
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);
    //init wiringPi
    wiringPiSetup();

    //test blink led
    /*
       pinMode (0, OUTPUT) ;
       do
       {
       digitalWrite (0, HIGH) ; delay (1000) ;
       digitalWrite (0,  LOW) ; delay (1000) ;
       }while(true);
       */

    // init engine
    engine = new motor_engine{make_motor_engine()};
    engine->set_frequency(500.);

    // init sensor
    ultrasonic_sensor ultrasonic(ULTRASONIC_TRIGGER_PIN_WPI, ULTRASONIC_ECHO_PIN_WPI,
                                 ULTRASONIC_BRAKE_LIGHT_PIN_WPI);
    ultrasonic_sensor *ultrasonic_ptr = &ultrasonic;
    ultrasonic.init();

    //Magnetic Sensor
    magnet_sensor = new magnetic_sensor{};
    /*
     * Test code for the ultrasonic sensor. 
     */

#ifdef TEST_ON
    bool ultrasonic_test = false;
    if(ultrasonic_test){
        //debug time diff
        for(int i=0 ;i<100;i++){
            ultrasonic.calc_distance() ;
        }
        for(int i=0 ;i<100;i++){
            auto dist = ultrasonic.calc_distance();
            if(!dist){
                std::cout << "No obstacle in front" << std::endl;
            }else{
                std::cout << "Obstacle in: " << dist->get() << " cm" << std::endl;
            }
        }
        return 0;
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
        delete engine;
        return 0;
    }

    //Test Case for compass: für das drehen des bugg
    bool test_magnetic_sensor = false;
    if (test_magnetic_sensor) {
        for (int i = 0; i < 10000; i++) {
            magnet_sensor->check();
            std::cout << "x,y regs: " << magnet_sensor->x << " " << magnet_sensor->y << std::endl;
            std::cout << "rot: " << magnet_sensor->get_rotation().value << std::endl;
            auto dir = magnet_sensor->get_direction();
            std::cout << "dir : " << dir.x << " " << dir.y << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    bool test_turning = true;
    if(test_turning){
       test_turns(engine, &ultrasonic, magnet_sensor);
    }

#endif //TEST_ON

	wasd_controller = new wasd_control{};
	wasd_controller->run(engine, &ultrasonic, magnetic_sensor);



	// join thread
//	sensor_thread.join();
    release_resources();
}

