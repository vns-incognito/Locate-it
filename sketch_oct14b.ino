#include <ESP8266WiFi.h>
//#include <FirebaseArduino.h>
#include <TinyGPS++.h>
#include "SoftwareSerial.h"
// Set these to run example.
#define FIREBASE_HOST "link"
#define FIREBASE_AUTH "link"
#define WIFI_SSID "wifi name"
#define WIFI_PASSWORD "wifi passr"
//GPS
SoftwareSerial serial_connection(4, 0); //tx,rx
TinyGPSPlus gps;// GPS object to process the NMEA data
float latt, lngg;
void setup() {
  Serial.begin(9600);
  serial_connection.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop() {
    while (serial_connection.available()) {
      gps.encode(serial_connection.read());
    }
    if (gps.location.isUpdated()) {    
      latt = gps.location.lat();
      lngg = gps.location.lng();
      Serial.println(latt, 6);
      Serial.println(lngg, 6);
      delay(1000);
    }   
    char location_str[25]={0};
    sprintf(location_str, "%f, %f", latt, lngg);
    String name = Firebase.push("location", location_str);
    if (Firebase.failed()) {
      Serial.print("setting /location failed:");
      Serial.println(Firebase.error());
      return;
      delay(1000);
    }
