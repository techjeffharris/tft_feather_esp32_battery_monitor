/*
  voltmeter.cpp - Voltmeter class for monitoring the discharge current of Lead-acid batteries.
  Created by Jeffrey A Harris, July 1, 2017
  Released into the public domain.
*/
#include "Arduino.h"
#include "voltmeter.h"
#include <ResponsiveAnalogRead.h>

float Voltmeter::getThreshold()
{
  return _act_thresh;
}

float Voltmeter::getSnapMulti()
{
  return _snapmulti;
}

void Voltmeter::setThreshold()
{
  _act_thresh = ACT_THRESH;
  _meter.setActivityThreshold(_act_thresh);
}

float Voltmeter::readVolts()
{
  _meter.update();

  float sensor_mv = adcMilliVolts(_meter.getValue());

  float millivolts = (sensor_mv * (R1 + R2)/R2);

  return millivolts / 1000;
}

float Voltmeter::adcMilliVolts(int adc_val) { 
  return (adc_val / 1024.0) * VCC;
}

