# XYKinetics

Simple XY motion controller for **ESP32** using **TMC2225 stepper drivers**, **AccelStepper**, and **normally-closed homing switches**.

The project provides:

- Two-axis motion control (X/Y)
- Independent homing for each axis
- Soft limits
- Acceleration and speed control
- Enable/disable driver support
- Blocking movement API (`moveXYAndWait()`)
- Startup safety checks
- PlatformIO project structure

---

## Hardware

### Controller
- ESP32
### Drivers
- 2 × TMC2225
- STEP/DIR mode
### Motors
- 2 × NEMA17 stepper motors
### Sensors
- 2 × Normally Closed (NC) limit switches
  - Home X
  - Home Y

### Power
- 24V power supply
- Current rating according to motor requirements

---

## Project Structure

```text
XYKinetics/
│
├── include/
│   ├── Config.h
│   └── XYController.h
│
├── src/
│   ├── main.cpp
│   └── XYController.cpp
│
└── platformio.ini
```

---

## File Responsibilities

### Config.h

Contains all hardware configuration:

- GPIO definitions
- Motion settings
- Speed limits
- Acceleration limits
- Travel ranges
- Homing parameters

Example:

```cpp
#define X_STEP_PIN 26
#define X_DIR_PIN 27

#define Y_STEP_PIN 14
#define Y_DIR_PIN 12

#define X_HOME_PIN 33
#define Y_HOME_PIN 32

#define EN_PIN 25
```

---

### XYController.h

Public interface of the motion controller.

Responsibilities:

- Motion commands
- Homing functions
- Position tracking
- Limit checking
- Driver enable control

---

### XYController.cpp

Implementation of:

- Homing sequence
- Motion execution
- Safety checks
- Soft limits
- Blocking movement logic

---

### main.cpp

Application entry point.

Typical workflow:

```cpp
xy.startupSafetyCheck();
xy.homeAll();

xy.moveXYAndWait(100, 50);
xy.moveXYAndWait(250, 100);
```

---

## Features

### Homing

At startup:

1. Verify home switches are operational.
2. Enable drivers.
3. Home X axis.
4. Home Y axis.
5. Zero both positions.

---

### Soft Limits

Movement outside configured work area is prevented.

Example:

```cpp
X_MIN = 0
X_MAX = 300

Y_MIN = 0
Y_MAX = 200
```

Any command beyond these limits is rejected before motion starts.

---

### Blocking Motion API

Move and wait until both axes finish:

```cpp
xy.moveXYAndWait(100, 50);
```

The function returns only after both motors stop.

---

### Driver Enable Control

Enable:

```cpp
xy.enableMotors();
```

Disable:

```cpp
xy.disableMotors();
```

Useful for:

- Emergency stop handling
- Low power operation
- Manual axis movement

---

## Homing Switch Logic

This project assumes **Normally Closed (NC)** switches.

Advantages:

- Broken wire detection
- Better fail-safe behavior
- Common industrial approach

Switch state:

| State | Signal |
|---------|----------|
| Normal | CLOSED |
| Triggered | OPEN |
| Broken cable | OPEN |

---

## Building

### Requirements

- Visual Studio Code
- PlatformIO extension
- ESP32 board package

### Build

```bash
pio run
```

### Upload

```bash
pio run --target upload
```

### Monitor

```bash
pio device monitor
```

---

## Example

```cpp
void setup() {
    xy.startupSafetyCheck();

    xy.enableMotors();

    xy.homeAll();

    xy.moveXYAndWait(100, 50);

    xy.moveXYAndWait(250, 100);

    xy.disableMotors();
}

void loop() {
}
```

---

## Future Improvements

- G-code interpreter
- Non-blocking motion queue
- Coordinated linear interpolation
- EEPROM position storage
- Web interface
- Wi-Fi control
- MQTT integration
- Joystick support
- End-stop protection on maximum travel

---

## License

MIT License

Feel free to use, modify, and distribute.
