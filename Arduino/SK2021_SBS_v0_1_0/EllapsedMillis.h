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
