#include <Autonomous.h>

Autonomous::Autonomous(SparkDrive *Sparkdrive)
{
m_SparkDrive = Sparkdrive;
}

void Autonomous::RightRight()
{
 m_SparkDrive->TankDrive(1.0, 0.0, false, false, 0.0);
}

void Autonomous::RightCenter()
{

}

void Autonomous::RightLeft()
{

}