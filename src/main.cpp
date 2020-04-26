#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include "potentiometer.h"

// Networking values here
const char * ssid = "GimmeFi"; // put your own SSID here
const char * password = "1029384756"; // Put your own Wifi Password here
const IPAddress yourip(192,168,43,134); // Put your own drone/pixel emulator's IP here

int port = 7000;
int outPort = 4000;

// Button for changing color
int btn = 32;
bool colourChanged = false;

// Potentiometers
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
    
    // Chekc for if WiFi failed and hault the program if WiFi did fail
    if (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
        Serial.println("WiFi Failed");
        // This is an infinite loop, which prevents the loop program from working
        while(1) {
            delay(1000);
        }
    }else{
        // If WiFi succeeded, tell the user so they don't just wait around
        Serial.println("WiFi Connected");
    }
}


void loop() 
{
    // Protection, sends only 1 command no matter how long the user presses
    if(digitalRead(btn) == LOW && !colourChanged){
        colourChanged = true;
        Serial.println("change colour");
        udp.writeTo((const uint8_t*)"chClr", 5, yourip, outPort);
        // Flip the switch, otherwise the button will only ever work once
    } else if (digitalRead(btn) == HIGH && colourChanged) {
        colourChanged = false;
    }
    
    // Potentiometers sends command here, there's a deadzone of 10% of each potentiometer, so the drone can stand still
    xAxis.doStuff([](int val){
        if(val > 10) { // check for deadzone
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
}