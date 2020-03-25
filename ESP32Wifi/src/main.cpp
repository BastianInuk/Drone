#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "Ore no Wifi";
const char * password = "302831298";

int port = 7000;
int outPort = 7007;

AsyncUDP udp;

void setup()
{
    Serial.begin(9600);
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
        /*    //Serial.print("state: ");
            //Serial.write(packet.data(), packet.length());
             String s((char*)packet.data());
            s = s.substring(0, packet.length()); 
            s.trim();
            // send string to method
            Serial.println(s);
            udp.printf("Received your message!");
        */
        });
    }

        //Send unicast
        //udp.print("Hello Server!");
        //udp.
        
    }


void loop()
{   
    delay(5000);
    //Send broadcast on port 4000
    udp.broadcastTo("Anyone here?", 4000);
    //Serial.println("waiting for udp message...");
    udp.writeTo((const uint8_t*)"Hej med dig", 11, IPAddress(192,168,1,2), 4000);
    //udp.writeTo()

}//arduin