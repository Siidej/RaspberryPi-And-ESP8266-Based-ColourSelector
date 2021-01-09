#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/*
 * GPIO definitions
*/
#define S0 D6
#define S1 D7
#define S2 D3
#define S3 D4
#define sensorOut D5
#define THRESHOLD 2000
///////////////////////////////////////
/*
 * Wifi Connection
*/
const char* ssid = "Livebox-CA70";
const char* password = "dP5fvAHHTg4KLQTvdo";
const char* mqtt_server = "127.0.1.1";
///////////////////////////////////////
float red = 0;
float green = 0;
float blue = 0;
///////////////////////////////////////
WiFiClient espClient;
PubSubClient client(espClient);
///////////////////////////////////////
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,LOW);
  digitalWrite(S1,HIGH);
  Serial.begin(9600);
///////////////////////////////////////
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer("192.168.1.17", 1883);
  client.setCallback(callback);
}

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  //client.publish("Trier");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
}
 

int detectColour(int red, int green, int blue)
{
  int ar[3] = {red, green, blue};
  int minNum = THRESHOLD;
  int minIdx = 0;
  for(int i = 0; i < sizeof(ar)/sizeof(ar[0]); i++){
    if (ar[i] < minNum){
      minIdx = i;
      minNum = ar[i];
    }
  } 
  return minIdx;
}
void loop() {
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  delay(20);
  red = pulseIn(sensorOut, LOW)*0.97;
  
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  delay(20);
  green = pulseIn(sensorOut, LOW)/1.6;
  //for (green = pulseIn(sensorOut, LOW)/2.5; green < 1.5; green = pulseIn(sensorOut, LOW)/2.5)

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  delay(20);
  blue = pulseIn(sensorOut, LOW)/2;
  //for (blue = pulseIn(sensorOut, LOW)/2.6; blue < 1.5; blue = pulseIn(sensorOut, LOW)/2.6)
  delay(50);
  

  if (red < THRESHOLD || green < THRESHOLD || blue <THRESHOLD) 
  {
    int colIdx = detectColour(red, green, blue);
    Serial.print(colIdx);
    char colIdxStr[1];
    dtostrf(colIdx, 1, 0, colIdxStr);
    client.publish("Trier", colIdxStr);
  }
  else
  {
    Serial.print(red);//printing RED color frequency
    Serial.println("  ");
    Serial.print(green);//printing RED color frequency
    Serial.println("  ");
    Serial.print(blue);//printing RED color frequency
    Serial.println("  ");
    Serial.println("xxxxxxxxxxxxxxx");
  }
  delay(50);
}
