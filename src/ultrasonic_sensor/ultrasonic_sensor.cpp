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
    //set trigger pin to OUTPUT and LOW
    pinMode(this->trigger_pin,OUTPUT);
    digitalWrite(this->trigger_pin,LOW);
    //set ECHO pin to INPUT MODE
    pinMode(this->echo_pin,INPUT);
    //set brake_light_pin to OUTPUT and LOW
    pinMode(this->brake_light_pin,OUTPUT);
    digitalWrite(this->brake_light_pin,LOW);
}


/*
 * Measures the time between trigger and echo
 * @returns time difference in microseconds
 */
double ultrasonic_sensor::measure_time_diff(){

    double average_time = 0;
    double total_time = 0;
    int32_t measurements = 100;
    auto end = std::chrono::steady_clock::now();

    //needed for multiple measures in a row
    //to avoid infinite loop
    digitalWrite(this->trigger_pin,LOW);

    //measure x times and calc average result because of high variance in single results
    for (int i=0; i<measurements;++i){

        //debug
        //read value of echo
        //std::cout << "0 " << digitalRead(this->echo_pin) << "\t";

        //set trigger to high
        digitalWrite(this->trigger_pin,HIGH);

        //debug
        //std::cout << "1 " << digitalRead(this->trigger_pin) << "\t";

        auto start = std::chrono::steady_clock::now();
        //set high flank for at least 20 microseconds to make sure sensor will trigger
        auto delay = std::chrono::steady_clock::now();
        while(delay - start < std::chrono::microseconds(20)){
            delay = std::chrono::steady_clock::now();
        }
        //set trigger to low
        digitalWrite(this->trigger_pin,LOW);

        //debug
        //std::cout << "0 " << digitalRead(this->trigger_pin) << "\t";

        //poll echo pin until we receive response
        do {
            if(digitalRead(this->echo_pin)==HIGH){
                end = std::chrono::steady_clock::now();
                break;
            }
        }
        while (true);

        auto diff = end-start;
        total_time += std::chrono::duration <double, std::milli> (diff).count();

        //sleep so echo pin can reset at next measurement
        std::this_thread::sleep_for(std::chrono::microseconds(50));

        //debug code std::cout << "difference was " << std::chrono::duration <double, std::milli> (diff).count() << " ms\n";
    }
    average_time = total_time / measurements;
    std::cout << "total time: " << total_time << "ms average time: " << average_time << "ms measurements: " << measurements;

    //return average time in milliseconds
    return average_time;
}


/*
 * Calculates distance between buggy and wall.
 * @returns distance in cm
 */
double ultrasonic_sensor::calc_distance(){
	//debug
	double result = (SPEED_OF_SOUND_IN_CM_PER_MILLISECOND/2) * measure_time_diff(); 
	std::cout << " calculated distance in cm: " << result << "\n";
	return result;
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
