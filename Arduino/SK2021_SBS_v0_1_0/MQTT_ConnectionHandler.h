
#include <ArduinoMqttClient.h>
#include "Network_Connection.h"
#include "real_secrets.h"
//#include "arduino_secrets.h"

#include "MQTT_MessagePackagerDefs.h"

MqttClient mqttClient(network_connection);

// details for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;


char subscription_topic[] = "skommunity";
char clientID[] = "Herald";


//--------------------------------------------- MQTT_MessagePackager

void sendMQTTObject(MQTT_Object* mqtto) {
  if (millis() - mqtto->lastTimeSent > mqtto->interval) {
    // start a new message on the objects topic:
    mqttClient.beginMessage(mqtto->topic);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(mqtto->getMessage());
    mqttClient.endMessage();
    mqtto->lastTimeSent = millis();
  }
}


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

void testMQTTConnection() {
    // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }
}

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
