#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include "potentiometer.h"

const char * ssid = "GimmeFi"; // put your own SSID here
const char * password = "1029384756"; // Put your own Wifi Password here
const IPAddress yourip(10,0,1,30); // Put your own drone/pixel emulator's IP here

int port = 7000;
int outPort = 4000;

int interval = 1000;
int prevTime = 0;
int currentTime = millis();

Potentiometer xAxis (33); 
Potentiometer yAxis (34); 
Potentiometer zAxis (0); // Set to an analogue input

AsyncUDP udp;

void setup()
{
    Serial.begin(9600);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }else{
        Serial.println("WiFi Connected");
    }
}


void loop() 
{
    delay(500);
    xAxis.doStuff([](int val){
        if(val > 10) {
            Serial.println("Moving right");
           udp.writeTo((const uint8_t*)"moveR", 5, yourip, port);
        } else if (val < -10) {
            Serial.println("Moving left");
            udp.writeTo((const uint8_t*)"moveL", 5, yourip, port);
        }
    });

    yAxis.doStuff([](int val){
        if(val > 10) {
            Serial.println("Moving forward");
           udp.writeTo((const uint8_t*)"moveF", 5, yourip, port);
        } else if (val < -10) {
            Serial.println("Moving barkwards");
            udp.writeTo((const uint8_t*)"moveB", 5, yourip, port);
        } 
    });

    zAxis.doStuff([](int val){
        if(val > 10) {
            Serial.println("Moving up");
           udp.writeTo((const uint8_t*)"moveU", 5, yourip, port);
        } else if (val < -10) {
            Serial.println("Moving down");
            udp.writeTo((const uint8_t*)"moveD", 5, yourip, port);
        } 
    });
    udp.writeTo((const uint8_t*)"test", 4, yourip, port)
}