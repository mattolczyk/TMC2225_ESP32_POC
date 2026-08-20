
#include <Arduino.h>
#include "XYController.h"

XYController devmatxy;

void setup() {
    // 1. Use 115200 - it is the ESP32 standard
    Serial.begin(115200);
    // ESP32-S3 Native USB can be slow to initialize
    unsigned long serialStart = millis();
    while (!Serial && (millis() - serialStart < 3000)); 

    delay(3000); 
    
    Serial.println("\n==============================");
    Serial.println("... SYSTEM BOOTING ...");
    Serial.println("==============================");

    Serial.println("Initializing Pins and Steppers...");
    devmatxy.begin();
    Serial.println("Pins and Steppers Initialized.");

    Serial.println("Attempting to Enable Motors...");
    devmatxy.enableMotors(); 
    Serial.println("Motors Enabled.");

    Serial.println("Starting Homing sequence...");
    devmatxy.home();
    Serial.println("Homing Complete. System Ready.");
}

void loop() {

    // CRITICAL: This must run every single loop iteration to provide the timing pulses for the motors.
    devmatxy.update();

    
    // Now you can do other things!
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 500) {
        if (devmatxy.isBusy()) {
            Serial.printf("Moving... Current X: %.2f\n", devmatxy.getX());
        } else {
            Serial.println("Destination reached!");
        }
        lastPrint = millis();
    }

}
