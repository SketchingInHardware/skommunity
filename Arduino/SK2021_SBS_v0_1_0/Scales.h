#include "MQTT_Handler.h"
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
