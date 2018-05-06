
/*
  voltmeter.h - Voltmeter class for monitoring the discharge current of Lead-acid batteries.
  Created by Jeffrey A Harris, July 1, 2017
  Released into the public domain.
*/
#ifndef Voltmeter_h
#define Voltmeter_h

#include "Arduino.h"
#include <ResponsiveAnalogRead.h>

#include "constants.h"

class Voltmeter
{
  public:
    Voltmeter() 
      : _sleep(SLEEP_MODE),
        _snapmulti(SNAPMULTI),
        _pin(VOLTMETER_PIN),
        _meter(_pin, _sleep, _snapmulti)
    {};
    
    float getThreshold();
    float getSnapMulti();

    void setThreshold();
    
    float readVolts();

  private:
    int _pin;
    int _r1;
    int _r2;

    float _act_thresh;
    float _snapmulti;
    float _sleep;

    ResponsiveAnalogRead _meter;

    float adcMilliVolts(int adc_val);
};

#endif
