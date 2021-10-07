#include "MQTT_Handler.h"
//20*5*number
//20Hz - 655Hz


// ------------------------------------------------------
// ---------------------------------------------- INPUTS
// ------------------------------------------------------

//---------------------------------  ZOT
const int pushButton = 4;
const bool trueState = LOW;
bool buttonState = false;
const int debounceDelay = 5;
int lastButtonState = 0;
//
void checkButton() {
  int buttonRead = digitalRead(pushButton);
  if (buttonRead != lastButtonState) {
    delay(debounceDelay);
    // and it's pressed:
    if (buttonRead == trueState) {
      buttonState = true;
    } else {
      buttonState = false;
    }
    // save current state for next comparison:
    lastButtonState = buttonState;
  }
}



//---------------------------------  FORTITUDE
const int fortitudePin = 0;  //A0 input
int currentFortitude;

int getFortitude() {
  int val = analogRead(fortitudePin);
  return val;
}


//String getValueForMessage(int sensorPin) {
//  int val = analogRead(sensorPin);
//  //return "dial it in";
//  return String((uint8_t) (map(val, 0, 1023, 0, 127)));
//}

// ------------------------------------------------------
// ---------------------------------------------- OUTPUTS
// ------------------------------------------------------

const int blinkPin = 3;
int blinkPinState = LOW;
const long blinkInterval = 1000; 
unsigned long LED_lastMillis;


void updateBlinkLED(unsigned long currentTime) {

  if (currentTime - LED_lastMillis >= blinkInterval) {
    // save the last time you blinked the LED
    LED_lastMillis = currentTime;

    // if the LED is off turn it on and vice-versa:
    if (blinkPinState == LOW) {
      blinkPinState = HIGH;
    } else {
      blinkPinState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(blinkPin, blinkPinState);
  }
}

// ------------------------------------------------------
// ------------------------------------------------ SETUP
// ------------------------------------------------------

void hardwareSetUp() {
    pinMode(pushButton, INPUT);
    pinMode(blinkPin, OUTPUT);
}

// ------------------------------------------------------
// ----------------------------------------- FOR THE LOOP
// ------------------------------------------------------

void haveFeelings(unsigned long currentTime) {
  updateBlinkLED(currentTime);
  checkButton();
  
}
