#include "MQTT_Handler.h"




//-------------------------------------   Ellapsed Millia

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

//-------------------------------------   RANDOM CHANNEL
//-------------------------------------   REGULAR RANDOM MESSAGE

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


//-------------------------------- RANDOM MESSAGE IRREGULAR FREQ.

#define IRREGRAND_SUB_TAG "irregularRandom"
#define IRREGRAND_FULL_TOPIC ROOT_TOPIC SEPARATOR IRREGRAND_SUB_TAG

boolean IR_lastState;
boolean IR_currentState;

boolean irregularRandomState() {
  IR_lastState = IR_currentState; 
  unsigned long theSeconds = millis() * 0.0001;

  if (!(theSeconds % (random(255))) && !(theSeconds % random(20))) {
    IR_currentState = true;
  } else {
    IR_currentState = false;
  }

  //Not infact sending the current state, sending if the current
  //state just became true. 
  if ((IR_lastState != IR_currentState) && IR_currentState ) {
    return true;
  } else {
    return false;
  }
}

MQTT_ConditionalMessageObject irregularRandom = {
  .lastTimeSent = 0,
  .getState = irregularRandomState,
  .getMessage = randomMessage,
  {.topic = IRREGRAND_FULL_TOPIC }
};
