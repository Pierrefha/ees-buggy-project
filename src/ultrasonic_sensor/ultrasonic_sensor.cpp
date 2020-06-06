#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <wiringPi.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <iostream>


ultrasonic_sensor::ultrasonic_sensor(int8_t trigger_pin, int8_t echo_pin, int8_t brake_light_pin){    
    this->trigger_pin = trigger_pin;
    this->echo_pin = echo_pin;
    this->brake_light_pin = brake_light_pin;
    this->time_diff=0;
    this->distance=0;
}


/*
 * Initializes the required pins for the ultrasonic sensor.
 */
void ultrasonic_sensor::init(){
    pinMode(this->trigger_pin,OUTPUT);
    digitalWrite(this->trigger_pin,LOW);
    pinMode(this->echo_pin,INPUT);
    pinMode(this->brake_light_pin,OUTPUT);
    digitalWrite(this->brake_light_pin,LOW);

    // make sure pins are initialized before continuing    
    auto start = std::chrono::steady_clock::now();
    auto delay = std::chrono::steady_clock::now();
    while(delay - start < std::chrono::microseconds(50)){
	    delay = std::chrono::steady_clock::now();
    }
}


/*
 * Measures the time between trigger and echo
 * @returns time difference in microseconds
 */
double ultrasonic_sensor::measure_time_diff(){

	double total_time = 0;
	const int32_t measurements = 30;
	int32_t succeeded_measurements = 0;

	//measure x times and calc average result because of high variance in single results
	for (int i=0; i<measurements;++i){

		//sleep so echo pin can reset at next measurement
		auto start = std::chrono::steady_clock::now();
		auto delay = std::chrono::steady_clock::now();
		while(delay - start < std::chrono::microseconds(1000)){
			delay = std::chrono::steady_clock::now();
		}
		// send ultrasonic signal
		digitalWrite(this->trigger_pin, HIGH);
		start = std::chrono::steady_clock::now();
		delay = std::chrono::steady_clock::now();

		while(delay - start < std::chrono::microseconds(20)){
			delay = std::chrono::steady_clock::now();
		}
		digitalWrite(this->trigger_pin, LOW);

		start = std::chrono::steady_clock::now();
		delay = std::chrono::steady_clock::now();
		auto timeout = std::chrono::milliseconds(50);

		// wait until echo pin is high
		// wait until response received
		while (digitalRead(this->echo_pin) == LOW && delay -start<timeout){
			delay = std::chrono::steady_clock::now();
		}

		// if timeout / no wave received, skip this measurement
		if(delay - start >= timeout){
			// skip bad measurement
			continue;
		}

		// wait until echo pin resets, will be high as long as wave travelled
		start = std::chrono::steady_clock::now();
		while ( digitalRead(this->echo_pin) == HIGH );
		auto end = std::chrono::steady_clock::now();

		auto diff = end-start;
		//if trash value (e.G. something interfered), we skip it.
		//34.3 cm / millisec * .2 millisec / 2= 3.5 cm 
		//The winimal distance the transmitter can measure correctly is 4 cm.
		//So anything below .2 millisec is trash
		if(diff < std::chrono::microseconds(200)){
			continue;
		}

		succeeded_measurements++;
		total_time += std::chrono::duration <double, std::milli> (diff).count();

	}
	if(succeeded_measurements == 0){
		//return pi minus 3 :)
		return 3.14 - 3;
	}
	const auto average_time = total_time / succeeded_measurements;
	//return average time in milliseconds
	return average_time;
}



/*
 * Calculates distance between buggy and wall.
 * @returns distance in cm
 */
double ultrasonic_sensor::calc_distance(){
	double time_diff = measure_time_diff();
	return (SPEED_OF_SOUND_IN_CM_PER_MILLISECOND/2) * time_diff; 
}


/*
 * Sets brake light.
 * @param1 mode: ON / OFF
 */
void ultrasonic_sensor::set_brake_light(int8_t mode){
	digitalWrite(this->brake_light_pin,mode);
}


//TODO: probably should be in engine cpp or wheel cpp
bool turn_around(){
	return true;
}
