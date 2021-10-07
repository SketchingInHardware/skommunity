#include "MQTT_Handler.h"

//-------------------------------------   RANDOM MESSAGE

#define MILLIS_SUB_TAG "millis"
#define MILLIS_FULL_TOPIC ROOT_TOPIC SEPARATOR MILLIS_SUB_TAG


String millisMessage() {
  return String(millis());
}

MQTT_TimedMessageObject ellapsedMillisObject = {
  .lastTimeSent = 0,
  .interval = 1000,
  .getMessage = millisMessage,
  {.topic = MILLIS_FULL_TOPIC }
};

//-------------------------------------   STEPPING TONE
int incrementTone;
int nextToneStep;
int stepMagnitude = 1;
int incrementToneMax = 255;
int incrementToneMin = 0;

#define SCALE_SUB_TAG "byte_scale"
#define SCALE_FULL_TOPIC ROOT_TOPIC SEPARATOR SCALE_SUB_TAG

String scaleToneMessage() {
  
    //Serial.print(incrementTone);
    incrementTone = incrementTone + nextToneStep;
    
    if (incrementTone >= incrementToneMax) {
      nextToneStep = -1 * stepMagnitude;
    } else if (incrementTone <= incrementToneMin) {
      nextToneStep = 1 * stepMagnitude;
    }
    return String(incrementTone);
    
}

MQTT_TimedMessageObject scaleToneMessageObject = {
  .lastTimeSent = 0,
  .interval = 3000,
  .getMessage = scaleToneMessage,
   {.topic = SCALE_FULL_TOPIC }
};

//-------------------------------------   RANDOM MESSAGE

#define RANDOM_SUB_TAG "random"
#define RANDOM_FULL_TOPIC ROOT_TOPIC SEPARATOR RANDOM_SUB_TAG


String randomMessage() {
  return String(random(255));
}

MQTT_TimedMessageObject randomMessageObject = {
  .lastTimeSent = 0,
  .interval = 5000,
  .getMessage = randomMessage,
  {.topic = RANDOM_FULL_TOPIC }
};

//-------------------------------------   CONDITIONAL SENDER EG

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
