#include <EEPROM.h>
#include <painlessMesh.h>
#include "button.h"

#define GREEN  0x1FC9BF39
#define YELLOW 0x1FC9AF69
#define RED    0x1FCB3D69

#define CURRENT GREEN
//#define CURRENT YELLOW
//#define CURRENT RED

#if CURRENT == GREEN
#define DEPENDS RED
#elif CURRENT == YELLOW
#define DEPENDS GREEN
#else
#define DEPENDS YELLOW
#endif

#define MESH_PREFIX     "YG-RR"
#define MESH_PASSWORD   "TZcqBRT6yN.bCwdsT3Ux"
#define MESH_PORT       5555

#define EEPROM_DEVICE_ADDRESS   0
#define EEPROM_SIZE             256

#define DEVICE_LOCAL    1
#define DEVICE_EXTERNAL 2

#define LED_PIN      27
#define BUTTON_PIN   32

Scheduler     userScheduler;
painlessMesh  mesh;
Button        button(BUTTON_PIN);

bool getState(int device) {
  return EEPROM.read(EEPROM_DEVICE_ADDRESS + device);
}

void setState(int device, bool state) {
  EEPROM.write(EEPROM_DEVICE_ADDRESS + device, state);
  EEPROM.commit();

  if (DEVICE_EXTERNAL == device) {
    digitalWrite(LED_PIN, state ? HIGH : LOW);
  }
  Serial.println("State: " + String(device, DEC) + ":" + (state ? "T" : "F"));
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
  if (msg.startsWith("REQIEST:")) {
    byte device = msg.substring(8).toInt(); 
    updateMessage(device, getState(device));
    return;
  }  
  if (msg.startsWith("UPDATE:")) {
    if (DEPENDS == from) {
      String payload = msg.substring(7);
      int i = payload.indexOf(":");
      byte device =  payload.substring(0,i).toInt();
      boolean state = 'T' == payload[i+1];
      setState(device + 1, state);
    }
    return;
  }
}

void onButtonPress() {
  bool newState = !getState(DEVICE_LOCAL);
  setState(DEVICE_LOCAL, newState);
  updateMessage(DEVICE_LOCAL, newState);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Restart");
  EEPROM.begin(EEPROM_SIZE);
  pinMode(LED_PIN, OUTPUT);

  //mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS ); 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&onMessage);

  button.setup();
  button.onPress(&onButtonPress);

  digitalWrite(LED_PIN, getState(DEVICE_EXTERNAL) ? HIGH : LOW);

  updateMessage(DEVICE_LOCAL, getState(DEVICE_LOCAL));
  requestMessage(DEPENDS, DEVICE_LOCAL);
}

void loop() {
  mesh.update();
  button.loop();
}
