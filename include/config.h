#pragma once

// PIN CONFIGURATION
#define X_STEP      25
#define X_DIR       26
#define Y_STEP      32
#define Y_DIR       33
#define X_HOME      18
#define Y_HOME      19
#define STEPPER_EN  27

// HOMING SETTINGS
// adjust if axis moves away from switch
#define X_HOME_DIR  (-1)
#define Y_HOME_DIR  (-1)


// MACHINE SETTINGS
constexpr float X_STEPS_PER_MM = 80.0f;
constexpr float Y_STEPS_PER_MM = 80.0f;
constexpr float X_MAX_MM = 800.0f;
constexpr float Y_MAX_MM = 250.0f;
constexpr float MAX_SPEED = 3000.0f;
constexpr float ACCELERATION = 1500.0f;
constexpr float HOMING_FAST_SPEED = 800.0f;
constexpr float HOMING_SLOW_SPEED = 200.0f;
constexpr int HOMING_BACKOFF_STEPS = 100;
constexpr unsigned long HOMING_TIMEOUT_MS = 10000;
