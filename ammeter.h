
/*
  Ammeter.h - Ammeter class for monitoring the discharge current of Lead-acid batteries.
  Created by Jeffrey A Harris, July 1, 2017
  Released into the public domain.
*/
#ifndef Ammeter_h
#define Ammeter_h

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include "acs712.h"
#include "constants.h"

class Ammeter
{
  public:
    Ammeter() 
      : _pin(AMMETER_PIN),
        _sleep(SLEEP_MODE),
        _snapmulti(SNAPMULTI),
        _meter(_pin, _sleep, _snapmulti)
    {};
    float getThreshold();
    float getSnapMulti();
    void setThreshold();
    float readAmperes();
  private:
    int   _pin;
    float _act_thresh;
    bool  _sleep;
    float _snapmulti;
    ACS712 _acs712;

    ResponsiveAnalogRead _meter;
};

#endif
