
/*
  ACS712.h - Class for monitoring the discharge current of Lead-acid batteries.
  Created by Jeffrey A Harris, July 1, 2017
  Released into the public domain.
*/
#ifndef ACS712_h
#define ACS712_h

#include "Arduino.h"

class ACS712
{
  public:
    ACS712(int vcc);
    ACS712();
    int getMidpoint();
    float getMultiplier();
    int getVCC();

  private:
    int midpoint;
    int mv_per_amp;
    float multiplier;
    int vcc;

    void init(int vIn);
};

#endif
