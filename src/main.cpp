#include <Arduino.h>
#include "XYController.h"


// GLOBAL INSTANCE - MUST HAVE !!!!
XYController xy;

void setup()
{
    Serial.begin(115200);
    xy.begin();
    delay(1000);
    xy.home();
    Serial.println("XY KINETICS READY");
}

void loop()
{
//DEMO
    xy.moveXYAndWait(100, 50);
    delay(1000);
    xy.moveXYAndWait(50, 100);
    delay(1000);
    xy.moveXYAndWait(0, 0);
    Serial.printf(
        "X=%.2f mm  Y=%.2f mm\n",
        xy.getX(),
        xy.getY());
    xy.disableMotors();
    delay(5000);
    xy.home();  
//DEMO
}
