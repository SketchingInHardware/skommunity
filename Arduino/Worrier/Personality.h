#include "MQTT_Handler.h"


#define WORRY_SUB_TAG "ouch"
#define WORRY_FULL_TOPIC ROOT_TOPIC SEPARATOR WORRY_SUB_TAG

#define WORRY_SCOPE 1000000

boolean W_lastState;
boolean W_currentState;

int W_currentValue= random(WORRY_SCOPE);
int W_targetValue = random(WORRY_SCOPE);

boolean worryState() {
  W_lastState = W_currentState; 
  
  if (W_currentValue == W_targetValue) {
    W_currentState = true;
  } else {
    W_currentState = false;
  }

  if ((W_lastState != W_currentState) && W_currentState ) {
    return true;
  } else {
    return false;
  }
}

String worryMessage() {
  return "that hurt.";
  //return String(W_currentValue);
}

MQTT_ConditionalMessageObject Worry = {
  .lastTimeSent = 0,
  .getState = worryState,
  .getMessage = worryMessage,
  {.topic = WORRY_FULL_TOPIC }
};

int worry() {
  W_currentValue = random(WORRY_SCOPE);
//  Serial.print(W_currentValue);
//  Serial.print("\t");
//  Serial.println(W_targetValue);
  testMQTTConnection();
  sendMQTTConditionalMessage(&Worry);
}
