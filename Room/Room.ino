/* Drafted by Jordan Mellish (jordan.mellish@townsville.qld.gov.au)
 * MIT License 
 * Description: Basic code for sending DHT11(Temperature and Humidity) data to ThingSpeak with a WeMos D1 Pro or D1 Mini  
 * Including ESP Deep Sleep for 15 minutes.
 */

/* Import Libraries  */ 
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <DHT.h>

/* Definitions */ 
#define DHTPIN D4 //select the pin used for the dht11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int sleepTimeS = 900;
int lightpin = A0;    // select the input pin for the light sensor
int sensorvalue = 0;  // variable to store the value coming from the sensor

/* Wi-Fi Credentials */ 
char ssid[] = "";    //  your network SSID (name) 
char pswd[] = "";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

/* ThingSpeak Credentials  */
unsigned long myChannelNumber1 = 1111111; // Enter your channel number from ThingSpeak
const char * myWriteAPIKey1 = "api"; //Enter your Write API Key from ThingSpeak

unsigned long myChannelNumber2 = 1111111; // Enter your channel number from ThingSpeak
const char * myWriteAPIKey2 = "api"; //Enter your Write API Key from ThingSpeak

unsigned long myChannelNumber3 = 1111111; // Enter your channel number from ThingSpeak
const char * myWriteAPIKey3 = "api"; //Enter your Write API Key from ThingSpeak

void setup() {
  Serial.begin(115200); 
  dht.begin(); 
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED) { connectWiFi();}
    Serial.print("I'm Awake");
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    sensorvalue = analogRead(lightpin);
    Serial.print(t);
    Serial.print(h);
    Serial.print(sensorvalue);
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT Sensor");
      return;
    } else { 
    //ThingSpeak.setField(Field No. , data)
    ThingSpeak.setField(1, t);
    ThingSpeak.writeFields(myChannelNumber1, myWriteAPIKey1);
    delay(2000);
    
    ThingSpeak.setField(1, h);
    ThingSpeak.writeFields(myChannelNumber2, myWriteAPIKey2);

    delay(2000);
    ThingSpeak.setField(1, sensorvalue);
    ThingSpeak.writeFields(myChannelNumber3, myWriteAPIKey3);
    
    Serial.println("Back to sleep");
    ESP.deepSleep(sleepTimeS * 1000000);
    }

}

void loop() {
}

void connectWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address :");
  Serial.println(WiFi.localIP());
  Serial.println();
}
