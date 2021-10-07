/*
 * Waver.ino - Sketching in Hardware 21 MQTT receiver
 * 5 Oct 2021 - @todbot
 *
 * It's optimistic, it's holding on. It waves to you 
 * when you talk to it!
 * 
 * This sketch uses https://shiftr.io/try as the MQTT broker.
 *
 * The circuit: 
 * - Arduino Nano 33 IoT board 
 * - Servo on pin 9
 *
 * The arduino_secrets.h file:
 *  #define SECRET_SSID ""    // network name
 *  #define SECRET_PASS ""    // network password
 *  #define SECRET_MQTT_USER "public" // broker username
 *  #define SECRET_MQTT_PASS "public" // broker password
 *
 * Based off " MQTT Client with a button and an LED" 
 *    created 11 June 2020
 *    updated 12 Apr 2021
 *    by Tom Igoe
*/

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"
#include <Servo.h>

Servo myservo;

const int servoPin = 9;
const int servoMin = 120;
const int servoMax = 180;

// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;
char topic[] = "skommunity/feeds/dinger";
char clientID[] = "SkommunityClient";

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // wait for serial monitor to open:
  //while (!Serial);

  myservo.attach(servoPin);
  myservo.write(servoMin);

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
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  // try to connect to the MQTT broker once you're connected to WiFi:
  while (!connectToBroker()) {
    Serial.println("attempting to connect to broker");
    delay(1000);
  }
  Serial.println("connected to broker");
}

void dingServo(uint8_t num) {
  num = constrain(num, 3, 6);
  Serial.print("dinging servo times ");
  Serial.println(num);
  for ( int i = 0; i < num; i++ ) {
    myservo.write(servoMin);
    delay(200);
    myservo.write(servoMax);
    delay(200);
    myservo.write(servoMin);
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
  // once you're connected, you can proceed:
  mqttClient.subscribe(topic);
  // return that you're connected:
  return true;
}

void loop() {
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }

  // if a message comes in, read it:
  if (mqttClient.parseMessage() > 0) {
    Serial.print("Got a message on topic: ");
    Serial.println(mqttClient.messageTopic());
    
    // read the message:
    while (mqttClient.available()) {
      // convert numeric string to an int:
      int message = mqttClient.parseInt();
      Serial.println(message);
       dingServo( message / 10 );
    }
  }

}
