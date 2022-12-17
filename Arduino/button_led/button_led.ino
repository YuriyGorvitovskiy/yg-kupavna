
#include <EEPROM.h>
#include "button.h"

const int LED_PIN = 27;
const int BUTTON_PIN = 32;
const int EPROM_SIZE = 50;
const int ADDRESS = 37;

Button button(BUTTON_PIN);

boolean prevButtonState = false;
//int ledState = LOW;
void setup() {
  //Serial.begin(9600);

  EEPROM.begin(EPROM_SIZE);
  
  // put your setup code here, to run once:
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setup();
  prevButtonState = button.pressed();

  int value = EEPROM.read(ADDRESS);
  digitalWrite(LED_PIN, value);
  //digitalWrite(LED_PIN, ledState);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the state of the pushbutton value:
  button.loop();
  //boolean currButtonState = digitalRead(BUTTON_PIN);
  boolean currButtonState = button.pressed();
  if (currButtonState && !prevButtonState) {
    //ledState = !ledState;
    int value = !EEPROM.read(ADDRESS);
    EEPROM.write(ADDRESS, value);
    EEPROM.commit();
    digitalWrite(LED_PIN, value);
    //digitalWrite(LED_PIN, ledState);
    //Serial.print(currButtonState);
    //Serial.print("-");
    //Serial.print(prevButtonState);
    //Serial.print("-");
    //Serial.print(value);
    //Serial.println();
    // delay(200);
  }
  //Serial.print(currButtonState);
  //Serial.println();
  prevButtonState = currButtonState;
}
