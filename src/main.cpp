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
xy.run();
}
