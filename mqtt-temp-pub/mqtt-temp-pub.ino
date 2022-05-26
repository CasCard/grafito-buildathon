/*
  Basic ESP8266 MQTT example
  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.
  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> // http web access library
#include <ArduinoJson.h>       // JSON decoding library
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char *ssid = "Function2";
const char *password = "Function";
const char *mqtt_server = "192.168.199.142";

// set location and API key
String Location = "Kochi,IN";
String API_Key = "9f385b5cd968cedd56522634f74eb068";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
                                        // but actually the LED is on; this is because
                                        // it is active low on the ESP-01)
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsg > 10000)
    {
        lastMsg = now;
        //    Serial.println("Hi");

        WiFiClient happy;
        HTTPClient http; // Declare an object of class HTTPClient

        // specify request destination
        http.begin(happy, "http://api.openweathermap.org/data/2.5/weather?q=Kochi,IN&APPID=9f385b5cd968cedd56522634f74eb068"); // !!
        int httpCode = http.GET();                                                                                             // send the request
        if (httpCode > 0)                                                                                                      // check the returning code
        {
            String payload = http.getString(); // Get the request response payload

            DynamicJsonBuffer jsonBuffer(512);

            // Parse JSON object
            JsonObject &root = jsonBuffer.parseObject(payload);
            if (!root.success())
            {
                Serial.println(F("Parsing failed!"));
                return;
            }

            float temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
            float humidity = (float)root["main"]["humidity"];    // get humidity in %

            dtostrf(temp, 6, 2, msg);

            client.publish("temperature/Kochi", msg);
            dtostrf(humidity, 6, 2, msg);
            client.publish("humidity/Kochi", msg);
        }

        http.begin(happy, "http://api.openweathermap.org/data/2.5/weather?q=Edappally,IN&APPID=9f385b5cd968cedd56522634f74eb068"); // !!
        httpCode = http.GET();                                                                                                     // send the request
        if (httpCode > 0)                                                                                                          // check the returning code
        {
            String payload = http.getString(); // Get the request response payload

            DynamicJsonBuffer jsonBuffer(512);

            // Parse JSON object
            JsonObject &root = jsonBuffer.parseObject(payload);
            if (!root.success())
            {
                Serial.println(F("Parsing failed!"));
                return;
            }

            float temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
            float humidity = (float)root["main"]["humidity"];    // get humidity in %

            dtostrf(temp, 6, 2, msg);

            client.publish("temperature/Edappally", msg);
            dtostrf(humidity, 6, 2, msg);
            client.publish("humidity/Edappally", msg);
        }

        http.begin(happy, "http://api.openweathermap.org/data/2.5/weather?q=Aluva,IN&APPID=9f385b5cd968cedd56522634f74eb068"); // !!
        httpCode = http.GET();                                                                                                 // send the request
        if (httpCode > 0)                                                                                                      // check the returning code
        {
            String payload = http.getString(); // Get the request response payload

            DynamicJsonBuffer jsonBuffer(512);

            // Parse JSON object
            JsonObject &root = jsonBuffer.parseObject(payload);
            if (!root.success())
            {
                Serial.println(F("Parsing failed!"));
                return;
            }

            float temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
            float humidity = (float)root["main"]["humidity"];    // get humidity in %

            dtostrf(temp, 6, 2, msg);

            client.publish("temperature/Aluva", msg);
            dtostrf(humidity, 6, 2, msg);
            client.publish("humidity/Aluva", msg);
        }

        http.begin(happy, "http://api.openweathermap.org/data/2.5/weather?q=Kalamassery,IN&APPID=9f385b5cd968cedd56522634f74eb068"); // !!
        httpCode = http.GET();                                                                                                       // send the request
        if (httpCode > 0)                                                                                                            // check the returning code
        {
            String payload = http.getString(); // Get the request response payload

            DynamicJsonBuffer jsonBuffer(512);

            // Parse JSON object
            JsonObject &root = jsonBuffer.parseObject(payload);
            if (!root.success())
            {
                Serial.println(F("Parsing failed!"));
                return;
            }

            float temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
            float humidity = (float)root["main"]["humidity"];    // get humidity in %

            dtostrf(temp, 6, 2, msg);

            client.publish("temperature/Kalamassery", msg);
            dtostrf(humidity, 6, 2, msg);
            client.publish("humidity/Kalamassery", msg);
        }
    }
}
