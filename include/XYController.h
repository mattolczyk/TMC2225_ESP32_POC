#pragma once
#include <AccelStepper.h>
#include "Config.h"

class XYController {
public:
    XYController();
    void begin();
    void enableMotors();
    void disableMotors();
    void home();


    //BLOCKING METHOD - can be used with delay
    void moveXYAndWait(float x_mm, float y_mm);
    float getX();
    float getY();

    //NONBLOCKING Methods
    void moveXY(float x_mm, float y_mm); // Start the move and return immediately
    void update();                       // Must be called in loop() constantly
    bool isBusy();                       // Returns true if still moving

private:
    void homeAxis(AccelStepper& motor, int switchPin, int homeDirection, float stepsPerMm);
    void failSafe(const char* message);
};
