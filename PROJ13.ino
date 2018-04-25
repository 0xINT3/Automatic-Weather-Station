
#include <DHT.h>
#include <ESP8266WiFi.h>

#include <Wire.h>

#define DHTPIN D4
  
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);


int nRainIn = A0;
int nRainDigitalIn = 2;
int nRainVal;
boolean bIsRaining = false;
String strRaining;
int WET = 16; // Wet Indicator at Digital PIN D0
int DRY = 2; // Dry Indicator at Digital PIN D4

int sense_Pin = D0; // Soil Sensor input at Analog PIN A0
int value = 0;


String apiKey = "******";
const char* ssid = "******";
const char* password = "********";

const char* server = "api.thingspeak.com";

WiFiClient client;


void setup() {
  Serial.begin(9600); 
  pinMode(2, INPUT);
  pinMode(A0, INPUT);
  Serial.println("DHTxx test!");
  pinMode(WET, OUTPUT);
  pinMode(DRY, OUTPUT);
  dht.begin();
   WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {

  int temp = analogRead(A0);
   nRainVal = analogRead(nRainIn);
   bIsRaining = !(digitalRead(nRainDigitalIn));
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("MOISTURE LEVEL : \n");
  value = analogRead(sense_Pin);
  Serial.println(value);
   if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(h);
           postStr +="&field2=";
           postStr += String(t);
           postStr +="&field3=";
           postStr += String(nRainVal);
           postStr +="&field4=";
           postStr += String(temp);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
  if (value < 500)
  {
    digitalWrite(WET, HIGH);
  }
  else
  {
    digitalWrite(DRY, HIGH);
  }
  
  digitalWrite(WET, LOW);
  digitalWrite(DRY, LOW);
  Serial.print("Moisture Sensor Value: \n");
  Serial.print(temp);

  

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    
    if (bIsRaining) {
      strRaining = "YES";
    }
    else {
      strRaining = "NO";
    }

    Serial.print("Raining?: \n");
    Serial.print(strRaining);
    Serial.print("\t Moisture Level: \n");
    Serial.println(nRainVal);

    delay(300);
  }

   }}
