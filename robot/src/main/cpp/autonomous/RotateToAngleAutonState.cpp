#include "autonomous/RotateToAngleAutonState.h"

RotateToAngleAutonState::RotateToAngleAutonState(TeleopFunctions* teleop_functions_, double angle_)
    : teleop_functions( teleop_functions_ ),
      angle( angle_ )
{}

void RotateToAngleAutonState::AutonomousInit()
{}

void RotateToAngleAutonState::AutonomousPeriodic()
{
  std::cout << "Kayeeeeeey!!" << std::endl;
  teleop_functions->TurnToAngle(angle, frc::SmartDashboard::GetNumber("Turn P Value", 0.002));
}

void RotateToAngleAutonState::CheckState()
{
  if(teleop_functions->GetTurnStatus())
  {
    SetDone(true);
  }
}