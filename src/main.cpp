#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "GimmeFi"; // put your own SSID here
const char * password = "1029384756"; // Put your own Wifi Password here
const IPAddress yourip(10,0,1,30); // Put your own drone/pixel emulator's IP here

int port = 7000;
int outPort = 7007;

int interval = 1000;
int prevTime = 0;
int currentTime = millis();

int xAxis = 33;
int yAxis = 34;
int newX;
int newY;
int prevX;
int prevY;
int joystkBTN = 32;
int tolerance = 20;

AsyncUDP udp;

void setup()
{
    Serial.begin(9600);

    pinMode(xAxis, INPUT);
    pinMode(yAxis, INPUT);
    pinMode(joystkBTN, INPUT_PULLUP);

    prevX = analogRead(xAxis);
    prevY = analogRead(yAxis);


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

    // Check for new x axis
    newX = map(analogRead(xAxis), 0, 4095, -100, 100);
    if (abs(newX-prevX) > tolerance)
    {
        prevX = newX;
    }

    // Check for new y axis
    newY = map(analogRead(yAxis), 0, 4095, -100, 100);
    if (abs(newY-prevY) > tolerance) 
    {
        prevY = newY;
    }

    // initialise new pixel on button press
    if(digitalRead(joystkBTN) == LOW) {
        Serial.println("clicked");
        udp.writeTo((const uint8_t*)"init 10 10", 10, yourip, port);
    }

    // check direction in x axis or stay still
    if (prevX == 100) 
    {  
        udp.writeTo((const uint8_t*)"moveright", 9, yourip, port);
    } else if (prevX == -100) {
        udp.writeTo((const uint8_t*)"moveleft", 8, yourip, port);
    } else {
       udp.writeTo((const uint8_t*)"stop", 8, yourip, port); 
    }
    
    // Check direction in y axis, or stay still
    if (prevY == 100) 
    {  
        udp.writeTo((const uint8_t*)"moveup", 6, yourip, port);
    } else if (prevY == -100) 
    {
        udp.writeTo((const uint8_t*)"movedown", 8, yourip, port);
    } else {
        udp.writeTo((const uint8_t*)"stop", 8, yourip, port);
    }

    //Send broadcast on port 4000
    udp.broadcastTo("Anyone here?", outPort);
}