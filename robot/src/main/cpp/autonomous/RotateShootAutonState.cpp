#include "autonomous/RotateShootAutonState.h"

RotateShootAutonState::RotateShootAutonState(Manipulator* manipulator_)
    : manipulator( manipulator_ )
{}

void RotateShootAutonState::AutonomousInit()
{}

void RotateShootAutonState::AutonomousPeriodic()
{
  manipulator->ContinuousShoot(frc::SmartDashboard::GetNumber("Hood Position", 0.5), 0.4, frc::SmartDashboard::GetNumber("Target Speed", 250));
}

void RotateShootAutonState::CheckState()
{}