#include <ESP8266WiFi.h>
#include "src/iotc/common/string_buffer.h"
#include "src/iotc/iotc.h"

#define WIFI_SSID "ARRIS-08E2"
#define WIFI_PASSWORD "DEACB4B5B6447B55"

const char *SCOPE_ID = "0ne000B39E2";
const char *DEVICE_ID = "6a8a4f72-ac96-45b3-bdf9-725d2598cfc6";
const char *DEVICE_KEY = "zI7j2NeAENDXhFbY556rd6SVaXzvXchXPGq6lQZmdrs=";

void on_event(IOTContext ctx, IOTCallbackInfo *callbackInfo);
#include "src/connection.h"

void on_event(IOTContext ctx, IOTCallbackInfo *callbackInfo)
{
    // ConnectionStatus
    if (strcmp(callbackInfo->eventName, "ConnectionStatus") == 0){
        LOG_VERBOSE("Is connected ? %s (%d)",
                    callbackInfo->statusCode == IOTC_CONNECTION_OK ? "YES" : "NO",
                    callbackInfo->statusCode);
        isConnected = callbackInfo->statusCode == IOTC_CONNECTION_OK;
        return;
    }
    AzureIOT::StringBuffer buffer;
    if (callbackInfo->payloadLength > 0){
        buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
    }
    LOG_VERBOSE("- [%s] event was received. Payload => %s\n",
                callbackInfo->eventName, buffer.getLength() ? *buffer : "EMPTY");

    if (strcmp(callbackInfo->eventName, "Command") == 0){
        LOG_VERBOSE("- Command name was => %s\r\n", callbackInfo->tag);
    }

    if (strcmp(callbackInfo->eventName, "SettingsUpdated") == 0){
        LOG_VERBOSE("- Setting name was => %s\r\n", callbackInfo->tag);
    }
}

void setup()
{
    Serial.begin(9600);
    connect_wifi(WIFI_SSID, WIFI_PASSWORD);
    connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);
    if (context != NULL){
        lastTick = 0; // set timer in the past to enable first telemetry a.s.a.p
    }
}

void loop(){
    if (isConnected){
        unsigned long ms = millis();
        if (ms - lastTick > 5000){ // send telemetry every 5 seconds
            char msg[128] = {0};
            int pos = 0, errorCode = 0;
            lastTick = ms;
            pos = snprintf(msg, sizeof(msg) - 1, "{\"parametro1\": %d, \"parametro2\":%d}", 10 + (rand() % 20), 1 + (rand() % 5));
            errorCode = iotc_send_telemetry(context, msg, pos);
            msg[pos] = 0;

            if (errorCode != 0){
                LOG_ERROR("Sending message has failed with error code %d", errorCode);
            }
        }
        iotc_do_work(context); // do background work for iotc
    }
    else{
        iotc_free_context(context);
        context = NULL;
        connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);
    }
}