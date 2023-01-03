#ifndef turnout_h
#define turnout_h

#include "led.h"

enum TurnoutState {
  TURNOUT_STRAIGHT,
  TURNOUT_DIVERGE,
  TURNOUT_MOVE_TO_STRAIGHT,
  TURNOUT_MOVE_TO_DIVERGE,
};


class TurnoutSensor {
public:

  TurnoutSensor(unsigned char straight_pin, unsigned char diverge_pin, void (*onChange)(TurnoutSensor& turnout));
  void setup();
  void loop();
  
  TurnoutState getState();
  bool         isStraight();
  bool         isDiverge();

protected:
  const unsigned char m_straight_pin;
  const unsigned char m_diverge_pin;
  void              (*m_onChange)(TurnoutSensor& turnout);

  TurnoutState        m_state;
  unsigned long       m_checked_time_ms;
};


class TurnoutIndicator {
public:
  TurnoutIndicator(bool ctrl_anode, unsigned char red_pin, unsigned char red_channel, unsigned char green_pin, unsigned char green_channel, unsigned char blue_pin, unsigned char blue_channel);
  void setup();
  void loop();

  void setState(TurnoutState state);

protected:

  LED           m_red_led;
  LED           m_green_led;
  LED           m_blue_led;
};

inline TurnoutState TurnoutSensor::getState() {
  return m_state;
}
inline bool TurnoutSensor::isStraight() {
  return TurnoutState::TURNOUT_STRAIGHT == m_state;
}
inline bool TurnoutSensor::isDiverge() {
  return TurnoutState::TURNOUT_DIVERGE == m_state;
}

#endif  //block_h