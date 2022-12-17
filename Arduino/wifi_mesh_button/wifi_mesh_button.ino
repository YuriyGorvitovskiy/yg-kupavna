#include <painlessMesh.h>

// #define RED
// #define YELLOW
#define GREEN

#ifdef RED
String nodeName = "RED"; // Name needs to be unique
int32_t nodeID = 533413225; // Node Id unique
#endif
#ifdef YELLOW
String nodeName = "YELLOW"; // Name needs to be unique
int32_t nodeID = 533311337; // Node Id unique
#endif
#ifdef GREEN
String nodeName = "GREEN"; // Name needs to be unique
int32_t nodeID = 533315385; // Node Id unique
#endif


#define   MESH_PREFIX     "YG-RR"
#define   MESH_PASSWORD   "TZcqBRT6yN.bCwdsT3Ux"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessage();

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

int counter = 0;
void sendMessage() {
  String msg = "ping";
  mesh.sendBroadcast( nodeName + ":" + ++counter );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
}
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);
  Serial.println(nodeName);

  //mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS ); 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );

/*
#ifdef RED
  mesh.setRoot(true);
#endif
  mesh.setContainsRoot(true);
*/

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  // put your setup code here, to run once:
}

void loop() {
  mesh.update();
  // construnct_json();
}
