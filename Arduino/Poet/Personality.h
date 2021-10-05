#include "MQTT_Handler.h"
#include "functionGenerator.h"

#define POET_SUB_TAG "thoughts_on_autumn"
#define POET_FULL_TOPIC ROOT_TOPIC SEPARATOR POET_SUB_TAG

funcgen sentiment;
funcgen awareness;
funcgen ambition;

const float sentQ = 36;
const float awareQ = 18;
const float ambQ = 30;

const float scope = 2* (sentQ + awareQ +  ambQ);
const float freqScale = 0.05;

int POET_currentValue;
int POET_targetValue = (sentQ + awareQ +  ambQ); //random(scope);

boolean POET_lastState;
boolean POET_currentState;


void setMusings() {
  sentiment.setFrequency(0.08 * freqScale);
  sentiment.setAmplitude(sentQ);
  sentiment.setPhase(0);
  sentiment.setYShift(sentQ);

  awareness.setFrequency(0.23 * freqScale);
  awareness.setAmplitude(awareQ);
  awareness.setPhase(0.1);
  awareness.setYShift(awareQ);

  ambition.setFrequency(0.34  * freqScale);
  ambition.setAmplitude(ambQ);
  ambition.setPhase(0);
  ambition.setYShift(ambQ);
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
  if (currentThought < numberOfThoughts-1) {
    currentThought ++;
  } else {
    currentThought = 0; 
  }
}

boolean poetState() {
  POET_lastState = POET_currentState; 
  
  if (POET_currentValue == POET_targetValue) {
    POET_currentState = true;
  } else {
    POET_currentState = false;
  }

  if ((POET_lastState != POET_currentState) && POET_currentState ) {
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

void muse() {
  float t = millis() * 0.001;
  float x = sentiment.sinus(t);
  float y = awareness.sinus(t);
  float z = ambition.sinus(t);
  float sum = x + y + z;

//    For Plotter
//    Serial.print("Target");
//    Serial.print("\t");
//    Serial.print("x");
//    Serial.print("\t");
//    Serial.print("y");
//    Serial.print("\t");
//    Serial.print("z");
//    Serial.print("\t");
//    Serial.println("Sum");
//    Serial.print(POET_targetValue);
//    Serial.print("\t");
//    Serial.print(x);
//    Serial.print("\t");
//    Serial.print(y);
//    Serial.print("\t");
//    Serial.print(z);
//    Serial.print("\t");
//    Serial.print(sum);
//    Serial.println();
  
  POET_currentValue = int(sum);
//
// if (poetState()) {
//  Serial.println(poetMessage());
// }
   testMQTTConnection();
   sendMQTTConditionalMessage(&Poem);
}
