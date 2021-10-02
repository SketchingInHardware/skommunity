/*
  MQTT Client sender

  This sketch demonstrates an MQTT client that connects to a broker, subsrcibes to a topic,
  and both listens for messages on that topic and sends messages to it, a random number between 0 and 15.
  When the client receives a message, it parses it, and if the number matches the client's
  number (myNumber, chosen arbitrarily), it sets an LED to full. When nothing is happening,
  if the LED is not off, it's faded down one point every time through the loop.

  This sketch uses https://shiftr.io/try as the MQTT broker.

  the arduino_secrets.h file:
  #define SECRET_SSID ""    // network name
  #define SECRET_PASS ""    // network password
  #define SECRET_MQTT_USER "" // broker username
  #define SECRET_MQTT_PASS "" // broker password

  created 11 June 2020
  by Tom Igoe
*/

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"
#include "MidiHandler.h"

// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "broker.shiftr.io";
int port = 1883;
char topic[] = "try/table_7/carlyn";
char clientID[] = "voice01";

char topicA[] = "try/table_7/random";
// last time the client sent a message, in ms:
long lastTimeSentA = 0;
// message sending interval:
int intervalA = 10000;

char topicB[] = "try/table_7/scales";
// last time the client sent a message, in ms:
long lastTimeSentB = 0;
// message sending interval:
int intervalB = 3000;

char topicC[] = "try/table_7/poem";
// last time the client sent a message, in ms:
long lastTimeSentC = 0;
// message sending interval:
int intervalC = 11000;

const int lineLengthOfPoem = 4;
String lines[lineLengthOfPoem] = { "The lamp once out", "Cool stars enter", "The window frame.", "- Natsume Soseki" };
int lineInterval[lineLengthOfPoem] = { 10000, 5000, 7000, 50000 };
int whichLine = 0;


int incrementTone = 0;
int incrementToneMax = 127;
int incrementToneMin = 0;
int nextToneStep = 1;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial);

  // initialize WiFi, if not connected:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  // print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());

  // set the credentials for the MQTT client:
  mqttClient.setId(clientID);
  // login to the broker with a username and password:
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  // try to connect to the MQTT broker once you're connected to WiFi:
  while (!connectToBroker()) {
    Serial.println("attempting to connect to broker");
    delay(1000);
  }
  Serial.println("connected to broker");
}

void loop() {
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }

  // once every interval, send a message:
  if (millis() - lastTimeSentA > intervalA) {
    // start a new message on the topic:
    mqttClient.beginMessage(topicA);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(random(127));
    //readVoice();
    //Serial.println(myVoice);
    // send the message:
    mqttClient.endMessage();
    lastTimeSentA = millis();
  }

    if (millis() - lastTimeSentB > intervalB) {
    // start a new message on the topic:
    mqttClient.beginMessage(topicB);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(incrementTone);
    //Serial.print(incrementTone);
    incrementTone = incrementTone + nextToneStep;
    if (incrementTone >= incrementToneMax) {
      nextToneStep = -1;
    } else if (incrementTone <= incrementToneMin) {
      nextToneStep = 1;
    }
    // send the message:
    mqttClient.endMessage();
    lastTimeSentB = millis();
  }


      if (millis() - lastTimeSentC > lineInterval[whichLine]) {
    // start a new message on the topic:
    mqttClient.beginMessage(topicC);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(lines[whichLine]);
    Serial.println(lines[whichLine]);
     whichLine = whichLine + 1;
    if (whichLine > (lineLengthOfPoem -1) ) {
      whichLine = 0;
    }
    
    // send the message:
    mqttClient.endMessage();
    lastTimeSentC = millis();
  }

}

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MOTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you
  // return that you're connected:
  return true;
}
