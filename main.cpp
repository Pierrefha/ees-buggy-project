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
#include <thread>
#include <chrono>
#include <magnetic_sensor/magnetic_sensor.h>
#include <fstream>
#include <util/time_util.h>

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


/*
 * Periodically checks distance using the ultrasonic sensor.
 * Will enable the emergency break if obstacle is to close.
 */
void poll_distance(bool *  stop_condition_ptr, ultrasonic_sensor * sensor,
                   motor_engine * engine)
{

	double speed = 0;
	double distance = 0;
	double safety_threshold= 0;
	while(true){

		// Adapt threshold depending on current speed.
		speed = engine->get_speed();
		if(speed >= 2000){
			safety_threshold = 20;
		}
		else {
			safety_threshold = 10;
		}

		distance = sensor->calc_distance()->get();

		// Set stop condition to true if the distance to
        // the obstacle is below our safety threshold.
		if(distance <= safety_threshold){
			* stop_condition_ptr=true;
			engine->emergency_stop();
			sensor->set_brake_light(ON);
			/*
			 * TODO TURN ROUTINE HERE
			 * TODO TURN 45 degrees for a maximum of 4 times
			 * If the measured distance will be <=4cm after one iteration
             * we can enable the remote control again.
			 */

			// mock turning routine by a sleep timer
            buy_wait(std::chrono::milliseconds(2000));
            sensor->set_brake_light(OFF);
			* stop_condition_ptr=false;
		}
		// check every 100 ms
        busy_wait(std::chrono:milliseconds(100));
		}
	}
}


int main ()
{
	bool stop_condition = false;
	bool * stop_condition_ptr = & stop_condition;
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

	// init engine
	engine = new motor_engine{make_motor_engine()};
	engine->set_frequency(1600.);

	// init sensor
	ultrasonic_sensor ultrasonic(ULTRASONIC_TRIGGER_PIN_WPI
                                 ULTRASONIC_ECHO_PIN_WPI
                                 ULTRASONIC_BRAKE_LIGHT_PIN_WPI);
	ultrasonic_sensor * ultrasonic_ptr = & ultrasonic;
	ultrasonic.init();

	//Magnetic Sensor
	magnetic_sensor magneticSensor;
	std::ofstream outputData;


    /*
     * Test code for the ultrasonic sensor. 
     */
	bool ultrasonic_test = false;
	if(ultrasonic_test){
		//debug time diff
		for(int i=0 ;i<100;i++){
			ultrasonic.calc_distance() ;
		}
		for(int i=0 ;i<100;i++){
			std::cout << ultrasonic.calc_distance()->get() << std::endl;
		}
		return 0;
	};

    /*
     * Test code for the motor engine.
     */
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
		return 0;
	}


    /*
     * Remote control of the motor engine using wasd keys.
     */
	char user_cmd = 0;
	initscr();
	cbreak();
	noecho();

    // periodically poll distance
    std::thread sensor_thread(poll_distance,stop_condition_ptr,ultrasonic_ptr,engine);
	while((user_cmd = getch()) != 'x'){
		
		//TEST FOR SENSOR
		if(magneticSensor.check()==1){
			outputData.open("outputData.txt", std::ios_base::app);
			outputData << magneticSensor.getX() << " " << magneticSensor.getY() << " " << magneticSensor.getZ() << "\n";
			//Output magnetic test
			outputData.close();
			//std::cout << "Direction: " << magneticSensor.testDirection() << std::endl;
		} else {
			std::cout << "No new Data" << std::endl;
		}
		
		
		//END TEST
		/*
		 * Check if remote control is stopped. Which is the case if our buggy is 
		 * not below our safety distance threshold. 
		 */
		if(* stop_condition_ptr){
			std::cout << "Remote control is disabled while distance is below"
                      << " safety threshold!" << std::endl;
		}
		else {
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
				//Test Case for compass: für das drehen des buggys
				case 'r':{
					for(int i = 0; i < 1000; i++){
						magneticSensor.check();
						outputData.open("outputData.txt", std::ios_base::app);
						outputData << magneticSensor.getX() << " " << magneticSensor.getY() << " " << magneticSensor.getZ() << "\n";
						outputData.close();
						//etwas warten damit der Kompass neue Daten laden kann
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}
				}
				
				default: break;
			}
		}
	}
	
    ultrasonic.set_brake_light(OFF);
	engine->smooth_stop();
	engine->release_engine();
	endwin();
}
