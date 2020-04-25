#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include "potentiometer.h"

const char * ssid = "GimmeFi"; // put your own SSID here
const char * password = "1029384756"; // Put your own Wifi Password here
const IPAddress yourip(10,0,1,30); // Put your own drone/pixel emulator's IP here

int port = 7000;
int outPort = 7007;

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
    }
}


void loop() 
{
    delay(100);

    xAxis.doStuff([](int val){
        Serial.write("The X val is " + val + '\n');
        if(val > 10) {
           udp.writeTo((const uint8_t*)"moveR", 9, yourip, port);
        } else if (val < -10) {
            udp.writeTo((const uint8_t*)"moveL", 8, yourip, port);
        }
    });

    yAxis.doStuff([](int val){
        Serial.write("The Y val is " + val + '\n');
        if(val > 10) {
           udp.writeTo((const uint8_t*)"moveF", 9, yourip, port);
        } else if (val < -10) {
            udp.writeTo((const uint8_t*)"moveB", 8, yourip, port);
        } 
    });

    zAxis.doStuff([](int val){
        Serial.write("The Z val is " + val + '\n');
        if(val > 10) {
           udp.writeTo((const uint8_t*)"moveU", 9, yourip, port);
        } else if (val < -10) {
            udp.writeTo((const uint8_t*)"moveD", 8, yourip, port);
        } 
    });

}