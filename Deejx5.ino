#include "Keyboard.h"
#include "HIDRemote.h"

typedef struct {
  const int pin;
  const uint16_t key;
  const int flag; //Media flag 1: Media key 0: Macro
} Buttons;

const int NUM_BUTTONS = 5;

// {Pin number, Key command, Media flag}
Buttons buttons[NUM_BUTTONS] = {
  {21, HID_REMOTE_PREVIOUS, 1},
  {19, HID_REMOTE_PLAY_PAUSE, 1},
  {15, HID_REMOTE_NEXT, 1},
  {14, HID_REMOTE_MUTE, 1},
  {16, KEY_F24, 0}
};

//^^^ ONLY EDIT ^^^

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A6,A7,A8,A9,A10};
int analogSliderValues[NUM_SLIDERS];

int previousState[NUM_BUTTONS] = {HIGH,HIGH,HIGH,HIGH,HIGH};


void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  Serial.begin(9600);

  for (int i = 0; i < NUM_BUTTONS; i++) {
      pinMode(buttons[i].pin, INPUT_PULLUP);
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
    int buttonState = digitalRead(buttons[i].pin);

    if (buttonState != previousState[i]) {
      if (buttonState == LOW) {
        if (buttons[i].flag) {
          HIDRemote.press(buttons[i].key);
        } else {
          Keyboard.press(buttons[i].key);
        }

      } else {
        if (buttons[i].flag) {
          HIDRemote.release(buttons[i].key);
        } else {
          Keyboard.release(buttons[i].key);
        }

      }
    }
    previousState[i] = buttonState;
  }
}
