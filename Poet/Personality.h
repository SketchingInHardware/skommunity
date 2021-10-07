#include "MQTT_Handler.h"
#include "functionGenerator.h"

#define POET_SUB_TAG "thoughts_on_autumn"
#define POET_FULL_TOPIC ROOT_TOPIC SEPARATOR POET_SUB_TAG

const bool selfTalk = false;

funcgen sentiment;
funcgen awareness;
funcgen ambition;

const float rangeOfSentiment = 36;
const float rangeOfAwareness = 18;
const float rangeOfAmbition = 30;

const float freqScale = 0.05;

int POET_currentMood;
int POET_inspiredMood = (rangeOfSentiment + rangeOfAwareness +  rangeOfAmbition);

boolean POET_wasInspired;
boolean POET_isInspired;


void setMusings() {
  sentiment.setFrequency(0.08 * freqScale);
  sentiment.setAmplitude(rangeOfSentiment);
  sentiment.setPhase(0);
  sentiment.setYShift(rangeOfSentiment);

  awareness.setFrequency(0.23 * freqScale);
  awareness.setAmplitude(rangeOfAwareness);
  awareness.setPhase(0.1);
  awareness.setYShift(rangeOfAwareness);

  ambition.setFrequency(0.34  * freqScale);
  ambition.setAmplitude(rangeOfAmbition);
  ambition.setPhase(0);
  ambition.setYShift(rangeOfAmbition);
}

const int numberOfThoughts = 20;
String thoughts[numberOfThoughts] = {
  "thin the veil",
  "transition",
  "joy in the mourning",
  "getting into bed",
  "equitorial insomniacs",
  "pumpkin shell",
  "pumpkin shield",
  "pumpkin squash",
  "given foods",
  "stolen foods",
  "stolen lands",
  "destruction before restoration",
  "withdrawl",
  "sleep, or stillness like a cat",
  "Springs Leap",
  "Leaves as good bye party costume",
  "Uncertainty",
  "anticipation of dreams",
  "Hope",
  " ",
};
int currentThought = 0;

void setThought() {
  if (currentThought < numberOfThoughts - 1) {
    currentThought ++;
  } else {
    currentThought = 0;
  }
}

boolean poetState() {
  POET_wasInspired = POET_isInspired;

  if (POET_currentMood == POET_inspiredMood) {
    POET_isInspired = true;
  } else {
    POET_isInspired = false;
  }

  //If poet is NEWLY inspred then it will send. 
  if ((POET_wasInspired != POET_isInspired) && POET_isInspired ) {
    setThought();
    return true;
  } else {
    return false;
  }
}

String poetMessage() {
  return thoughts[currentThought];
}

MQTT_ConditionalMessageObject Poem = {
  .lastTimeSent = 0,
  .getState = poetState,
  .getMessage = poetMessage,
  {.topic = POET_FULL_TOPIC }
};

void emote(int x, int y, int z, int sum) {
    if (selfTalk) {
    //For Plotter
    Serial.print("Target");
    Serial.print("\t");
    Serial.print("x");
    Serial.print("\t");
    Serial.print("y");
    Serial.print("\t");
    Serial.print("z");
    Serial.print("\t");
    Serial.println("Sum");
    Serial.print(POET_inspiredMood);
    Serial.print("\t");
    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.print("\t");
    Serial.print(z);
    Serial.print("\t");
    Serial.print(sum);
    Serial.println();

    if (poetState()) {
      Serial.println(poetMessage());
    }
  }

  testMQTTConnection();
  updateMQTTConditionalSender(&Poem);
}

void muse() {
  float t = millis() * 0.001;
  float x = sentiment.sinus(t);
  float y = awareness.sinus(t);
  float z = ambition.sinus(t);
  float sum = x + y + z;
  
  POET_currentMood = int(sum);
  
  emote(x, y, z, sum);
}
