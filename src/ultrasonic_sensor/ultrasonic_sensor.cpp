#include <ultrasonic_sensor/ultrasonic_sensor.h>
#include <wiringPi.h>
#include <chrono>
#include <optional>
#include <stdio.h>
#include <iostream>
#include <util/time_util.h>
#include <util/strong_type_defs.h>


ultrasonic_sensor::ultrasonic_sensor(int8_t trigger_pin, int8_t echo_pin, int8_t brake_light_pin){    
    this->trigger_pin = trigger_pin;
    this->echo_pin = echo_pin;
    this->brake_light_pin = brake_light_pin;
    this->time_diff = std::nullopt;
    this->distance = std::nullopt;
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
    busy_wait(std::chrono::microseconds(50));
}


/*
 * Measures the time between trigger and echo
 * @returns time difference in microseconds if
 */
std::optional<double> ultrasonic_sensor::measure_time_diff(){

	double total_time = 0;
	const int32_t measurements = 30;
	int32_t succeeded_measurements = 0;

	//measure x times and calc average result because of high variance in single results
	for (int i=0; i<measurements;++i){

		//sleep so echo pin can reset at next measurement
		busy_wait(std::chrono::microseconds(1000));
		// send ultrasonic signal
		digitalWrite(this->trigger_pin, HIGH);
		busy_wait(std::chrono::microseconds(20));
		digitalWrite(this->trigger_pin, LOW);

        // wait until echo pin is high
	    busy_wait_until(
		        [&]() -> bool { return digitalRead(this->echo_pin) == HIGH; },
		        std::chrono::milliseconds(50));
		    //We received an anwer on the echo pin

		// wait until echo pin resets, will be high as long as wave travelled
		auto time_taken = busy_wait_until(
		        [&]() -> bool {return digitalRead(this->echo_pin) == LOW; }
		        );
		//if trash value (e.G. something interfered), we skip it.
		//34.3 cm / millisec * .2 millisec / 2= 3.5 cm 
		//The winimal distance the transmitter can measure correctly is 4 cm.
		//So anything below .2 millisec is trash
		if(time_taken < std::chrono::microseconds(200)){
			continue;
		}

		succeeded_measurements++;
		total_time += std::chrono::duration <double, std::milli> (time_taken).count();

	}

	if(succeeded_measurements == 0){
	    return std::nullopt;
	}
	const auto average_time = total_time / succeeded_measurements;
	//return average time in milliseconds
	return average_time;
}

/*
 * Calculates distance between buggy and wall if a wall is present
 * @returns distance in cm
 */
std::optional<cm> ultrasonic_sensor::calc_distance(){
	if(!(time_diff = measure_time_diff())) return std::nullopt;
	distance = (SPEED_OF_SOUND_IN_CM_PER_MILLISECOND/2) * time_diff.value();
	return cm{distance.value()};
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
