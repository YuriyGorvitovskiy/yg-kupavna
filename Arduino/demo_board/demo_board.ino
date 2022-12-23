#include <EEPROM.h>
#include <painlessMesh.h>
#include "button.h"
#include "led.h"

#define GREEN  0x1FC9BF39
#define YELLOW 0x1FC9AF69
#define RED    0x1FCB3D69

//#define CURRENT GREEN
//#define CURRENT YELLOW
#define CURRENT RED

#define MESH_PREFIX     "YG-RR"
#define MESH_PASSWORD   "TZcqBRT6yN.bCwdsT3Ux"
#define MESH_PORT       5555


#define BUTTON_PIN           23
#define BUTTON_BLUE_LED_PIN  22
#define BUTTON_GREEN_LED_PIN  1
#define BUTTON_RED_LED_PIN    3
#define TRAFIC_RED_LED_PIN   21
#define TRAFIC_WHITE_LED_PIN 19

#define EEPROM_DEVICE_ADDRESS   0
#define EEPROM_SIZE             256

#define DEVICE_LOCAL    1


Scheduler     userScheduler;
painlessMesh  mesh;



Button        button(BUTTON_PIN);
LED           buttonBlueLed(true, BUTTON_BLUE_LED_PIN, 0);
LED           buttonGreenLed(true, BUTTON_GREEN_LED_PIN, 1);
LED           buttonRedLed(true, BUTTON_RED_LED_PIN, 2);
LED           trafficRedLed(false, TRAFIC_RED_LED_PIN, 3);
LED           trafficWhiteLed(false, TRAFIC_WHITE_LED_PIN, 4);

bool getState() {
  return EEPROM.read(EEPROM_DEVICE_ADDRESS + DEVICE_LOCAL);
}

void setState(bool state) {
  EEPROM.write(EEPROM_DEVICE_ADDRESS + DEVICE_LOCAL, state);
  EEPROM.commit();

  if (state) {
    buttonRedLed.set(0);
    buttonGreenLed.set(255);
    buttonBlueLed.set(0);
    trafficRedLed.set(255);
    trafficWhiteLed.set(0);
  } else {
    buttonRedLed.set(255);
    buttonGreenLed.set(255);
    buttonBlueLed.set(0);
    trafficRedLed.set(0);
    trafficWhiteLed.set(255);
  }
  Serial.println(String("State: ") + (state ? "Straight" : "Diverge"));
}

void updateMessage(byte device, bool state) {
  mesh.sendBroadcast("UPDATE:" + String(device, DEC) + ":" + (state ? "T" : "F"));
}

void updateMessage(uint32_t destId, byte device, bool state) {
  mesh.sendSingle(destId, "UPDATE:" + String(device, DEC) + ":" + (state ? "T" : "F"));
}

void requestMessage(uint32_t destId, byte device) {
  mesh.sendSingle(destId, "REQUEST:" +  String(device, DEC));
}

void onMessage( uint32_t from, String &msg ) {
  Serial.println("Message: " + msg);
}

void onButtonPress() {
  bool newState = !getState();
  setState(newState);
  updateMessage(DEVICE_LOCAL, newState);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Restart");
  EEPROM.begin(EEPROM_SIZE);
  

  //mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS ); 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&onMessage);

  button.setup();
  button.onPress(&onButtonPress);

  buttonBlueLed.setup();
  buttonGreenLed.setup();
  buttonRedLed.setup();
  trafficRedLed.setup();
  trafficWhiteLed.setup();

  setState(getState());

  updateMessage(DEVICE_LOCAL, getState());
  // requestMessage(DEPENDS, DEVICE_LOCAL);
}

void loop() {
  mesh.update();
  button.loop();
}
