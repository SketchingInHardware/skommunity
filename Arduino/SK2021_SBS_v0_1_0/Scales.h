#include "MQTT_MessagePackagerDefs.h"
//-------------------------------------   STEPPING TONE
int incrementTone;
int nextToneStep;
int incrementToneMax = 255;
int incrementToneMin = 0;

String scaleToneMessage() {
  
    //Serial.print(incrementTone);
    incrementTone = incrementTone + nextToneStep;
    
    if (incrementTone >= incrementToneMax) {
      nextToneStep = -1;
    } else if (incrementTone <= incrementToneMin) {
      nextToneStep = 1;
    }
    return String(incrementTone);
    
}

MQTT_Object scaleToneMessageObject = {
  .lastTimeSent = 0,
  .interval = 3000,
  .getMessage = scaleToneMessage,
   {.tag = "skommunity/byte_scale"}
};
