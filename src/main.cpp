#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include "potentiometer.h"

const char * ssid = "OneplusOne"; // put your own SSID here
const char * password = "inhumanddragon"; // Put your own Wifi Password here
const IPAddress yourip(192,168,43,134); // Put your own drone/pixel emulator's IP here

int port = 7000;
int outPort = 4000;

int interval = 1000;
int prevTime = 0;
int currentTime = millis();

Potentiometer xAxis (33); 
Potentiometer yAxis (34); 
Potentiometer zAxis (39); // Set to an analogue input

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
    }
}


void loop() 
{
    xAxis.doStuff([](int val){

        if(val > 10) {
           udp.writeTo((const uint8_t*)"moveR", 9, yourip, port);
        } else if (val < -10) {
            udp.writeTo((const uint8_t*)"moveL", 8, yourip, port);
        }
    });

    yAxis.doStuff([](int val){

        if(val > 10) {
           udp.writeTo((const uint8_t*)"moveF", 9, yourip, port);
        } else if (val < -10) {
            udp.writeTo((const uint8_t*)"moveB", 8, yourip, port);
        } 
    });

    zAxis.doStuff([](int val){

        if(val > 10) {
           udp.writeTo((const uint8_t*)"moveU", 9, yourip, port);
        } else if (val < -10) {
            udp.writeTo((const uint8_t*)"moveD", 8, yourip, port);
        } 
    });

}