#include <Arduino.h>
#include "traffic_light.h"

#define DIM            0
#define BRIGHT        64
#define SHINE_MS    1500
#define FADE_MS     1500

ShuntingTrafficLight::ShuntingTrafficLight(bool ctrl_anode, unsigned char white_pin, unsigned char white_channel, unsigned char red_pin, unsigned char red_channel) :
  m_white_led(ctrl_anode, white_pin, white_channel),
  m_red_led(ctrl_anode, red_pin, red_channel),
  m_open(false),
  m_start_ms(0) {
}

void ShuntingTrafficLight::setup() {
  m_white_led.setup();
  m_red_led.setup();

  m_white_led.set(DIM);
  m_red_led.set(BRIGHT);
}

void ShuntingTrafficLight::loop() {
  if (0 == m_start_ms) {
    return;
  }
  unsigned long duration_ms = millis() - m_start_ms;
  LED& fading = m_open ? m_red_led : m_white_led;
  LED& shining = m_open ? m_white_led : m_red_led;
  if (duration_ms < FADE_MS) {
    fading.set(BRIGHT - BRIGHT * duration_ms/FADE_MS);
    shining.set(DIM);
    return;
  }
  duration_ms -= FADE_MS;
  if (duration_ms < SHINE_MS) {
    fading.set(DIM);
    shining.set(BRIGHT * duration_ms/SHINE_MS);
    return;
  }
  fading.set(DIM);
  shining.set(BRIGHT);
  m_start_ms = 0;  
}

void ShuntingTrafficLight::open(bool open) {
  if (m_open == open) {
    return;
  }
  
  m_open = open;
  const unsigned long current_ms = millis();
  const unsigned long expected_ms = m_start_ms + FADE_MS + SHINE_MS;
  if (0 == m_start_ms || current_ms > expected_ms) {
    m_start_ms = current_ms;
    return;
  }
  m_start_ms = current_ms - (expected_ms - current_ms);
}
  
