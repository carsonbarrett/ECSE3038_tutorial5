#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "env.h"
#include <ArduinoJson.h>


#define LIGHT_PIN 26

void setup(){
 pinMode(LIGHT_PIN, OUTPUT);
 digitalWrite(LIGHT_PIN, LOW);

 Serial.begin(115200);

Serial.print("Connecting to WiFi");
WiFi.begin(SSID,PASS);
while (WiFi.status() != WL_CONNECTED) {
delay(100);
Serial.print(".");
   }
   Serial.println(" Connected!");
 }
 

void loop(){
    if (WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        http.begin(ENDPOINT);

        int responseCode = http.GET();

        if (responseCode <= 0){
            Serial.print("An error occured with code: ");
            Serial.println(responseCode);
            http.end();
            return;
        }

        Serial.print("HTTP Response Code: ");
        Serial.println(responseCode);

        String response = http.getString();
        Serial.println(response);
        http.end();

        JsonDocument object;

        DeserializationError error = deserializeJson(object, response);
        if (error){
            Serial.print("Deserializatiion failed:  ");
            Serial.println(error.c_str());
            return; 
        }

        float temperature = object["temperature"];
        float humidity = object["humidity"];
        float rainfall = object["rainfall"];
        const char* cloud = object["cloud"];
        
        Serial.print("Temperature: "); Serial.println(temperature);
        Serial.print("Humidity: "); Serial.println(humidity);
        Serial.print("Rainfall: "); Serial.println(rainfall);
        Serial.print("Cloud Condition: "); Serial.println(cloud);


        if (rainfall > 0) {
            digitalWrite(LIGHT_PIN, HIGH); 
            Serial.println("LED ON - Rain detected!");
        } else {
            digitalWrite(LIGHT_PIN, LOW); 
            Serial.println("LED OFF - No rain.");
        }
    }

}