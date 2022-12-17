#ifndef button_h
#define button_h

//Pressed - LOW, Released - HIGH

class Button {
public:

  Button(byte pin);

  void setup();
  void loop();

  bool pressed();

  void onPress(void (*subscriber)());
  void onRelease(void (*subscriber)());
  
protected:

  unsigned char m_pin;
  bool m_pressed;
  unsigned long m_checked_time_ms;
  void (*m_on_press)();
  void (*m_on_release)();
};

inline bool Button::pressed() {
  return m_pressed;
}

#endif  //button_h