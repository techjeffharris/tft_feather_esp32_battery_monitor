
/*

  Phoenix Rising Battery Monitor

  Sketch/Library for monitoring the discharge of Lead-acid batteries

  Created by Jeffrey A Harris, May 3, 2018
  Released into the public domain.

  Features: 
  * Voltage Divider
  * Digital Current Sensor
  * Temperature/Humidity sensor
  * Realtime Clock (RTC)
  * 8KB FRAM
  * 16x2 Character LCD
*/

#include "monitor.h"

PbMonitor monitor;

void setup() {

  Serial.begin(115200);

  // while (!Serial) delay(10);
  
  // start the monitor
  if (!monitor.begin()) {
    Serial.println("Error starting the monitor!!!");
  }
}


void loop(void) {

  monitor.checkForTouch();
  
}
