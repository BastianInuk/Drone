#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "Your wifi name WITHOUT space";
const char * password = "Your wifi password";

int port = 7000;
int outPort = 7007;

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
    if(udp.listen(4000)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());

        });
    }



    }


void loop() {
    delay(1250);

    if(abs(analogRead(xAxis)-prevX) > tolerance || abs(analogRead(yAxis)-prevY) > tolerance){
    Serial.println(map(analogRead(xAxis), 0, 4095, -100, 100));
    Serial.println(map(analogRead(yAxis), 0, 4095, -100, 100));
    //prevX = analogRead(xAxis);
    }
    if(digitalRead(joystkBTN) == LOW){
        Serial.println("clicked");
        udp.writeTo((const uint8_t*)"init 10 10", 10, IPAddress(192,168,43,134), port);
    }
    if(analogRead(xAxis) == HIGH) {
        udp.writeTo((const uint8_t*)"moveright", 9, IPAddress(192,168,43,134), port);
    }   
    if(analogRead(yAxis) == HIGH) {
        udp.writeTo((const uint8_t*)"moveup", 6, IPAddress(192,168,43,134), port);
    } 
    if(analogRead(xAxis) == LOW) {
        udp.writeTo((const uint8_t*)"moveleft", 8, IPAddress(192,168,43,134), port);
    } 
    if(analogRead(yAxis) == LOW) {
        udp.writeTo((const uint8_t*)"movedown", 8, IPAddress(192,168,43,134), port);
    } 
    //Send broadcast on port 4000
    udp.broadcastTo("Anyone here?", outPort);
    //Serial.println("waiting for udp message...");
    udp.writeTo((const uint8_t*)"moveup", 6, IPAddress(192,168,43,134), port);
    //udp.writeTo()

}//arduin