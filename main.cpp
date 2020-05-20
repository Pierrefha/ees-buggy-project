#include <wiringPi.h>
#include <signal.h>
#include <iostream>

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;
    // Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
    exit(signum);
}

int main (void)
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);
    wiringPiSetup () ;
    pinMode (0, OUTPUT) ;
    for (;;)
    {
        digitalWrite (0, HIGH) ; delay (500) ;
        digitalWrite (0,  LOW) ; delay (500) ;
    }
    return 0 ;
}
