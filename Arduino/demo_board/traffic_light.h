#ifndef traffic_light_h
#define traffic_light_h

#include "led.h"

class ShuntingTrafficLight {
public:
  ShuntingTrafficLight(bool ctrl_anode, unsigned char white_pin, unsigned char white_channel, unsigned char red_pin, unsigned char red_channel);
  void setup();
  void loop();

  void open(bool open);
  void stop();

protected:

  LED           m_white_led;
  LED           m_red_led;

  bool          m_open;
  unsigned long m_start_ms;
};

inline void ShuntingTrafficLight::stop() {
  open(false);
}

#endif  //traffic_light_h
