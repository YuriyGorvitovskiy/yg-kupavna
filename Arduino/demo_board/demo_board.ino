#include <EEPROM.h>
#include <painlessMesh.h>
#include "block.h"
#include "turnout.h"
#include "traffic_light.h"

#define MESH_PREFIX                   "YG-RR"
#define MESH_PASSWORD  "TZcqBRT6yN.bCwdsT3Ux"
#define MESH_PORT                       5555
#define MESH_BOARD_ID             0x1FCB3D69

#define PIN_BLOCK_1_POWER                 15
#define PIN_BLOCK_1_OCCUPY                 4
#define PIN_BLOCK_1_SHORT                 16

#define PIN_BLOCK_2_POWER                 17
#define PIN_BLOCK_2_OCCUPY                 5
#define PIN_BLOCK_2_SHORT                 18

#define PIN_BLOCK_3_POWER                 19
#define PIN_BLOCK_3_OCCUPY                21
#define PIN_BLOCK_3_SHORT                  3

#define PIN_TRAFFIC_LIGHT_M1_WHITE        13
#define PIN_TRAFFIC_LIGHT_M1_RED          12

#define PIN_TRAFFIC_LIGHT_M2_WHITE        14
#define PIN_TRAFFIC_LIGHT_M2_RED          27

#define PIN_TRAFFIC_LIGHT_M3_WHITE        26
#define PIN_TRAFFIC_LIGHT_M3_RED          25

#define PIN_TURNOUT_2_SENSOR_STRAIGHT     33
#define PIN_TURNOUT_2_SENSOR_DIVERGE      32

#define PIN_TURNOUT_2_INDICATOR_LED_RED   23
#define PIN_TURNOUT_2_INDICATOR_LED_GREEN 22
#define PIN_TURNOUT_2_INDICATOR_LED_BLUE   1

#define CHANNEL_TRAFFIC_LIGHT_M1_WHITE         0
#define CHANNEL_TRAFFIC_LIGHT_M1_RED           1

#define CHANNEL_TRAFFIC_LIGHT_M2_WHITE         2
#define CHANNEL_TRAFFIC_LIGHT_M2_RED           3

#define CHANNEL_TRAFFIC_LIGHT_M3_WHITE         4
#define CHANNEL_TRAFFIC_LIGHT_M3_RED           5

#define CHANNEL_TURNOUT_2_INDICATOR_LED_RED    6
#define CHANNEL_TURNOUT_2_INDICATOR_LED_GREEN  7
#define CHANNEL_TURNOUT_2_INDICATOR_LED_BLUE   8

#define EEPROM_DEVICE_ADDRESS              0
#define EEPROM_SIZE                        3

void onBlockChange(Block& block);
void onTurnoutChange(TurnoutSensor& turnout);
void onMessage(uint32_t from, String &msg);

painlessMesh          mesh;
Scheduler             mesh_scheduler;

Block                 block_1(PIN_BLOCK_1_POWER, PIN_BLOCK_1_OCCUPY, PIN_BLOCK_1_SHORT, &onBlockChange);
Block                 block_2(PIN_BLOCK_2_POWER, PIN_BLOCK_2_OCCUPY, PIN_BLOCK_2_SHORT, &onBlockChange);
Block                 block_3(PIN_BLOCK_3_POWER, PIN_BLOCK_3_OCCUPY, PIN_BLOCK_3_SHORT, &onBlockChange);

ShuntingTrafficLight  traffic_light_M1(false, PIN_TRAFFIC_LIGHT_M1_WHITE, CHANNEL_TRAFFIC_LIGHT_M1_WHITE, PIN_TRAFFIC_LIGHT_M1_RED, CHANNEL_TRAFFIC_LIGHT_M1_RED);
ShuntingTrafficLight  traffic_light_M2(false, PIN_TRAFFIC_LIGHT_M2_WHITE, CHANNEL_TRAFFIC_LIGHT_M2_WHITE, PIN_TRAFFIC_LIGHT_M2_RED, CHANNEL_TRAFFIC_LIGHT_M2_RED);
ShuntingTrafficLight  traffic_light_M3(false, PIN_TRAFFIC_LIGHT_M3_WHITE, CHANNEL_TRAFFIC_LIGHT_M3_WHITE, PIN_TRAFFIC_LIGHT_M3_RED, CHANNEL_TRAFFIC_LIGHT_M3_RED);

TurnoutSensor         turnout_2_sensor(PIN_TURNOUT_2_SENSOR_STRAIGHT, PIN_TURNOUT_2_SENSOR_DIVERGE, &onTurnoutChange);
TurnoutIndicator      turnout_2_indicator(true, PIN_TURNOUT_2_INDICATOR_LED_RED, CHANNEL_TURNOUT_2_INDICATOR_LED_RED,  PIN_TURNOUT_2_INDICATOR_LED_GREEN, CHANNEL_TURNOUT_2_INDICATOR_LED_GREEN, PIN_TURNOUT_2_INDICATOR_LED_BLUE, CHANNEL_TURNOUT_2_INDICATOR_LED_BLUE);

void update() {
  /*
  traffic_light_M1.open(block_1.isOccupied());
  traffic_light_M2.open(block_2.isOccupied()); 
  traffic_light_M3.open(block_3.isOccupied()); 
  */
  if(turnout_2_sensor.isStraight()) {
    traffic_light_M1.open(block_2.isFree()); 
    traffic_light_M2.open(block_1.isFree()); 
    traffic_light_M3.stop(); 
  } else if(turnout_2_sensor.isDiverge()) {
    traffic_light_M1.stop(); 
    traffic_light_M2.open(block_3.isFree()); 
    traffic_light_M3.open(block_2.isFree()); 
  } else {
    traffic_light_M1.stop(); 
    traffic_light_M2.stop(); 
    traffic_light_M3.stop(); 
  }
}
void onTurnoutChange(TurnoutSensor& turnout) {
  if (&turnout_2_sensor == &turnout) {
    turnout_2_indicator.setState(turnout.getState());
  }
  update();
}

void onBlockChange(Block& block) {
  update();
}

void onMessage(uint32_t from, String &msg) {
}

void setup() {
  //Serial.begin(115200);
  //Serial.println("Restart");
  EEPROM.begin(EEPROM_SIZE);
  
  //mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS ); 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &mesh_scheduler, MESH_PORT );
  mesh.onReceive(&onMessage);

  block_1.setup();
  block_2.setup();
  block_3.setup();

  traffic_light_M1.setup();
  traffic_light_M2.setup();
  traffic_light_M3.setup();

  turnout_2_sensor.setup();
  turnout_2_indicator.setup();
}

void loop() {
  mesh.update();

  block_1.loop();
  block_2.loop();
  block_3.loop();
  
  traffic_light_M1.loop();
  traffic_light_M2.loop();
  traffic_light_M3.loop();

  turnout_2_sensor.loop();
  turnout_2_indicator.loop();
}
