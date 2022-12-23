#ifndef led_h
#define led_h

//Shine - LOW, Released - HIGH

class LED {
public:

  LED(bool ctrl_anode, unsigned char pin, unsigned char channel);

  void setup();

  void set(unsigned char brightness);
  unsigned char get() const;
  
protected:

  const bool m_ctrl_anode;
  const unsigned char m_pin;
  const unsigned char m_channel;

  unsigned char m_brightness;
};

inline unsigned char LED::get()  const {
  return m_brightness;
}

class Slow {
public:
  Slow(LED& led, unsigned char brightness, unsigned long shine_ms, unsigned long fade_ms);

  void loop();

  void set(bool on);
  bool get() const;

protected:

  LED* m_led;
  const unsigned char m_brightness;
  const unsigned long m_shine_ms;
  const unsigned long m_fade_ms;

  bool m_state;
  unsigned long m_start_ms;
};

inline bool Slow::get()  const{
  return m_state;
}

class Blink {
public:
  Blink(LED& led, unsigned long shine_ms, unsigned long fade_ms);

  void loop();

  void set(bool blink);
  bool get() const;

protected:

  LED* m_led;
  const unsigned long m_shine_ms;
  const unsigned long m_fade_ms;

  bool m_blink;
  unsigned long m_brightness;
  unsigned long m_start_blink_ms;
};

inline bool Blink::get()  const{
  return m_blink;
}


#endif  //led_h