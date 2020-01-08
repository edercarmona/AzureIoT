#include "Arduino.h"
#include "Esp.h"
#include <ESP8266WiFi.h>
#include "WiFiClientSecure.h"
#include "WiFiUdp.h"
#include <Wire.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>
#include <AzureIoTHub.h>

//#include "config.h"

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.begin("ARRIS-08E2", "DEACB4B5B6447B55");

    Serial.print("Conectando");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Conectado, dirección IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {}
