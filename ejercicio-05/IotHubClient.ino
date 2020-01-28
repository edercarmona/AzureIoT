
static void sendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer){
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)buffer, strlen(buffer));
    if (messageHandle == NULL){
        Serial.println("No se puede crear el nuevo IoTHubMessage.");
    }
    else{
        MAP_HANDLE properties = IoTHubMessage_Properties(messageHandle);
        Serial.printf("Enviando Mensaje: %s.\r\n", buffer);
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendCallback,NULL) != IOTHUB_CLIENT_OK){
            Serial.println("Error al entregar el mensaje a IoTHubClient.");
        }
        else{
            messagePending = true;
            Serial.println("IoTHubClient mensaje aceptado.");
        }
        IoTHubMessage_Destroy(messageHandle);
    }
}

static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback){
     Serial.println("Send Callback");
    if (IOTHUB_CLIENT_CONFIRMATION_OK == result){
        Serial.println("Mensaje enviado a Azure IoT Hub");
        blinkLED();
    }
    else{
        Serial.println("Error al enviar el mensaje a Azure IoT Hub");
    }
    messagePending = false;
}


void start(){
    Serial.println("Inicio envio de  datos de temperatura y humedad..");
    messageSending = true;
}


void stop(){
    Serial.println("Alto envio de datos de temperatura y humedad.");
    messageSending = false;
}

IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback){
  Serial.println("Recibir mensaje de devolución de llamada");
  IOTHUBMESSAGE_DISPOSITION_RESULT result;
  const unsigned char *buffer;
  size_t size;
  if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK){
    Serial.println("No se puede IoTHubMessage_GetByteArray.");
    result = IOTHUBMESSAGE_REJECTED;
  }
  else{
    char *temp = (char *)malloc(size + 1);
    if (temp == NULL){
      return IOTHUBMESSAGE_ABANDONED;
    }
    strncpy(temp, (const char *)buffer, size);
    temp[size] = '\0';
    Serial.printf("Recibiendo mensaje C2D : %s.\r\n", temp);
    free(temp);
    blinkLED();
  }
  return IOTHUBMESSAGE_ACCEPTED;
}



int deviceMethodCallback(const char *methodName,const unsigned char *payload,size_t size,unsigned char **response,size_t *response_size,void *userContextCallback)
{
    Serial.printf("Try to invoke method %s.\r\n", methodName);
    const char *responseMessage = "Success";
    int result = 200;
    if (strcmp(methodName, "start") == 0){
        start();
    }
    else if (strcmp(methodName, "stop") == 0){
        stop();
    }
    else{
        Serial.printf("No method %s found.\r\n", methodName);
        responseMessage = "Failed, method not found";
        result = 404;
    }
    *response_size = strlen(responseMessage);
    *response = (unsigned char *)malloc(*response_size);
    strncpy((char *)(*response), responseMessage, *response_size);

    return result;
 }


void twinCallback(DEVICE_TWIN_UPDATE_STATE updateState,const unsigned char *payLoad,size_t size,void *userContextCallback)
{
 Serial.println("Twin Callback");
    char *temp = (char *)malloc(size + 1);
    for (int i = 0; i < size; i++)
    {
        temp[i] = (char)(payLoad[i]);
    }
    temp[size] = '\0';
    parseTwinMessage(temp);
    free(temp);
}
