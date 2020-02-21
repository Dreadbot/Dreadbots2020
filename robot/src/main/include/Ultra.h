#pragma once

#include <frc/Ultrasonic.h>

class Ultra
{
public:
  Ultra();
  
  float GetDistanceGen();
  frc::Ultrasonic *m_UltraGen = new frc::Ultrasonic(0, 1);
};