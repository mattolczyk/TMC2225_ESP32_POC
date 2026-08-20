#pragma once
#include <Arduino.h>

//"Safe" Pins for ESP32-S3 DevKitC-1:
//GPIO 1, 2, 4, 5, 6, 7 GPIO 15, 16, 17, 18 GPIO 38, 39, 40, 41, 42
// PIN CONFIGURATION (ESP32-S3)
#define X_STEP      15
#define X_DIR       16
#define Y_STEP      17
#define Y_DIR       18
#define X_HOME      13
#define Y_HOME      14
#define STEPPER_EN  5

// MACHINE SETTINGS
const float X_STEPS_PER_MM = 80.0;
const float Y_STEPS_PER_MM = 80.0;
const float X_MAX_MM       = 800.0;
const float Y_MAX_MM       = 250.0;

// MOTION PROFILES
const int MAX_SPEED          = 3000;
const int ACCELERATION       = 1500;

// HOMING PARAMETERS
#define X_HOME_DIR          -1 
#define Y_HOME_DIR          -1 
const int HOMING_FAST_SPEED      = 800;
const int HOMING_SLOW_SPEED      = 200;
const int HOMING_BACKOFF_MM      = 5;  
const unsigned long HOMING_TIMEOUT_MS = 10000;
