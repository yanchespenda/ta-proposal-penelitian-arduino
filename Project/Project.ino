#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ESP8266HTTPClient.h"

const int sensor_hujan = 16;
const int sensor_hujanA = A0;
const int LED = 5;
//const int RELAY = 4;

const char *ssid = "aws_gcp";
const char *password = "Q123457698";
//const char *link = "http://192.168.2.51/api/kirim";
const char *link = "http://192.168.42.154/api/kirim";

boolean is_conected = false;

void setup() {
  //  Serial.begin(9600);
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode (sensor_hujan, INPUT);
  pinMode (sensor_hujanA, INPUT);
  pinMode (LED, OUTPUT);
//  pinMode (RELAY, OUTPUT);

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  is_conected = true;
  while (WiFi.status() != WL_CONNECTED) {
    is_conected = false;
   delay(500);
   Serial.print(" Failed"); 
  }

  if(is_conected == true){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
  int kondisi_sensor = digitalRead(sensor_hujan);
  float kondisi_sensorA = analogRead(sensor_hujanA);
  //Serial.println("D: " + kondisi_sensor);
  Serial.print("Digital: ");
  Serial.println(kondisi_sensor);
  Serial.print("Analog: ");
  Serial.println(kondisi_sensorA);
  Serial.println("");
  if (kondisi_sensor == LOW){
    digitalWrite(LED, HIGH);
//    digitalWrite(RELAY, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
//    digitalWrite(RELAY, LOW);
  }

  HTTPClient http;

  String  ADCData = String(kondisi_sensorA),
          D0Data = String(kondisi_sensor),
          postData = "analog=" + ADCData + "&digital=" +  D0Data;

  http.begin(link);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData); //kirim request
  String payload = http.getString(); //respon dari php

  Serial.print("PostData: ");
  Serial.println(postData); //Print HTTP return code

  Serial.print("HttpCode: ");
  Serial.println(httpCode); //Print HTTP return code

  Serial.print("Payload: ");
  Serial.println(payload); //Print request response payload 
  http.end(); //Tutup Koneksi

  Serial.println("");
//  int n = WiFi.scanNetworks();
//  if (n == 0) {
//    Serial.println("no networks found");
//  } else {
//    Serial.print(n);
//    Serial.println(" networks found");
//    for (int i = 0; i < n; ++i) {
//      // Print SSID and RSSI for each network found
//      Serial.print(i + 1);
//      Serial.print(": ");
//      Serial.print(WiFi.SSID(i));
//      Serial.print(" (");
//      Serial.print(WiFi.RSSI(i));
//      Serial.print(")");
//      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
//    }
//  }
//  Serial.println("================================");
//  Serial.println("");
  
  delay (5000);
}
