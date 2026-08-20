#include "XYController.h"

// Define the stepper instances globally in this file
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP, X_DIR);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP, Y_DIR);

XYController::XYController() {}

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
    delay(200); // Give drivers a moment to wake up

    Serial.println(">>> Starting Homing: X Axis");
    homeAxis(stepperX, X_HOME, X_HOME_DIR, X_STEPS_PER_MM);
    
    Serial.println(">>> Starting Homing: Y Axis");
    homeAxis(stepperY, Y_HOME, Y_HOME_DIR, Y_STEPS_PER_MM);
    Serial.println("Homing sequence complete");
}

//NONBLOCKING METHOD - use this to multitask
void XYController::moveXY(float x_mm, float y_mm) {
    enableMotors();
    x_mm = constrain(x_mm, 0.0f, X_MAX_MM);
    y_mm = constrain(y_mm, 0.0f, Y_MAX_MM);
    
    stepperX.moveTo(lround(x_mm * X_STEPS_PER_MM));
    stepperY.moveTo(lround(y_mm * Y_STEPS_PER_MM));
}

void XYController::update() {
    stepperX.run();
    stepperY.run();
}

bool XYController::isBusy() {
    return (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0);
}

//BLOCKING METHOD - Keep this for when you WANT to block
void XYController::moveXYAndWait(float x_mm, float y_mm) {
    moveXY(x_mm, y_mm);
    while (isBusy()) {
        update();
        yield();
    }
}

float XYController::getX() { return stepperX.currentPosition() / X_STEPS_PER_MM; }
float XYController::getY() { return stepperY.currentPosition() / Y_STEPS_PER_MM; }

void XYController::homeAxis(AccelStepper& motor, int switchPin, int homeDirection, float stepsPerMm) {
    unsigned long startTime = millis();
    
    // 0. SAFETY: If already triggered, move away from switch first
    if (digitalRead(switchPin) == HIGH) {
        Serial.println("   - Switch already triggered, clearing...");
        motor.setSpeed(-homeDirection * HOMING_SLOW_SPEED);
        while (digitalRead(switchPin) == HIGH) {
            motor.runSpeed();
            yield();
            if (millis() - startTime > 3000) failSafe("Could not clear limit switch");
        }
        delay(200);
    }

    // 1. FAST SEEK
    Serial.println(" - Fast seeking...");
    motor.setSpeed(HOMING_FAST_SPEED * homeDirection); // Use constant speed for homing
    // Move while pin is LOW (NC switch connected to GND)
    while (digitalRead(switchPin) == LOW) { 
        motor.runSpeed();
        yield(); // Keep ESP32 watchdog happy
        if (millis() - startTime > HOMING_TIMEOUT_MS) {
            failSafe("FAST homing timeout");
            return;
        }
    }
    // Stop immediately
    motor.setSpeed(0);
    motor.setCurrentPosition(0);
    delay(200);

    // 2. BACK OFF
    Serial.println("   - Backing off...");
    motor.setSpeed(-homeDirection * HOMING_SLOW_SPEED);
    startTime = millis();
    // Move until switch is closed (LOW)
    while (digitalRead(switchPin) == HIGH) {
        motor.runSpeed();
        yield();
        if (millis() - startTime > 3000) failSafe("Backoff failed to release switch");
    }
    // Move a fixed extra distance to be safe
    motor.move(-homeDirection * (HOMING_BACKOFF_MM * stepsPerMm));
    while (motor.distanceToGo() != 0) { motor.run(); yield(); }
    delay(200);

    // 3. SLOW SEEK
    Serial.println(" - Slow seeking...");
    motor.setSpeed(HOMING_SLOW_SPEED * homeDirection);
    startTime = millis();        
    // Move while pin is LOW
    while (digitalRead(switchPin) == LOW) {
        motor.runSpeed();
        yield();
        if (millis() - startTime > HOMING_TIMEOUT_MS) {
            failSafe("SLOW homing timeout");
            return;
        }
    }
    
    motor.setSpeed(0);
    motor.setCurrentPosition(0); // This is now True Zero
    Serial.println(" - Axis homed successfully.");
}

void XYController::failSafe(const char* message) {
    disableMotors();
    Serial.print("!!! ERROR: ");
    Serial.println(message);
    while(1) { yield(); } // Lock up for safety
}
