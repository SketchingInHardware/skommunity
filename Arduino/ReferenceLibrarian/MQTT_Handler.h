
#ifndef MQTTDEFS
#define MQTTDEFS

#include <ArduinoMqttClient.h>
#include "Network_Connection.h"
#include "real_secrets.h"
//#include "arduino_secrets.h"


MqttClient mqttClient(network_connection);

// details for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;

#define ROOT_TOPIC "skommunity/reference"
#define SEPARATOR "/"
char subscription_topic[] = ROOT_TOPIC;
char clientID[] = "Librarian";



//------------------------------------------------------------------ 
//--------------------------------------------------------- OUTGOING

// define a new type that is a function pointer
typedef String (*messsage_function)(void);

struct MQTT_TimedMessageObject {
  long lastTimeSent;
  int interval;
  messsage_function getMessage;
  char topic[];
};

// define a new type that is a function pointer
typedef boolean (*conditional_function)(void);

struct MQTT_ConditionalMessageObject {
  long lastTimeSent;
  conditional_function getState;
  messsage_function getMessage;
  char topic[];
};

//TODO: Updateable topic for run time updates. The below function does print 
//but does not return an extant reference (myConcat gets released upon termination). 
//Convert topic references from char[] -> Arduio String? 
//For reference the char[] problem:
//https://forum.arduino.cc/t/can-a-function-return-a-char-array/63405/5
//https://forum.arduino.cc/t/how-to-return-a-char-from-a-function/184239/9
//char* topicAssembler(char* topic, char* subtag) {
//    char myConcatenation[strlen(topic) + 1 + strlen(subtag)];
//    sprintf(myConcatenation,"%s/%s",topic,subtag);
//    Serial.println(myConcatenation); //Prints...
//    return myConcatenation; //But does not return
//}

//-------------------------------------   sendMQTTTimedObject(MQTT_TimedMessageObject* mqtto)
void sendMQTTTimedObject(MQTT_TimedMessageObject* mqtto) {
  if (millis() - mqtto->lastTimeSent > mqtto->interval) {
    // start a new message on the objects topic:
    mqttClient.beginMessage(mqtto->topic);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(mqtto->getMessage());
    mqttClient.endMessage();
    mqtto->lastTimeSent = millis();
  }
}

//-------------------------------------   sendMQTTConditionalMessage(MQTT_ConditionalMessageObject* mqtto)
void sendMQTTConditionalMessage(MQTT_ConditionalMessageObject* mqtto) {
  if (mqtto->getState()) {
     // start a new message on the objects topic:
    mqttClient.beginMessage(mqtto->topic);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(mqtto->getMessage());
    mqttClient.endMessage();
    mqtto->lastTimeSent = millis();
  } else {
    //Serial.println("Nothing to send");
  }
}

//------------------------------------------------------------------ 
//--------------------------------------------- CONNECTION TO BROKER

//-------------------------------------   connectToBroker()
boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MOTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you can proceed:
  mqttClient.subscribe(subscription_topic);
  // return that you're connected:
  return true;
}


//-------------------------------------   connectToMQTT()
//TODO:Limit attempts, fail gracefully
void connectToMQTT() {
    // set the credentials for the MQTT client:
  mqttClient.setId(clientID);
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  // try to connect to the MQTT broker once you're connected to WiFi:
  while (!connectToBroker()) {
    Serial.println("attempting to connect to broker");
    delay(1000);
  }
  Serial.println("connected to broker");
}

//-------------------------------------   testMQTTConnection()
void testMQTTConnection() {
    // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }
}

//------------------------------------------------------------------ 
//--------------------------------------------------------- INCOMING

//----------------------------------- -- Place Holder Message Reader
//TODO: Message Reciever Processor
void fetchMQTTMessage() {
    // if a message comes in, read it:
  if (mqttClient.parseMessage() > 0) {
    Serial.print("Got a message on topic: ");
    Serial.println(mqttClient.messageTopic());
    // read the message:
    while (mqttClient.available()) {
      // convert numeric string to an int:
      int message = mqttClient.parseInt();
      Serial.println(message);
      // if the message is greater than 0, set the LED to full intensity:
//      if (message > 0) {
//        intensity = 255;
//      }
    }
  }
}

//------------------------------------------ End Place Holder Message Reader

#endif
