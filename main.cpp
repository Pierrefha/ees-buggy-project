/*
 * Pierre Dahmani pd1528s 3215892
 * Jens Peter Dennigmann jd8389s 3190025
 * Leonhard Kipp lk2149s 3188047 
 */

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <magnetic_sensor/compass.h>
#include <iostream>
#include <thread>
#include <motor_engine/constants.h>
#include <motor_engine/motor_engine.h>
#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <chrono>
#include <applications/wasd_control.h>
// !!!! include string for cli comparison
#inlude <string>
// !!!!

#ifdef TEST_ON
#include "test_inc/test.h"
#endif //TEST_ON

#define ULTRASONIC_BRAKE_LIGHT_PIN_WPI 0
#define ULTRASONIC_ECHO_PIN_WPI 1
#define ULTRASONIC_TRIGGER_PIN_WPI 2 

motor_engine* engine = nullptr;
wasd_control* wasd_controller = nullptr;
compass* _compass = nullptr;
ultrasonic_sensor* ultrasonic = nullptr;

/// Interrupt Routine for STRG-C
void release_resources(){
    if(engine){
        engine->emergency_stop();
        engine->release_engine();
    }
    if(ultrasonic){
        ultrasonic->release_resources();
    }
    if(_compass){
        _compass->release_resources();
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

// !!!! added argc and argv for cli
int main (int argc, char** argv) {
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);
    //init wiringPi
    wiringPiSetup();


    // init engine
    engine = new motor_engine{make_motor_engine()};
    engine->set_frequency(500.);

    // init sensor
    ultrasonic = new ultrasonic_sensor(ULTRASONIC_TRIGGER_PIN_WPI, ULTRASONIC_ECHO_PIN_WPI,
                                       ULTRASONIC_BRAKE_LIGHT_PIN_WPI);
    ultrasonic->init();

    //Magnetic Sensor
    _compass = new compass{};
    /*
     * Test code for the ultrasonic sensor. 
     */

#ifdef TEST_ON
    run_tests(engine, ultrasonic, _compass);
#endif //TEST_ON
	// !!!! processing of command line arguments
	char tests[] = "GeneralTest";
	char rotation[] = "Rotation";
	char turns[] = "Drehungen";
	char rectangle[] = "Rechteck";
	char movePoint[] = "Punktbewegung";
	char movePointsRectangle[] = "Punktbewegung_Rechteck";
	char assistedMove[] = "UnterstuetzteBewegung";
	char turn[] = "Drehen";
	for(int i = 1; i < argc; i++){
		
		if(strcmp(argv[i],tests) == 0){
			run_tests(engine, ultrasonic, _compass);
		}
		else if(strcmp(argv[i],rotation) == 0){
			test_rotation_over_time(engine, _compass);
		}
		else if(strcmp(argv[i],turns) == 0){
			test_turns(engine, ultrasonic, _compass);
		}
		else if(strcmp(argv[i],rectangle) == 0){
			test_rectangle(engine, ultrasonic, _compass);
		}
		else if(strcmp(argv[i],movePoint) == 0){
			test_move_to_point(engine, ultrasonic, _compass);
		}
		else if(strcmp(argv[i],movePointsRectangle) == 0){
			test_move_rectangle_with_points(engine, ultrasonic, _compass);
		}
		else if(strcmp(argv[i],assistedMove) == 0){
			test_move_with_direction_control(engine, ultrasonic, _compass);
		}
		else if(strcmp(argv[i],turn) == 0){
			test_turn_by(engine, ultrasonic, _compass);
		}
	}
	// !!!!
	
	wasd_controller = new wasd_control{};
	wasd_controller->run(engine, ultrasonic, _compass);

    release_resources();
}

