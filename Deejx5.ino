#include "Keyboard.h"
#include "HIDRemote.h"

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A6,A7,A8,A9,A10};

const int NUM_BUTTONS = 5;
const int buttonInputs[NUM_BUTTONS] = {21,19,15,14,16};
// const char *keyBindings[NUM_BUTTONS] = {"1","2","3","4","5"};
const uint16_t keyBindings[NUM_BUTTONS] = {HID_REMOTE_PREVIOUS, HID_REMOTE_PLAY_PAUSE, HID_REMOTE_NEXT, HID_REMOTE_MUTE, 0x185};
int previousState[NUM_BUTTONS] = {HIGH,HIGH,HIGH,HIGH,HIGH};

int analogSliderValues[NUM_SLIDERS];

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  Serial.begin(9600);

  for (int i = 0; i < NUM_BUTTONS; i++) {
      pinMode(buttonInputs[i], INPUT_PULLUP);
  }
  Keyboard.begin();

}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  updateButtons();
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void updateButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int buttonState = digitalRead(buttonInputs[i]);

    if ((buttonState != previousState[i]) && (buttonState == LOW)) {
      // Keyboard.write(keyBindings[i]);
      HIDRemote.press(keyBindings[i]);
    } else if ((buttonState != previousState[i]) && (buttonState == HIGH)) {
      HIDRemote.release(keyBindings[i]);
    }
    previousState[i] = buttonState;
  }
}
