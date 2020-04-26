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
int btn = 32;
bool colourChanged = false;





int prevTime = 0;
int currentTime = millis();

Potentiometer xAxis (33); 
Potentiometer yAxis (34); 
Potentiometer zAxis (39); // Set to an analogue input

AsyncUDP udp;

void setup()
{
    Serial.begin(9600);
    pinMode(btn, INPUT_PULLUP);
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

    if(digitalRead(btn) == LOW && !colourChanged){
        colourChanged = true;
        Serial.println("change colour");
        udp.writeTo((const uint8_t*)"chClr", 5, yourip, outPort);
    } else if (digitalRead(btn) == HIGH && colourChanged) {
        colourChanged = false;
    }
    delay(0);
    xAxis.doStuff([](int val){
        if(val > 10) {
            Serial.println("Moving right");
           udp.writeTo((const uint8_t*)"moveR", 5, yourip, outPort);
        } else if (val < -10) {
            Serial.println("Moving left");
            udp.writeTo((const uint8_t*)"moveL", 5, yourip, outPort);
        }
    });

    yAxis.doStuff([](int val){

        if(val > 10) {
            Serial.println("Moving down");
           udp.writeTo((const uint8_t*)"moveD", 5, yourip, outPort);
        } else if (val < -10) {
            Serial.println("Moving up");
            udp.writeTo((const uint8_t*)"moveU", 5, yourip, outPort);
        } 
    });

    zAxis.doStuff([](int val){

        if(val > 10) {
            Serial.println("Moving foward");
           udp.writeTo((const uint8_t*)"moveF", 5, yourip, outPort);
        } else if (val < -10) {
            Serial.println("Moving backward");
            udp.writeTo((const uint8_t*)"moveB", 5, yourip, outPort);
        } 
    });
    udp.writeTo((const uint8_t*)"test", 4, yourip, outPort);
}