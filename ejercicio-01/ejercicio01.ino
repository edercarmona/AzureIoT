#include "Arduino.h"
#include "Esp.h"

void setup(){
    pinMode(2,OUTPUT);
}

void loop(){
    digitalWrite(2,HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(100);
}