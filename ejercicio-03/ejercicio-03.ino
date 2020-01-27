#include <SimpleDHT.h>
#include "Arduino.h"
#include "Esp.h"

#define DHT_PIN 16
#define DHT_TYPE DHT11

SimpleDHT11 dht(DHT_PIN);

void setup(){
    Serial.begin(9600);
}

void loop(){
    byte temp = 0;
    byte hum = 0;
    dht.read(&temp, &hum, NULL);
    Serial.print((int)temp);
    Serial.print(" *C, ");
    Serial.print((int)hum);
    Serial.println(" H");
    delay(1500);
}