#include "MQTT_Handler.h"
//-------------------------------------   RANDOM MESSAGE

#define CONDSEND_SUB_TAG "navelGazer"
#define CONDSEND_FULL_TOPIC ROOT_TOPIC SEPARATOR CONDSEND_SUB_TAG

boolean CS_lastState;
boolean CS_currentState;

boolean conditionalSenderState() {
  CS_lastState = CS_currentState; 
  unsigned long theSeconds = millis()/1000;

  //The condition if the current second is divisible by 17
  //if sent this value raw it would send multiple times as this 
  //function is called many times a second when in main loop.
  //This could be a harware read, etc. 
  if (!(theSeconds % 17)) {
    CS_currentState = true;
  } else {
    CS_currentState = false;
  }

  //Not infact sending the current state, sending if the current
  //state just became true. 
  if ((CS_lastState != CS_currentState) && CS_currentState ) {
    return true;
  } else {
    return false;
  }
}

String conditionalSenderMessage() {
  return String(random(255));
}

MQTT_ConditionalMessageObject conditionalSender = {
  .lastTimeSent = 0,
  .getState = conditionalSenderState,
  .getMessage = conditionalSenderMessage,
  {.topic = CONDSEND_FULL_TOPIC }
};
