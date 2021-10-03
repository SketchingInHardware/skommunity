#include "MQTT_MessagePackagerDefs.h"
//-------------------------------------   RANDOM MESSAGE
String randomMessage() {
  return String(random(255));
}

MQTT_Object randomMessageObject = {
  .lastTimeSent = 0,
  .interval = 5000,
  .getMessage = randomMessage,
  {.tag = "skommunity/random"}
};
