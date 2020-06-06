#include "ultrasonic_sensor/ultrasonic_sensor.h" 
#include <wiringPi.h>
#include <chrono>
#include <thread>
#include <stdio.h>

ultrasonic_sensor::ultrasonic_sensor(int8_t trigger_pin, int8_t echo_pin){    
    this.trigger_pin = trigger_pin;
    this.echo_pin = echo_pin;
}

ultrasonic_sensor::init(){
    //set trigger pin to OUTPUT and LOW
    pinMode(this.trigger_pin,OUTPUT);
    digitalWrite(this.trigger_pin,LOW);
    //set ECHO pin to INPUT MODE
    pinMode(this.echo_pin,OUTPUT);
}


int32_t ultrasonic_sensor::get_distance(){
    double speed_of_sound = SPEED_OF_SOUND_IN_CM_PER_SECOND;

    //set trigger to high for one flank
    digitalWrite(this.trigger_pin,HIGH);
    auto start = std::chrono::system_clock::now();
    //sleep for one millisecond
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_IN_MILLISECONDS0));
    //set trigger to low
    digitalWrite(this.trigger_pin,LOW);

    //poll echo pin until we receive response
    while(TRUE){
        if(digitalRead(this.echo_pin)==HIGH){
            auto end = std::chrono::system_clock::now();
            break;
        }
    }
    std::chrono::duration<double> time_diff = end-start;
    int32_t distance_in_cm  = (speed_of_sound/2) * time_diff.count(); 
    //DEBUG MESSAGE DISPLAY THE DISTANCE
    std::cout << "current distance: "+distance_in_cm;
    return distance_in_cm;
}

//TODO continue dev
