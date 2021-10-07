#include "MQTT_Handler.h"


#define WORRY_SUB_TAG "ouch"
#define WORRY_FULL_TOPIC ROOT_TOPIC SEPARATOR WORRY_SUB_TAG

#define WORRY_SCOPE 1000000

const bool selfTalk = false;

boolean W_wasAggitated;
boolean W_isAggitated;

int W_currentWorry = random(WORRY_SCOPE);
const int W_bigWorry = random(WORRY_SCOPE);

boolean worryState() {
  W_wasAggitated = W_isAggitated;

  if (W_currentWorry == W_bigWorry) {
    W_isAggitated = true;
  } else {
    W_isAggitated = false;
  }

  if ((W_wasAggitated != W_isAggitated) && W_isAggitated ) {
    return true;
  } else {
    return false;
  }
}

String worryMessage() {
  return "that hurt.";
}

MQTT_ConditionalMessageObject Worry = {
  .lastTimeSent = 0,
  .getState = worryState,
  .getMessage = worryMessage,
  {.topic = WORRY_FULL_TOPIC }
};

void emote() {
  if (selfTalk) {
    Serial.print(W_currentWorry);
    Serial.print("\t");
    Serial.println(W_bigWorry);
  }
  testMQTTConnection();
  updateMQTTConditionalSender(&Worry);
}

int setConcern() {
  return random(WORRY_SCOPE);
}

void worry() {
  W_currentWorry = setConcern();
  emote();
}
