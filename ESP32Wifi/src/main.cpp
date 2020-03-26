#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "yourwifinamewithoutspaces";
const char * password = "wifipassword";

int port = 7000;
int outPort = 7007;

int interval = 1000;
int prevTime = 0;
int currentTime = millis();

int xAxis = 33;
int yAxis = 34;
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
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
}


void loop() {
    delay(1250);

    if(abs(analogRead(xAxis)-prevX) > tolerance || abs(analogRead(yAxis)-prevY) > tolerance){
    Serial.println(map(analogRead(xAxis), 0, 4095, -100, 100));
    Serial.println(map(analogRead(yAxis), 0, 4095, -100, 100));
    //prevX = analogRead(xAxis);
    }

    if(digitalRead(joystkBTN) == LOW) {
        Serial.println("clicked");
        udp.writeTo((const uint8_t*)"init 10 10", 10, IPAddress(youripaddress), port);
    }

    if(currentTime - prevTime > interval) {
         if (analogRead(xAxis) == 100) {  
            udp.writeTo((const uint8_t*)"moveright", 9, IPAddress(youripaddress), port);
        } else if (analogRead(yAxis) == 100) {  
            udp.writeTo((const uint8_t*)"moveup", 6, IPAddress(youripaddress), port);
        } else if (analogRead(xAxis) == -100) {
            udp.writeTo((const uint8_t*)"moveleft", 8, IPAddress(youripaddress), port);
        } else if (analogRead(yAxis) == -100) {
            udp.writeTo((const uint8_t*)"movedown", 8, IPAddress(youripaddress), port);
        } 
    }
    //Send broadcast on port 4000
    udp.broadcastTo("Anyone here?", outPort);
    //Serial.println("waiting for udp message...");
    //udp.writeTo((const uint8_t*)"moveup", 6, IPAddress(youripaddress), port);
    //udp.writeTo()

}