#pragma once

#include <frc/Ultrasonic.h>

class Ultra
{
 public:
  Ultra();
  
  float GetDistanceGen();
  float GetDistanceGen2();

 private:
  frc::Ultrasonic *m_UltraGen = new frc::Ultrasonic(0, 1);
  frc::Ultrasonic *m_UltraGen2 = new frc::Ultrasonic(2, 3);
};