
#include "MQTT_Handler.h"

#define MAX_BANDWIDTH 10000 //cannot be bigger than int
#define MAX_DELAY 5000

const bool selfTalk = true;

int currentBandwidth = MAX_BANDWIDTH;
int currentSaturation = 0;

#define STEPS 100
#define OUTPUT_MAX 100
byte exhaustionCurve[STEPS];

//https://tigoe.github.io/LightProjects/fading.html
//https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
void setExhaustionTable() {
  float R = (STEPS * log10(2)) / (log10(OUTPUT_MAX));
  // set the range of values:
  // iterate over the array and calculate the right value for it:
  for (int e = 0; e <= STEPS; e++) {
    // calculate the light level here:
    byte level = pow (2, (e / R)) - 1;
    exhaustionCurve[e] = level;
  }

  if (selfTalk) { Serial.print("Exhaustion Curve: ");}
  for (int i = 0; i < STEPS; i++) {
    if (selfTalk) {
      Serial.print(exhaustionCurve[i]);
      Serial.print(" ");
    }

  }
  if (selfTalk) { Serial.println(); }
}

void rest(float feels) {
  int extenuations = exhaustionCurve[int(feels * 100)];
  int napTime = extenuations * (MAX_DELAY / 100);
  if (selfTalk) {
    Serial.print("naptime:");
    Serial.print(napTime);
    Serial.print("\t");
  }
  delay(napTime);
}

void wellFuckMe() {
  //For SAMD boards like the Nano 33 IoT
  NVIC_SystemReset();
}

void checkInWithTheBody() {
  currentBandwidth = map(getFortitude(), 0, 1023, MAX_BANDWIDTH / 2, MAX_BANDWIDTH);
}


float howamIfeeling() {
  checkInWithTheBody();
  
  if (currentBandwidth < currentSaturation) {
    wellFuckMe();
  }

  float percentBrainFry = float(currentSaturation) / float(currentBandwidth);
  if (selfTalk) {
    Serial.print("currentBandwidth:");
    Serial.print(currentBandwidth);
    Serial.print("\t");

    Serial.print("brainFry:");
    Serial.print(percentBrainFry);
    Serial.print("\t");
  }
  return percentBrainFry;
}


void inhale(int gulpSize) {
  currentSaturation += gulpSize;
  if (selfTalk) { 
    Serial.println();
    Serial.print("updatedSaturation:");
    Serial.println(currentSaturation); 
  }
}

void chooseFeed() {
  //This intializer subscribes to topic and all its sub topics
  startMQTTListener(ROOT_TOPIC, inhale);
  startMQTTListener("", inhale);
}

void scroll() {
    touchMQTT();
}

void react() {
  float feels = howamIfeeling();
  rest(feels);
}

void doomScroll() {
  scroll();
  react();
  if (selfTalk) { Serial.println(); }
}
