#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define DHTTYPE DHT11
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

const long utcOffsetInSeconds = 25200;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

const char *ssid = "OPPO A3s";  
const char *password = "IWAKKADAL";

const char *mqtt_server = "ec2-3-83-145-199.compute-1.amazonaws.com";

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(D7, DHTTYPE);
const int relay=D6;

long now = millis();
long lastMeasure = 0;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() {
  pinMode(relay,OUTPUT);
  Serial.begin(115200);
  Serial.println("Tes Suhu");
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
  {
    client.connect("ESP8266Client");
  }
  now = millis();
  if (now - lastMeasure > 5000)
  {
    lastMeasure = now;
    int err = SimpleDHTErrSuccess;

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

    client.publish("")
}