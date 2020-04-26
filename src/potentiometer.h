#include <Arduino.h>
#include <functional>

/** Wrapper class around Potentiometers */
class Potentiometer {
private:
    int pin; // Pin number for potentiometer
    int newVal; // The new uncertain value, in hindseight, this could be initialised in `doStuff`
    int prevVal; // The value, it's far enough away from potentiometers actual value, to be considered safe from jitter
    int tolerance = 20; // This should probably be in the constructor, but as it's the same for all potentiometers, it's ok. This tolerence is to tell the system how far away from the "safe value" the actual one should be to avoid jitter.
public:
    Potentiometer(int pin);
    void doStuff(std::function<void(int)> callback); // This function evaluates the passed in function, and passes in the potentiometer value.
};