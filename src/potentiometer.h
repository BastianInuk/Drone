#include <Arduino.h>
#include <functional>

/** Wrapper class around Potentiometers */
class Potentiometer {
private:
    int pin;
    int newVal;
    int prevVal;
    int tolerance = 20;
public:
    Potentiometer(int pin);
    void doStuff(std::function<void(int)> callback);
};