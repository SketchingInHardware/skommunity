
#include "MQTT_Handler.h"



#define MAX_BANDWIDTH 10000 //cannot be bigger than int
#define MAX_DELAY 5000


int currentBandwidth = MAX_BANDWIDTH;
int currentSaturation = 0;
const float fineThreshold = 0.3;

#define STEPS 100
#define OUTPUT_MAX 100
byte exhaustionCurve[STEPS];

void setExhaustionTable() {
  float R = (STEPS * log10(2)) / (log10(OUTPUT_MAX));
  // set the range of values:
  // iterate over the array and calculate the right value for it:
  for (int e = 0; e <= STEPS; e++) {
    // calculate the light level here:
    byte level = pow (2, (e / R)) - 1;
    exhaustionCurve[e] = level;
  }
  Serial.print("Exhaustion Curve: ");
  for (int i = 0; i < STEPS; i++) {
    Serial.print(exhaustionCurve[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void rest(float howFull) {
  int multiplier = exhaustionCurve[int(howFull*100)];
  int amount = multiplier * (MAX_DELAY/100);
  Serial.print("naptime:");
  Serial.print(amount);
  Serial.print("\t");
  delay(amount);
}

void wellFuckMe() {
  NVIC_SystemReset();
}

void checkInWithTheBody() {
  currentBandwidth = map(getFortitude(), 0, 1023, MAX_BANDWIDTH/2, MAX_BANDWIDTH);
}


void howamIfeeling() {
  checkInWithTheBody(); 
  if (currentBandwidth < currentSaturation) {
    wellFuckMe();
  }
  Serial.print("currentBandwidth:");
  Serial.print(currentBandwidth);
    Serial.print("\t");
  float percentFull = float(currentSaturation) / float(currentBandwidth);
  Serial.print("precentFul:");
  Serial.print(percentFull);
  Serial.print("\t");
  rest(percentFull); 
  //  if (percentFull < fineThreshold) {
  //    Serial.print("This is fine.");
  //    Serial.print("\t");
  //    //rest(currentSaturation);
  //  } else {
  //    Serial.print("hunh");
  //    Serial.print("\t");
  //    rest(percentFull);
  //  }
}


void inhale(int gulpSize) {
  currentSaturation += gulpSize;
  Serial.println(currentSaturation);
}

void chooseFeed() {
  //This intializer subscribes to topic and all its sub topics
  startMQTTListener(ROOT_TOPIC, inhale);
  startMQTTListener("", inhale);
}



void doomScroll(){
  //by putting this here, may nap multiple times if there is 
  //no new messages. Could put a message check, but maybe that
  //is the right behavior.
  howamIfeeling();
  touchMQTT();
}
