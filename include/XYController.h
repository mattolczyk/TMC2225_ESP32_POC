#pragma once

#include <Arduino.h>
#include <AccelStepper.h>

class XYController
{
public:
    XYController();

    void begin();
    void enableMotors();
    void disableMotors();
    void home();
    void moveXY(float x_mm, float y_mm);
    void moveXYAndWait(float x_mm, float y_mm);
    void waitForMotion();
    float getX();
    float getY();

private:
    void homeAxis(
        AccelStepper& motor,
        int switchPin,
        int homeDirection);

    [[noreturn]] void failSafe(const char* message);

    AccelStepper stepperX;
    AccelStepper stepperY;
};
