#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <Servo_ESP32.h>

#include "DHT.h"
#define DHTTYPE DHT11
#define DHT_PIN 0
DHT dht(DHT_PIN, DHTTYPE);

//data
#define WIFI_SSID "wifi"
#define WIFI_PASSWORD "1212patrick"
#define DATABASE_URL "https://fir-mini-3e80b-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_SECRET "vl0pClHmxbX14Mc4cEKZKs2kls8985qMNmnX1nmF"


//servo
static const int servoPin = 14;
Servo_ESP32 servo1;

int angle =0;
int angleStep = 1;

int angleMin =0;
int angleMax = 180;

void servo(){
   for(int angle = 0; angle <= angleMax; angle +=angleStep) {
        servo1.write(angle);
        delay(20);
    }

    for(int angle = 180; angle >= angleMin; angle -=angleStep) {
        servo1.write(angle);
        delay(20);
    }
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(DATABASE_URL,DATABASE_SECRET);

  servo1.attach(servoPin);

}


void loop() {
  // put your main code here, to run repeatedly:
  int value = Firebase.getInt("Node1/dht");
  Serial.println(value);
  if(value == 0){
    Firebase.setFloat("Node1/suhu",0);
    Firebase.setString("Node1/status","Berhenti");
    Serial.println("Turning OFF");
    return;
  }else if(value == 1){
    float lembab = dht.readTemperature();
    if (isnan(lembab)){
      Serial.print("Failed to read from DHT sensor!");
      Serial.println(Firebase.error());
      return;
    }
    Serial.print("Cek kelembaban!");
    Firebase.setFloat("Node1/suhu",lembab);
    Firebase.setString("Node1/status","Cek suhu!");
    Serial.print("suhu: "+(String)lembab);
    if(lembab >= 30){
      Serial.println(" mejalankan servo !!");
      servo();
    }
  }
  
}
