#include <Arduino.h>
#include "button.h"

#define CHECK_INTERVAL_MS 200

Button::Button(unsigned char pin) {
  m_pin = pin;
  m_pressed = false;
  m_checked_time_ms = 0L;
  m_subscriber = nullptr;
}

void Button::setup() {
  pinMode(m_pin, INPUT_PULLUP);
}

void Button::loop() {
  unsigned long time_ms = millis();
  if (m_checked_time_ms + CHECK_INTERVAL_MS > time_ms) {
    return;
  }
  m_checked_time_ms = time_ms;

  int state = digitalRead(m_pin);
  if ((LOW == state) != m_pressed) {
    m_pressed = (LOW == state);
    if (nullptr != m_subscriber) {
      (*m_subscriber)(*this);
    }
  }
}

void Button::subscribe(void (*subscriber)(Button& button)) {
  m_subscriber = subscriber;
}
