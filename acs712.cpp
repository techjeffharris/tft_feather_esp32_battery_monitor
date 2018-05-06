/*
  ACS712.cpp - Class for monitoring the discharge current of Lead-acid batteries.
  Created by Jeffrey A Harris, July 1, 2017
  Released into the public domain.
*/

#include "Arduino.h"
#include "acs712.h"

namespace defaults
{
  const int MIDPOINT          = 510;
  const int VCC               = 5000;
  const float MV_PER_AMP      = 66;
}


ACS712::ACS712(int vIn)
{
  init(vIn);
}

ACS712::ACS712()
{
  init(defaults::VCC);
}

void ACS712::init(int vIn)
{
  midpoint = defaults::MIDPOINT;
  mv_per_amp = defaults::MV_PER_AMP;
  vcc = vIn;
  multiplier = vcc / mv_per_amp / 1000;
}

int ACS712::getMidpoint()
{
  return midpoint;
}

float ACS712::getMultiplier()
{
  return multiplier;
}

int ACS712::getVCC()
{
  return vcc;
}
