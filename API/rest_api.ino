#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "time.h"


//lamp

#define led_dingin 2
#define led_sedang 0
#define led_panas 4



const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;
const int suhu=0;

const char* ssid = "kocan";
const char* password = "gapakepasswort";

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "b452e770d3c17519334e8aed4c8fd218";
// Example:
//String openWeatherMapApiKey = "bd939aa3d23ff33d3c8f5dd1dd435";

// Replace with your country code and city
String city = "jakarta";
String countryCode = "jkt";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
  pinMode(led_dingin, OUTPUT);
  pinMode(led_sedang, OUTPUT);
  pinMode(led_panas, OUTPUT);
}

void loop() {
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
//      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//      Serial.print("JSON object = ");
//      Serial.println(myObject);
      Serial.print("Temperature: ");
      int suhu = (int)myObject["main"]["temp"] ;
      suhu = suhu - 273,15;
      Serial.print(suhu);
      Serial.println("c");
      printLocalTime();
      lamp();   
//      Serial.print("Pressure: ");
//      Serial.println(myObject["main"]["pressure"]);
//      Serial.print("Humidity: ");
//      Serial.println(myObject["main"]["humidity"]);
//      Serial.print("Wind Speed: ");
//      Serial.println(myObject["wind"]["speed"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
//  Serial.print("Day of week: ");
//  Serial.println(&timeinfo, "%A");
//  Serial.print("Month: ");
//  Serial.println(&timeinfo, "%B");
//  Serial.print("Day of Month: ");
//  Serial.println(&timeinfo, "%d");
//  Serial.print("Year: ");
//  Serial.println(&timeinfo, "%Y");
//  Serial.print("Hour: ");
//  Serial.println(&timeinfo, "%H");
//  Serial.print("Hour (12 hour format): ");
//  Serial.println(&timeinfo, "%I");
//  Serial.print("Minute: ");
//  Serial.println(&timeinfo, "%M");
//  Serial.print("Second: ");
//  Serial.println(&timeinfo, "%S");
//
//  Serial.println("Time variables");
//  char timeHour[3];
//  strftime(timeHour,3, "%H", &timeinfo);
//  Serial.println(timeHour);
//  char timeWeekDay[10];
//  strftime(timeWeekDay,10, "%A", &timeinfo);
//  Serial.println(timeWeekDay);
//  Serial.println();
}

void lamp(){
      if(suhu <= 30){
         Serial.println("ac hangat");
         Serial.println("----------");
         digitalWrite(led_dingin, HIGH);
         delay(2000);
         digitalWrite(led_dingin, LOW);
       }else if (30 < suhu <= 39){
         Serial.println("ac sedang");
         Serial.println("----------");
         digitalWrite(led_sedang, HIGH);
         delay(2000);
         digitalWrite(led_sedang,LOW);
      }else{
         Serial.println("ac dingin");
         Serial.println("----------");
         digitalWrite(led_panas, HIGH);
         delay(2000);
         digitalWrite(led_panas, LOW);
      };
}
