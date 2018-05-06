/*
  Ammeter.h - Ammeter class for monitoring the discharge current of Lead-acid batteries.
  Created by Jeffrey A Harris, July 1, 2017
  Released into the public domain.
*/
#include "Arduino.h"
#include "ResponsiveAnalogRead.h"
#include "ammeter.h"
#include "acs712.h"

float Ammeter::getThreshold()
{
  return _act_thresh;
}

float Ammeter::getSnapMulti()
{
  return _snapmulti;
}

void Ammeter::setThreshold()
{
  _act_thresh = ACT_THRESH;
  _meter.setActivityThreshold(_act_thresh);
}

float Ammeter::readAmperes()
{
  float milliamperes;

  _meter.update();

  milliamperes = (_meter.getValue() - _acs712.getMidpoint()) * _acs712.getMultiplier() / 1023;

  // derived from Julian Illet
  return milliamperes / 1000;
}
