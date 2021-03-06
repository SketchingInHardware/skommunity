/*
2021 October 03
Message Seeder Code v 0.0.1
*/

#include "MQTT_Handler.h"
#include "Network_Connection.h"
#include "HardwareHandler.h"

//Message Generators
#include "SenderExamples.h"


//----------------------------------------------------   SETUP
void setup() {
  // pin settings
  pinMode(pushButton, INPUT);
  for (int thisPin = cillium4; thisPin <= cillium1; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }

  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial);

  connectToWiFi();
  connectToMQTT();

  //startMQTTDefaultListener();
  startMQTTListener("skommunity", onMqttMessage);
  
}


//----------------------------------------------------   LOOP
void loop() {

//  //-------------------------------------  Read Binary Hardware
//  checkButton();
//
//  //-------------------------------------  Read Analog Hardware
//  updateFolicleData();
//
//  //-------------------------------------  Update Local World
//
//  updateCiliaData();
//
//  if (buttonState == true) {
//    printFolicleData();
//    Serial.println("------");
//    printFolicleAverage();
//    Serial.println(" ");
//    printCiliaData();
//    Serial.println(" ");
//  }
//
//  updateCilia();


  //-------------------------------------  Update Remote World
  // if not connected to the broker, try to connect:
    touchMQTT();

    updateMQTTTimedSender(&randomMessageObject);
    updateMQTTTimedSender(&scaleToneMessageObject);
    updateMQTTTimedSender(&ellapsedMillisObject);
    sendMQTTConditionalMessage(&conditionalSender);
    

//  updateMQTTTimedSender(&hungerMessageObject);
//  updateMQTTTimedSender(&strokeMessageObject);

}

//----------------------------------------------------   END LOOP
