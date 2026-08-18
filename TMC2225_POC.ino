#include <AccelStepper.h>
#include <math.h>

// ================================================================
// PIN CONFIGURATION
// ================================================================

#define X_STEP      25
#define X_DIR       26
#define Y_STEP      32
#define Y_DIR       33
#define X_HOME      18
#define Y_HOME      19
#define STEPPER_EN  27

// ================================================================
// MACHINE SETTINGS -  Adjust for your mechanics
// ================================================================

const float X_STEPS_PER_MM = 80.0;
const float Y_STEPS_PER_MM = 80.0;
const float X_MAX_MM = 800.0;
const float Y_MAX_MM = 250.0;
const int MAX_SPEED = 3000;
const int ACCELERATION = 1500;
const int HOMING_FAST_SPEED = 800;
const int HOMING_SLOW_SPEED = 200;
const int HOMING_BACKOFF_STEPS = 100;
const unsigned long HOMING_TIMEOUT_MS = 10000;

// ================================================================
// STEPPER OBJECTS
// ================================================================

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP, X_DIR);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP, Y_DIR);

// ================================================================
// XY CONTROLLER
// ================================================================

class XYController
{
public:
    void begin()
    {
        pinMode(X_HOME, INPUT_PULLUP);
        pinMode(Y_HOME, INPUT_PULLUP);
        pinMode(STEPPER_EN, OUTPUT);
        disableMotors();
        stepperX.setMaxSpeed(MAX_SPEED);
        stepperX.setAcceleration(ACCELERATION);
        stepperY.setMaxSpeed(MAX_SPEED);
        stepperY.setAcceleration(ACCELERATION);
    }
    void enableMotors() {
        digitalWrite(STEPPER_EN, LOW);     // TMC2225 enable
    }
    void disableMotors() {
        digitalWrite(STEPPER_EN, HIGH);    // TMC2225 disable
    }
    void home()
    {
        enableMotors();
        Serial.println("Homing X...");
        homeAxis(stepperX, X_HOME, X_HOME_DIR);
        Serial.println("Homing Y...");
        homeAxis(stepperY, Y_HOME, Y_HOME_DIR);
        stepperX.setCurrentPosition(0);
        stepperY.setCurrentPosition(0);
        Serial.println("Homing complete");
    }
    void moveXY(float x_mm, float y_mm) {
        enableMotors();
        x_mm = constrain(x_mm, 0.0f, X_MAX_MM);
        y_mm = constrain(y_mm, 0.0f, Y_MAX_MM);
        stepperX.moveTo(lround(x_mm * X_STEPS_PER_MM));
        stepperY.moveTo(lround(y_mm * Y_STEPS_PER_MM));
    }

    // WAIT FOR COMPLETION

    void waitForMotion()
    {
        while (
            stepperX.distanceToGo() != 0 ||
            stepperY.distanceToGo() != 0)
        {
            stepperX.run();
            stepperY.run();
        }
    }

    // HIGH LEVEL API

    void moveXYAndWait(
        float x_mm,
        float y_mm)
    {
        moveXY(x_mm, y_mm);
        waitForMotion();
    }
    float getX()
    {
        return stepperX.currentPosition() / X_STEPS_PER_MM;
    }
    float getY()
    {
        return stepperY.currentPosition() / Y_STEPS_PER_MM;
    }

private:
    void homeAxis(AccelStepper& motor, int switchPin, int homeDirection) {

        unsigned long start;

        // ------------------------------------------------
        // FAST SEEK
        // ------------------------------------------------
        motor.setMaxSpeed(HOMING_FAST_SPEED);
        start = millis();
        while (digitalRead(switchPin) == LOW) {
            motor.move(homeDirection);
            motor.run();
            if (millis() - start > HOMING_TIMEOUT_MS)
                failSafe("Homing timeout");
        }
        delay(100);
        // ------------------------------------------------
        // BACK OFF
        // ------------------------------------------------
        motor.move(-homeDirection * HOMING_BACKOFF_STEPS);
        while (motor.distanceToGo() != 0)
            motor.run();
        delay(100);
        // ------------------------------------------------
        // SLOW SEEK
        // ------------------------------------------------
        motor.setMaxSpeed(HOMING_SLOW_SPEED);
        start = millis();
        while (digitalRead(switchPin) == LOW) {
            motor.move(homeDirection);
            motor.run();
            if (millis() - start > HOMING_TIMEOUT_MS)
                failSafe("Precision homing timeout");
        }
        motor.setCurrentPosition(0);
        motor.setMaxSpeed(MAX_SPEED);
    }

    // FAIL SAFE
    void failSafe(const char* message){
        Serial.println("************************");
        Serial.println("EMERGENCY STOP");
        Serial.println(message);
        Serial.println("************************");
    }
};

// ================================================================
// GLOBAL INSTANCE
// ================================================================

XYController xy;


// ================================================================
// SETUP
// ================================================================

void setup()
{
    Serial.begin(115200);

    xy.begin();
    xy.home();
    Serial.println("XY KINETICS Ready");
}

// ================================================================
// LOOP
// ================================================================

void loop()
{

//DEMO
    xy.moveXYAndWait(100, 50);
    delay(1000);
    xy.moveXYAndWait(50, 100);
    delay(1000);
    xy.moveXYAndWait(0, 0);

    Serial.print("X=");
    Serial.println(xy.getX());
    Serial.print("Y=");
    Serial.println(xy.getY());

    xy.disableMotors();
//DEMO

}
