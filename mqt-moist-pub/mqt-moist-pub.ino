#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid ="Function2"; //replace this with your wifi  name
const char* password ="Function"; //replace with your wifi password
char hostname[] ="192.168.199.142"; //replace this with IP address of machine 
//on which broker is installed
#define TOKEN "bytesofgigabytes"

int sense_Pin = 0; // sensor input at Analog pin A0

int value = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.println("ESP8266 AS PUBLISHER");
  client.setServer(hostname, 1883 ); //default port for mqtt is 1883
}

void loop()
{
  if ( !client.connected() )
  {
  reconnect();
  }
  MQTTPOST();
  delay(5000);//delay 5 Sec
}

void MQTTPOST()
{
  value= analogRead(sense_Pin);
  value= value/10;
  //payload formation begins here
  String payload ="{";
  payload +="\"Temp\":"; payload +=10; payload +=",";
  payload +="\"Humi\":"; payload +=20;
  payload +="}";
  
  char attributes[1000];
  payload.toCharArray( attributes, 1000 );
  client.publish("test", attributes); //topic="test" MQTT data post command.
  Serial.println( attributes );
}
//this function helps you reconnect wifi as well as broker if connection gets disconnected.
void reconnect() 
{
while (!client.connected()) {
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
  Serial.print("Connecting to Broker …");
  Serial.print("192.168.199.142");
  
  if ( client.connect("ESP8266 Device", TOKEN, NULL) )
  {
    Serial.println("[DONE]" );
  }
  else {
    Serial.println( " : retrying in 5 seconds]" );
    delay( 5000 );
  }
  }
}