#include <Arduino.h>
#include "led.h"

#define PWM_FRQUENCY 5000
#define PWM_RESOLUTION 8
#define PWM_MIN 0
#define PWM_MAX 255

LED::LED(bool ctrl_anode, unsigned char pin, unsigned char channel) :
  m_ctrl_anode(ctrl_anode),
  m_pin(pin),
  m_channel(channel) {
  m_brightness = PWM_MIN;
}

void LED::setup() {
  ledcSetup(m_channel, PWM_FRQUENCY, 8);
  ledcAttachPin(m_pin, m_channel);
  ledcWrite(m_channel, m_ctrl_anode ? m_brightness : PWM_MAX - m_brightness);
}

void LED::set(unsigned char brightness) {
  if (m_brightness != brightness) {
    m_brightness = brightness;
    ledcWrite(m_channel, m_ctrl_anode ? m_brightness : PWM_MAX - m_brightness);
  }
}

Slow::Slow(LED& led, unsigned char brightness, unsigned long shine_ms, unsigned long fade_ms):
 m_led(&led),
 m_brightness(max((int)brightness,PWM_MAX)),
 m_shine_ms(shine_ms),
 m_fade_ms(fade_ms) {
  m_state = m_led->get() != PWM_MIN;
  m_start_ms = 0L;
}

void Slow::loop() {
  if (0L == m_start_ms) {
    return;
  }

  unsigned long current_ms = millis();
  if (m_state) {
    if (current_ms - m_start_ms >= m_shine_ms) {
      m_start_ms = 0L;
      m_led->set(m_brightness);
    } else {  
      m_led->set(m_brightness * (current_ms - m_start_ms)/m_shine_ms);
    }
  } else {
    if (current_ms - m_start_ms >= m_fade_ms) {
      m_start_ms = 0L;
      m_led->set(PWM_MIN);
    } else {  
      m_led->set(m_brightness * (m_fade_ms - (current_ms - m_start_ms))/m_fade_ms);
    }
  }
}

void Slow::set(bool on) {
  if (m_state == on) {
    return;
  }
  m_state == on;
  unsigned long current_ms = millis();
  if (0L == m_start_ms) {
    m_start_ms = current_ms;
    return;
  }
  if (m_state) {
    if (current_ms - m_start_ms > m_fade_ms) {
      m_start_ms = current_ms;
      return;
    }
    m_start_ms = current_ms - (m_fade_ms - (current_ms - m_start_ms)) * m_shine_ms / m_fade_ms;
  } else {
    if (current_ms - m_start_ms > m_shine_ms) {
      m_start_ms = current_ms;
      return;
    }
    m_start_ms = current_ms - (m_shine_ms - (current_ms - m_start_ms)) * m_fade_ms / m_shine_ms;
  }
}

Blink::Blink(LED& led, unsigned long shine_ms, unsigned long fade_ms) :
 m_led(&led),
 m_shine_ms(shine_ms),
 m_fade_ms(fade_ms) {
  m_blink = false;
  m_brightness = 0;
  m_start_blink_ms = 0L;
}

void Blink::loop() {
  if (!m_blink) {
    return;
  }
  unsigned long ms = (millis() - m_start_blink_ms) % (m_fade_ms + m_shine_ms);
  m_led->set(ms <= m_fade_ms ? PWM_MIN : m_brightness);
}

void Blink::set(bool blink) {
  if (m_blink == blink) {
    return;
  }
  m_blink = blink;
  if (m_blink) {
    m_brightness = m_led->get();
    m_start_blink_ms = millis();
  } else {
    m_brightness = PWM_MIN;
    m_start_blink_ms = 0L;
  }
}
