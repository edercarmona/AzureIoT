#include <SimpleDHT.h>

#include "Arduino.h"
#include "Esp.h"

#define TRIGGER 5
#define ECHO 4
#define DHT_PIN 16
#define DHT_TYPE DHT11

SimpleDHT11 dht(DHT_PIN);

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
    byte temp = 0;
    byte hum = 0;
    dht.read(&temp, &hum, NULL);
    Serial.print((int)temp);
    Serial.print(" *C, ");
    Serial.print((int)hum);
    Serial.println(" H");
    delay(1500);
}