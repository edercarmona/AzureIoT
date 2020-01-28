#include <ArduinoJson.h>
#include <SimpleDHT.h>

SimpleDHT11 dht(DHT_PIN);

long duracion, distancia;

String readMessage( char *payload)
{
  byte temperatura = 0;
  byte humedad = 0; 
  
  dht.read(&temperatura,&humedad,NULL);

  
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIGGER, LOW);
  duracion = pulseIn(ECHO, HIGH);
  distancia = (duracion / 2) / 29.1;

  Serial.println("Duracion: " + duracion);
  Serial.println("Distancia: " + distancia);
  DynamicJsonDocument root(1024);
  root["device"] = DEVICE_ID;
  root["location"] = location;
  
  if (std::isnan(temperatura))
  {
    root["temperatura"] = NULL;
  }
  else
  {
    root["temperatura"] = temperatura;
  }
  
  if (std::isnan(humedad))
  {
    root["humedad"] = NULL;
  }
  else
  {
    root["humedad"] = humedad;
  }
  
  if (std::isnan(distancia))
  {
    root["distancia"] = NULL;
  }
  else
  {
    root["distancia"] =  distancia;
  } 
  serializeJson(root,payload,MESSAGE_MAX_LEN);
}


void parseTwinMessage(char *message){
    Serial.println("Parse Twin");
    DynamicJsonDocument root(1024);
    deserializeJson(root, message,DeserializationOption::NestingLimit(15));
    if (root.isNull())
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }
    if (!root["desired"]["interval"].isNull())
    {
        interval = root["desired"]["interval"];
    }
    else if(root.containsKey("interval"))
    {
       interval = root["interval"];
    }

     if (!root["desired"]["location"].isNull())
    {
        location = root["desired"]["location"].as<String>();
    }
    else if(root.containsKey("location"))
    {
       location = root["location"].as<String>();
    }
}
