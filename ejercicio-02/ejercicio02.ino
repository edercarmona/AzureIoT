#include "Arduino.h"
#include "Esp.h"

#define TRIGGER 5
#define ECHO 4


void setup()
{

    Serial.begin(9600);
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
}

void loop()
{

    long duration, distance;
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIGGER, LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = (duration / 2) / 29.1;

    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(1000);
}