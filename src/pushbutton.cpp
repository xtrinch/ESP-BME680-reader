#include "pushbutton.h"

int btnState, prevBtnState;
long btnPressCount = 0;
unsigned long btnPressMillis = 0;

void setupButton() {
  // pin is pulled low by the button when active
  pinMode(BTN_PIN, INPUT_PULLUP);
}

bool checkButtonPressed() {
  btnState = digitalRead(BTN_PIN); // check the state of the button

  if (millis() - btnPressMillis >= BTN_SAMPLE_INTERVAL) {
    btnPressMillis = millis();

    if (btnState == BTN_PRESSED_STATE && prevBtnState == BTN_RELEASED_STATE) {
      Serial.println("Key press");
      btnPressCount = 0;
    } else if (btnState == BTN_RELEASED_STATE && prevBtnState == BTN_PRESSED_STATE) {
      Serial.println("Key release");

      // make sure to call this on release only, as we do not want the button pressed when it resets
      if (btnPressCount >= BTN_RESET_CYCLES) {
        clearConfig();
        // reset so it will execute setup again
        ESP.restart();
      }
    } else if (btnState == BTN_PRESSED_STATE) {
      btnPressCount ++;
    }

    prevBtnState = btnState;
  }

  // return previous button state to make sure we register all states in here before sleeping
  if (prevBtnState == BTN_PRESSED_STATE) {
    return true;
  }

  return false;
}