#ifndef power_h
#define power_h
#include <Arduino.h>
// code goes here
class Power {
    public:
        Power();
        void setPower(int power);
        int getPower();
        void setPowerState(bool state);
        bool getPowerState();
    private:
        int power;
        bool powerState;
};

#endif