#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT11

DHT dht(D7, DHTTYPE);
const int relay=D6;

void setup() {
  pinMode(relay,OUTPUT);
  Serial.begin(115200);
  Serial.println("Tes Suhu");
  dht.begin();
}

void loop() {
delay(2000);
  float t = dht.readTemperature(); //suhu
  float h = dht.readHumidity(); //kelembaban

  if (isnan(t) || isnan(h)) {
      Serial.println("Periksa konfigurasi pin/kabelnya");
      Serial.println("Sensor tidak terbaca");
      return;
  }
  if (t>=28){
      digitalWrite(relay ,HIGH);//eksekusi
  }
    
  else {
    digitalWrite (relay,LOW);
  }
  Serial.print("Suhu : ");
  Serial.print(t);
}