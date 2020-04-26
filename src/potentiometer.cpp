#include "potentiometer.h"

Potentiometer::Potentiometer(int pin)
{
    this->pin = pin;
    pinMode(this->pin, INPUT);
    // Initial prevVal as whatever the potentiometer is on init, but mapped from -100 to +100
    this->prevVal = map(analogRead(this->pin), 0, 4095, -100, 100);
}

void Potentiometer::doStuff(std::function<void(int)> callback)
{
    // Protection against slight changes in potentiometer value
    this->newVal = map(analogRead(this->pin), 0, 4095, -100, 100);
    if (abs(this->newVal - this->prevVal) > this->tolerance)
    {
        // Sets potentiometer value if it's high enough
        prevVal = newVal;
    }
    // Call the passed in value
    callback(prevVal);
}