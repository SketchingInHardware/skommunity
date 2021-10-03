#include "MQTT_Handler.h"
//-------------------------------------   RANDOM MESSAGE

#define RANDOM_SUB_TAG "random"
#define RANDOM_FULL_TOPIC ROOT_TOPIC SEPARATOR RANDOM_SUB_TAG


String randomMessage() {
  return String(random(255));
}

MQTT_Object randomMessageObject = {
  .lastTimeSent = 0,
  .interval = 5000,
  .getMessage = randomMessage,
  {.topic = RANDOM_FULL_TOPIC }
};
