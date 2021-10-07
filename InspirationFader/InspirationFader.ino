/*
 * InspirationFader.ino - Sketching in Hardware 21 MQTT receiver
 * 5 Oct 2021 - @todbot
 *
 * Receives great ideas from the Net! Inspired to do something! 
 * And then it fades...
 *
 * This sketch uses https://shiftr.io/try as the MQTT broker.
 *
 * The circuit: 
 * - Arduino Nano 33 IoT board 
 * - Strand of 16 Neopixels on pin 21
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

#include <FastLED.h>

#define LED_PIN     21
#define NUM_LEDS    16
CRGB leds[NUM_LEDS];

// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;
char topic[] = "skommunity/feeds/dinger";
char clientID[] = "SkommunityClient";


void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  FastLED.show();
  FastLED.delay(20);

  dingNeopixels(255, 100);

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

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MOTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  mqttClient.subscribe(topic);

  return true;
}

// unused
void sendDingerMessage(int val) {
  Serial.print("sending dinger message:");
  Serial.println(val);
  // start a new message on the topic:
  mqttClient.beginMessage(topic);
  // add a random number as a numeric string (print(), not write()):
  mqttClient.print(val);
  // send the message:
  mqttClient.endMessage();
}

void dingNeopixels(uint8_t num, uint8_t hue) {
  //void dingNeopixels(uint8_t num, const CRGB& c) {
  //fill_solid(leds, NUM_LEDS, CHSV( hue, 255, 255));
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] += CHSV(hue, 255,255);
  }
  FastLED.show();
}

void updateNeopixelsOld() {
  fadeToBlackBy( leds, NUM_LEDS, 1);
  FastLED.show();
}

void updateNeopixels() {
  int i = random(0, NUM_LEDS);
  leds[i].fadeToBlackBy( random(0,10) );
  FastLED.show();
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
      int message = mqttClient.parseInt(); // convert numeric string to an int:
      Serial.println(message);
      // if the message is greater than 0, set the LED to full intensity:
      if (message > 0) {
        dingNeopixels(255, message);
      }
    }
  }

  updateNeopixels();

}


