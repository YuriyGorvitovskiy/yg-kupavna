#include <Arduino.h>
#include "turnout.h"


#define DIM                 0
#define BRIGHT             64
#define CHECK_INTERVAL_MS  50

TurnoutSensor::TurnoutSensor(unsigned char straight_pin, unsigned char diverge_pin, void (*onChange)(TurnoutSensor& turnout)) :
  m_straight_pin(straight_pin),
  m_diverge_pin(diverge_pin),
  m_onChange(onChange),
  m_state(TurnoutState::TURNOUT_MOVE_TO_DIVERGE),
  m_checked_time_ms(0) {
}

void TurnoutSensor::setup() {
  pinMode(m_straight_pin, INPUT_PULLUP);
  pinMode(m_diverge_pin, INPUT_PULLUP);
}

void TurnoutSensor::loop() {
  unsigned long time_ms = millis();
  if (m_checked_time_ms + CHECK_INTERVAL_MS > time_ms) {
    return;
  }
  m_checked_time_ms = time_ms;

  TurnoutState new_state = m_state;
  if (LOW == digitalRead(m_straight_pin)) {
    new_state = TurnoutState::TURNOUT_STRAIGHT;
  } else if (LOW == digitalRead(m_diverge_pin)) {
    new_state = TurnoutState::TURNOUT_DIVERGE;
  } else if (TurnoutState::TURNOUT_STRAIGHT == m_state) {
    new_state = TurnoutState::TURNOUT_MOVE_TO_DIVERGE;
  } else if (TurnoutState::TURNOUT_DIVERGE == m_state) {
    new_state = TurnoutState::TURNOUT_MOVE_TO_STRAIGHT;
  }
  if (new_state == m_state) {
    return;
  }

  m_state = new_state;
  if (nullptr != m_onChange) {
    (*m_onChange)(*this);
  }
}

TurnoutIndicator::TurnoutIndicator(bool ctrl_anode, unsigned char red_pin, unsigned char red_channel, unsigned char green_pin, unsigned char green_channel, unsigned char blue_pin, unsigned char blue_channel):
  m_red_led(ctrl_anode, red_pin, red_channel),
  m_green_led(ctrl_anode, green_pin, green_channel),
  m_blue_led(ctrl_anode, blue_pin, blue_channel) {
}

void TurnoutIndicator::setup() {
  m_red_led.setup();
  m_green_led.setup();
  m_blue_led.setup();
}

void TurnoutIndicator::loop() {
}

void TurnoutIndicator::setState(TurnoutState state) {
    switch(state) {
      case TurnoutState::TURNOUT_STRAIGHT: 
        m_red_led.set(DIM);
        m_green_led.set(BRIGHT);
        m_blue_led.set(DIM);
        break;
      case TurnoutState::TURNOUT_DIVERGE: 
        m_red_led.set(BRIGHT);
        m_green_led.set(BRIGHT);
        m_blue_led.set(DIM);
        break;
      case TurnoutState::TURNOUT_MOVE_TO_STRAIGHT: 
        m_red_led.set(DIM);
        m_green_led.set(DIM);
        m_blue_led.set(BRIGHT);
        break;
      case TurnoutState::TURNOUT_MOVE_TO_DIVERGE: 
        m_red_led.set(BRIGHT);
        m_green_led.set(DIM);
        m_blue_led.set(DIM);
        break;
    }
}
