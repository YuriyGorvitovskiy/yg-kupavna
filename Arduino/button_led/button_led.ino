
#include <EEPROM.h>
#include "button.h"

const int LED_PIN = 27;
const int BUTTON_PIN = 32;
const int EPROM_SIZE = 50;
const int ADDRESS = 37;

Button button(BUTTON_PIN);

void onButtonPress() {
    bool value = !EEPROM.read(ADDRESS);
    EEPROM.write(ADDRESS, value);
    EEPROM.commit();

    digitalWrite(LED_PIN, value ? HIGH : LOW);
}

void setup() {
  //Serial.begin(9600);
  EEPROM.begin(EPROM_SIZE);
  pinMode(LED_PIN, OUTPUT);
  button.setup();

  bool value = EEPROM.read(ADDRESS);
  digitalWrite(LED_PIN, value ? HIGH : LOW);

  button.onPress(&onButtonPress);
}

void loop() {
  button.loop();
  //Serial.print("Button: " + button.pressed());
  //Serial.println();
}
