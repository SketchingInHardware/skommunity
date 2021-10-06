
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

//#define ROOT_TOPIC ""  //GETS EVERYTHING on server
#define ROOT_TOPIC "skommunity"
#define SEPARATOR "/"
#define MULTIALL "#"
#define SUBSCRIBE_ALL ROOT_TOPIC SEPARATOR MULTIALL
char clientID[] = "Tester";



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
//--------------------------------------------------------- INCOMING




//-------------------------------------   subscrbing to a topic
const String suffix[3] = { "", "/+", "/#" };

void subscribeTo(String local_topic, int depth) {
  String fullTopic = local_topic + suffix[depth];
  Serial.println(fullTopic);
  mqttClient.subscribe(fullTopic);
}

void listenToEverytingOn(String local_topic) {
  subscribeTo(local_topic, 2);
}

//-------------------------------------   incoming message object

//// define a new type that is a function pointer
//struct MQTT_IncomingMessageObject {
//  long timeRecieved;
//  int messageSize;
//  boolean dup;
//  int QoS;
//  boolean retain;
//  //byte[] message;
//  char topic[];
//};
//
//
//typedef void (*message_processor)(void);
//

//struct Topic {
//  String fullTopic;
//  message_processor processMessage;

//}

void retrieveMessage(uint8_t *msg, int messageSize) {
  mqttClient.read(msg, messageSize);
}

//TODO: Message Reciever Processor
void messageProcessor() {
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

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  uint8_t msg[messageSize];
  retrieveMessage(msg, messageSize);

  String testString;

  Serial.print("\t");
  for (int b = 0; b <messageSize; b++) {
    Serial.print((char)msg[b]);
    testString = testString + ((char)msg[b]);
  }
  
  uint8_t msgstr[messageSize+1];
  memcpy(msgstr,msg,messageSize); 
  msgstr[messageSize+1] = 0; // null-termination so it acts like a C-string
  int testInt = String((char*)msgstr).toInt();

  char msgstr_c[messageSize+1];
  memcpy(msgstr_c,msg,messageSize); 
  msgstr_c[messageSize+1] = 0; // null-termination so it acts like a C-string
  int testInt2 = String(msgstr_c).toInt();
  

  
  Serial.print("\t");
  Serial.print((char*)msg);

//    Serial.print("\t");
//  Serial.print(int(msg));
  Serial.print("\t");
  Serial.print(testString);
    Serial.print("\t");
  Serial.print(testInt);
  Serial.println();

  Serial.println();
}

void mqttIncomingOperator(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();

  Serial.println();
}



//------------------------------------------------------------------
//--------------------------------------------- CONNECTING TO BROKER

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

//-------------------------------------   setMQTTDefaultListener()
void startMQTTDefaultListener() {
  listenToEverytingOn(ROOT_TOPIC);
  mqttClient.onMessage(onMqttMessage);
}

//-------------------------------------   testMQTTConnection()
void testMQTTConnection() {
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }
}

void touchMQTT() {
  testMQTTConnection();
  mqttClient.poll();
}



#endif
