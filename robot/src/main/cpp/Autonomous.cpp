#include "Autonomous.h"

Autonomous::Autonomous(SparkDrive *Sparkdrive, std::multimap<units::second_t, AutonState>* time_to_state)
{
  m_SparkDrive = Sparkdrive;
  m_time_to_state = time_to_state;
}

void Autonomous::RightRight()
{
 m_SparkDrive->TankDrive(0.0, 0.0, false, false, 0.0);
}   

void Autonomous::RightCenter()
{

}

void Autonomous::RightLeft()
{

}