#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <OneButton.h>
#include "OneButton.h"

const char* ssid = ".UniFi";
const char* password = "lesenaopeka";

// const int errorPin = A0; // sense pin if no flame > 5s
OneButton button(A0);
const int preheat = 2;   // relay output for preheater
const int start = 3;     // relay output for main motor
const int greenled = 4;  // green led pin output
int analogerror;         // analog read for sense pin
bool error = false;     // assume no error on startup
int count;              // retry start count
void(* resetFunc) (void) = 0;
ESP8266WebServer server(80);



void Cstart() {
digitalWrite(preheat, LOW);
digitalWrite(start, HIGH);
Serial.println("preheat ON, start OFF");
if ((millis() > 1200*1000) && (error = false))
  {
  Serial.println("1200s elapsed without error");
  digitalWrite(start, LOW);
  digitalWrite(greenled, LOW);
  Serial.println("start ON");
  return;
  }
}
void retry()
  {
  Serial.println("retrying 5 times - 5seconds");
  if ((error = true) && (count < 5)){
  digitalWrite(start, HIGH);
  Serial.println("start OFF");
  delay(500);
  digitalWrite(start, LOW);
  Serial.println("start ON");
  delay(500);
  Serial.println(count, BIN);
  count++;
  return;
  }
}

void DCshutdown() {
  digitalWrite(preheat, HIGH);
  digitalWrite(start, HIGH);
  digitalWrite(greenled, HIGH);
  return;
}

void reset(){
resetFunc();  //call reset
}


void loop() {
  button.tick();
  delay(10);

  Cstart();
}

void setup() {
  Serial.begin(9600);
  pinMode(preheat, OUTPUT);
  pinMode(start, OUTPUT);
  pinMode(greenled, OUTPUT);
  button.attachClick(Cstart);
  button.attachDoubleClick(DCshutdown);

}