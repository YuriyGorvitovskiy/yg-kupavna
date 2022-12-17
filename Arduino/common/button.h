#ifndef button_h
#define button_h

//Pressed - LOW, Released - HIGH

class Button {
public:

  Button(byte pin);

  void setup();
  void loop();

  void subscribe(void (*subscriber)(Button& button));
  bool pressed();

protected:

  unsigned char m_pin;
  bool m_pressed;
  unsigned long m_checked_time_ms;
  void (*m_subscriber)(Button& button);
};

inline bool Button::pressed() {
  return m_pressed;
}

#endif  //button_h