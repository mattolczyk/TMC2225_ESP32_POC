#include "XYController.h"
#include "Config.h"
#include <math.h>

XYController::XYController()
    : stepperX(AccelStepper::DRIVER, X_STEP, X_DIR),
      stepperY(AccelStepper::DRIVER, Y_STEP, Y_DIR) {}

void XYController::begin() {
    pinMode(X_HOME, INPUT_PULLUP);
    pinMode(Y_HOME, INPUT_PULLUP);
    pinMode(STEPPER_EN, OUTPUT);

    disableMotors();
    stepperX.setMaxSpeed(MAX_SPEED);
    stepperX.setAcceleration(ACCELERATION);
    stepperY.setMaxSpeed(MAX_SPEED);
    stepperY.setAcceleration(ACCELERATION);
}

void XYController::enableMotors() { digitalWrite(STEPPER_EN, LOW); }
void XYController::disableMotors() { digitalWrite(STEPPER_EN, HIGH); }

void XYController::home() {
    enableMotors();
    Serial.println("Homing X...");
    homeAxis(stepperX, X_HOME, X_HOME_DIR);
    Serial.println("Homing Y...");
    homeAxis(stepperY, Y_HOME, Y_HOME_DIR);
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    Serial.println("Homing complete");
}

void XYController::run() {
    stepperX.run();
    stepperY.run();
}

bool XYController::isMoving() {
    return stepperX.distanceToGo() ||
           stepperY.distanceToGo();
}

void XYController::moveXY(float x_mm, float y_mm) {
    enableMotors();
    x_mm = constrain(x_mm, 0.0f, X_MAX_MM);
    y_mm = constrain(y_mm, 0.0f, Y_MAX_MM);
    stepperX.moveTo(lround(x_mm * X_STEPS_PER_MM));
    stepperY.moveTo(lround(y_mm * Y_STEPS_PER_MM));
}

bool XYController::isIdle() {
    return !isMoving();
}


float XYController::getX() { return stepperX.currentPosition() / X_STEPS_PER_MM; }
float XYController::getY() { return stepperY.currentPosition() / Y_STEPS_PER_MM; }

void XYController::homeAxis(AccelStepper& motor, int switchPin, int homeDirection) {
    unsigned long start = millis();
    
    motor.setMaxSpeed(HOMING_FAST_SPEED);    
    motor.moveTo(homeDirection * 1000000L);
    while (digitalRead(switchPin) == HIGH) {
        motor.run();
        yield();

        if (millis() - start > HOMING_TIMEOUT_MS)
            failSafe("Homing timeout");
    }
    delay(100);
    
    motor.move(-homeDirection * HOMING_BACKOFF_STEPS);
    while (motor.distanceToGo()) {
        motor.run();
        yield();
    }
    delay(100);

    motor.setMaxSpeed(HOMING_SLOW_SPEED);
    start = millis();

    motor.moveTo(homeDirection * 1000000L);
    while (digitalRead(switchPin) == HIGH) {
        motor.run();
        yield();
        
        if (millis() - start > HOMING_TIMEOUT_MS)
            failSafe("Precision homing timeout");
    }

    motor.setCurrentPosition(0);
    motor.moveTo(0);
    motor.setMaxSpeed(MAX_SPEED);
}

[[noreturn]]
void XYController::failSafe(const char* message) {
    Serial.println("************************");
    Serial.println("EMERGENCY STOP");
    Serial.println(message);
    Serial.println("************************");
    disableMotors();
    while (true)
        delay(1000);
}
