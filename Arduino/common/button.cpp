#include <Arduino.h>
#include "button.h"

#define CHECK_INTERVAL_MS 200

Button::Button(unsigned char pin) {
  m_pin = pin;
  m_pressed = false;
  m_checked_time_ms = 0L;
  m_on_press = nullptr;
  m_on_release = nullptr;
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

  bool pressed = LOW == digitalRead(m_pin);
  if (pressed && !m_pressed) {
    m_pressed = pressed;
    if (nullptr != m_on_press) {
      (*m_on_press)();
    }
  } else if (!pressed && m_pressed) {
    m_pressed = pressed;
    if (nullptr != m_on_release) {
      (*m_on_release)();
    }
  }
}

void Button::onPress(void (*subscriber)()) {
  m_on_press = subscriber;
}

void Button::onRelease(void (*subscriber)()) {
  m_on_release = subscriber;
}
