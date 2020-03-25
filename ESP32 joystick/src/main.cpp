#include <Arduino.h>

int pinX = 33;
int pinY = 34;
int prevX;
int prevY;
int tolerance = 20;

void setup() {
  Serial.begin(9600);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(32, INPUT_PULLUP);
  prevX = analogRead(pinX);
  prevY = analogRead(pinY);  
}

void loop() {
  if(abs(analogRead(pinX)-prevX) > tolerance || abs(analogRead(pinY)-prevY) > tolerance){
    Serial.println(map(analogRead(pinX), 0, 4095, -100, 100));
    Serial.println(map(analogRead(pinY), 0, 4095, -100, 100));
    prevX = analogRead(pinX);
  }
  if(digitalRead(32) == LOW){
    Serial.println("Clicked");
  }
  delay(250);
}